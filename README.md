# E4Streamer
Integration of the [Empatica E4](https://www.empatica.com/research/e4/) into the [Lab Streaming Layer](https://github.com/sccn/labstreaminglayer), currently "work in progress".

## Overview
In order to gain data from subjects utilizing the wristband Empathica E4, a pipeline of different tools is required. Given the complexity of streaming procedure, the workflow may not seem straightforward. Nevertheless, this tutorial will guide you towards a successful recorded session. It is important to understand the job of each part in the pipeline. Therefore, we will discuss each of them shortly:
- **E4 Streaming Server**: The E4 Streaming Server is a proprietary tool by the company Empathica. Given a specific Bluetooth dongle and an API key bound to a specific device, it provides an API for external software to communicate with the E4.
- **E4Streamer**: The E4Streamer is the tool developed by me. It connects to a running E4 Streaming Server and uses it to control the E4. Once successfully started, it provides different streams filled by the live data recieved from the E4. These streams follow the Lab Streaming Layer protocol and can be subscribed by every software on the system.
- **Lab Recorder**: The software Lab Recorder by the authors of Lab Streaming Layers allows the recording on streams on the system. It writes all the data provided into a XDF file.

## Starting a session
The following section describes the proposed outline for starting a successful session.
1. Start the **Lab Recorder** by running the file *LabRecorder.exe* and specify an appropriate storage path and subject ID for the recording.
2. Let the subject take on the Empathica E4 wristband. Consult the small manuals for an proposed procedure.
3. Start the **E4 Streaming Server** by running the file *EmpaticaBLEServer.exe*, normally found in *C:\Program Files (x86)\Empatica\EmpaticaBLEServer*. Follow the guidance until a overview about all devices appear. Do not worry, if our E4 is not found by than. 
   1. Click on the gear nearby the top right corner 
      1. Notice for the port
      2. Ensure *Connection* is set to *Automatic*
   2. Minimize the server window. You will not need any more interaction with it, but ensure the server is running in the background.
4. Start the **E4Streamer** for creating the streams of interest.
   1. Specify the port the E4 Streaming Server is running. By default, this will be 28000, but it may vary. Type the the port you just noticed in the server and click on "Next".
   2. Power on the **Empathica E4** by holding down the button for 0.5 - 1 second.
   3. In the device overview, the device will appear once the server discovered it via Bluetooth. You may press F5 for enforcing an update. Once the device appears in the list, double click on it. The former gray button next will than allow to continue.
5. In the **LabRecorder**, press the *Update* button. A list of all available Empathica E4 streams will appear. Select the streams of interest and press *Start*. **The recorder is recording, but does not get any data from the E4Streamer!**
6. In the **E4Streamer**, mark all the streams you want to record. In the moment the checkbox gets activated, data is read from the Empathica E4, translated into the appropriate format and stored into the streams. Do <u>not</u> click on *Finish*, as this will just stop the tool.
7. (Once the study is ready, let the subject press the button on the Empathica shortly. This will send a marker which is stored into a stream, too. This marker might later be used to specify the begin of the actual experiment.)

## Stopping a session
The following section describes the proposed outline for stopping a session.
1. In the **Lab Recorder**, press *Stop* and close the window.
2. In the **E4Streamer**, press *Finish*. The tool will shut down the Empathica E4 and close itself automatically.
3. Close the **E4 Streaming Server**.
