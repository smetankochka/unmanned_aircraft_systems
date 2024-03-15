# Python software package for NTO 2024 final

This package contains python program template for solving computer vision problems during the finals.

The setup is as follows:

- Aviant simulator with ESP32 and PixelStreaming support (mar 2024 and later)

- ESP32 DOIT board with external COM port connected to UART3

- This software

Simulator connects to ESP32 via UART0 and UART3 connects to python script (it is used to send commands from python to ardu).

Telemetry is received by ESP32 through UART0 and by python script through local UDP server.

## Required packages

### Python libraries:

```
pip3 install opencv-python
pip3 install pyserial
```

### GO programming language

Follow the instructions on https://go.dev/dl/

On first launch the GO script will download the required packages, so it is essential to run in separately the first time around:

```
go run ./main.go
```

![gorun](C:\Users\Shen\Desktop\gorun.PNG)

The installation process might take some time.

  ### Node.js packages

These are required to launch PixelStreaming part of Aviant simulator. Simply run run.bat script and everything will be install automatically:

```
./run.bat
```

## Installation

For python to be able to real RTP stream via .sdp file the following environmental variable:

```
OPENCV_FFMPEG_CAPTURE_OPTIONS
protocol_whitelist;file,rtp,udp
```

Keep in mind that GO script will ask for network permissions on each launch, so it is recommended to disable windows defender firewall for local networks.

## Testing

The simulator should be launched with run.bat script for PixelStreaming to become active.

After launch you can setup the simulation parameters in the main menu and start the simulation. The PixelStreaming only works while the simulation is active. You can check that that image is transferred correctly by visiting http://localhost/?StreamerId=Camera1.

To convert this image to RTP stream a GO script is used. You can launch it separately from the terminal:

```
go run ./main.go
```

or run the python script - it will launch the GO automatically.

Either way, after the GO script is launched it will generated RTP stream that can be accessed by `rtp-forwarder.sdp` file. You can open it in python `test_stream.py` or a VLC player to check that the stream is active.

