# NURDVision (In Development)
SuperNURDs implementation of vision co-processing
# NOTE: THIS CODE IS CURRENTLY IN DEVELOPMENT AND NOT READY FOR PUBLIC USE
A public (cleaned up) version will be available shortly

Major resource used - [SMblyRequired](https://github.com/SMblyRequired/Computer-Vision-2017)

### TO DO:
(Maybe) Have HSL constants get data from robotPreferences or main code

## How to Set Up on the Jetson TK1
What you will need: a Jetson TK1, the provided Micro USB cable, a PC running UBUNTU 64 bit, a monitor or TV (or advanced SSH knoledge), ethernet access, a mouse and keyboard with a USB splitter, a webcam(works best with an LED ring), and the provided 12v power supply

Step 1: Download flashJetson.run

Step 2: plug micro usb cable into the jetson (small side intto the port next to audio jacks, and the large side into your PC running UBUNTU)

Step 3: Turn jetson into recovery mode by holding down the recovery button and pressing restart (if the jetson is on already) or power off (if the jetson is off)

Step 4: type lsusb into the ubuntu PC's console and check to make sure that there is a NVidia device plugged in

Step 5: open flashJetson.run by going to the directory you downloaded it in and typing './flashJetson.run' into the PC's console

Step 6: roll through the user intuitive flash process (this may take a few hours)

Step 7: plug in TV, webcam, ethernet and Mouse/Keyboard to jetson (no longer need the PC)

Step 8: download install.sh to jetson (anywhere on the jetson is fine)

Step 9: go to the directory that the install script is in and type 'bash ./install.sh' (this may take a few hours)

Step 10: type 'nurdall' into the jetsons terminal and you're all set 

Any questions contact Mike at the SuperNURDs

## C++ Libraries
	-lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab

## C++ Compiler arguments
	g++ NURDVision3.0.cpp -std=c++11 -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -o runNURDVision
