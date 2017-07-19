# NURDVision (In Development)
SuperNURDs implementation of vision co-processing
# NOTE: THIS CODE IS CURRENTLY IN DEVELOPMENT AND NOT READY FOR PUBLIC USE
A public (cleaned up) version will be available shortly

Major resource used - [SMblyRequired](https://github.com/SMblyRequired/Computer-Vision-2017)

### TO DO:
(Maybe) Have HSL constants get data from robotPreferences or main code

## How to Set Up on the Jetson TK1
What you will need: a Jetson TK1, the provided Micro USB cable, a PC running UBUNTU 64 bit, a monitor or TV (or advanced SSH knoledge), ethernet access, a mouse and keyboard with a USB hub, a webcam(works best with an LED ring), and the provided 12v power supply

###Jetson initial setup and flashing:

Step 1: plug micro usb cable into the jetson (small side into the port next to audio jacks, and the large side into your PC running UBUNTU)
plug in TV, webcam, ethernet and Mouse/Keyboard to jetson as well

Step 2: Turn jetson into recovery mode by holding down the recovery button and pressing restart (if the jetson is on already) or power off (if the jetson is off)

Step 3: Open up console (terminal) on your LINUX computer

Step 4: type lsusb into the ubuntu PC's console and check to make sure that there is a NVidia device plugged in

Step 5: Download flashJetson.run 
Type: wget https://github.com/FRCTeam3255/NURDVision/raw/master/flashJetson.run; bash ./flashJetson.run

Step 6: roll through the user intuitive flash process (this may take a few hours) selecting Jetson Tk1 where available
Follow the on screen steps. It should say flash succesfull
You may be asked to enter an ip address on the jetson type: ifconfig then type inte addr: (example 192.168.0.0) you see into the prompt on your computer click continue.

###Jetson NURDVision installation: 
(no longer need the PC all this work is done on the jetson) (alterantively can be done by typing: ssh ubuntu@(IP_ADDRESS_FOUND_IN_IFCONFIG password:ubuntu and typing commands there instead)

Step 1: download install.sh to jetson 
Type: wget https://github.com/FRCTeam3255/NURDVision/raw/master/install.sh; bash ./install.sh

(this may take a few hours)

Step 2: once complete type 'nurdall' into the jetsons terminal and you're all set
Note: 	nurdget - downloads NURDVision from github
		nurdcompile - compliles NURDVision as runNURDVision
		nurdrun - runs runNURDVision
		nurdall - runs nurdget; nurdcompile; nurdrun;

Any questions contact Mike at the SuperNURDs

## C++ Libraries
	-lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab

## C++ Compiler arguments
	g++ NURDVision3.0.cpp -std=c++11 -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -o runNURDVision
