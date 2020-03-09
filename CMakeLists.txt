cmake_minimum_required(VERSION 3.8)
project(E4Streamer)

set(TESTING OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Import Qt
set(Qt5_Components Widgets Multimedia)
find_package(Qt5 COMPONENTS ${Qt5_Components} REQUIRED)
list(TRANSFORM Qt5_Components PREPEND "Qt5::")

# Import Lab Streaming Layer
find_package(LSL REQUIRED HINTS "extern/liblsl"
        "${CMAKE_CURRENT_LIST_DIR}/../../LSL/liblsl/build/"
        "${CMAKE_CURRENT_LIST_DIR}/../../LSL/liblsl/build/install"
        PATH_SUFFIXES share/LSL
        )

set(Code_Base src/model/Command.cpp src/model/Command.h src/model/Response.cpp src/model/Response.h src/model/Server.cpp src/model/Server.h src/model/Connection.cpp src/model/Connection.h src/model/Device.cpp src/model/Device.h src/model/commands/Connect.cpp src/model/commands/Connect.h src/model/commands/Disconnect.cpp src/model/commands/Disconnect.h src/model/commands/DiscoverDevices.cpp src/model/commands/DiscoverDevices.h src/view/widgets/ServerConfig.cpp src/view/widgets/ServerConfig.h src/view/util/FileSelector.h src/view/util/FileSelector.cpp src/view/ServerPage.cpp src/view/ServerPage.h src/view/Wizard.cpp src/view/Wizard.h src/view/widgets/Devices.cpp src/view/widgets/Devices.h src/view/DevicePage.cpp src/view/DevicePage.h src/model/Sample.cpp src/model/Sample.h src/model/Stream.cpp src/model/Stream.h src/model/commands/Subscription.cpp src/model/commands/Subscription.h src/view/widgets/StreamWidget.cpp src/view/widgets/StreamWidget.h src/view/RecordingPage.cpp src/view/RecordingPage.h src/model/commands/Register.cpp src/model/commands/Register.h src/model/commands/Unregister.cpp src/model/commands/Unregister.h src/model/ConnectionManager.cpp src/model/ConnectionManager.h src/model/Disconnectable.h src/view/ConnectionPage.cpp src/view/ConnectionPage.h src/view/RemoteConnectionPage.cpp src/view/RemoteConnectionPage.h src/view/util/PortBox.cpp src/view/util/PortBox.h)

IF (NOT TESTING)
    add_executable(E4Streamer src/main.cpp ${Code_Base})
    target_link_libraries(E4Streamer ${Qt5_Components} LSL::lsl)

    set(E4Streamer_Current_Target E4Streamer)
ELSE ()
    MESSAGE(STATUS "Creating tests")

    find_package(Qt5Test REQUIRED)
    enable_testing(true)

    add_executable(E4Streamer tests/main.cpp ${Code_Base} tests/CommandTests.cpp tests/ConnectionTests.h)
    add_test(NAME E4Streamer COMMAND E4Streamer)
    target_link_libraries(E4Streamer ${Qt5_Components} Qt5::Test LSL::lsl)
ENDIF ()

IF (WIN32)
    # On Windows, deploy all required DLLs automatically
    get_target_property(QMake_Executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(Qt_Bin_Dir "${QMake_Executable}" DIRECTORY)
    find_program(Win_Deploy_Executable windeployqt HINTS "${Qt_Bin_Dir}")

    add_custom_command(TARGET E4Streamer POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${Qt_Bin_Dir}" "${Win_Deploy_Executable}"
            --verbose 0
            --no-compiler-runtime
            $<IF:$<CONFIG:Debug>,--debug,--release>
            \"$<TARGET_FILE:E4Streamer>\"
            COMMENT "Deploying Qt..."
            )

endif ()