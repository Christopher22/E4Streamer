cmake_minimum_required(VERSION 3.8)
project(E4Streamer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt5_Components Widgets Multimedia)
find_package(Qt5 COMPONENTS ${Qt5_Components} REQUIRED)
list(TRANSFORM Qt5_Components PREPEND "Qt5::")

add_executable(E4Streamer src/main.cpp)
target_link_libraries(E4Streamer ${Qt5_Components})

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