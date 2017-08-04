# NURDVision (In Development)
SuperNURDs implementation of vision co-processing on the Jetson TK1
# NOTE: THIS CODE IS CURRENTLY IN DEVELOPMENT AND NOT READY FOR PUBLIC USE
A public (cleaned up) version will be available shortly

Major resource used - [SMblyRequired](https://github.com/SMblyRequired/Computer-Vision-2017)

### TO DO:
- Run NURDVision on Jetson boot
- Add boiler vs peg detection
- (Maybe) Have HSL constants get data from robotPreferences or main code
- (Maybe) Implement CSCore to send video data to roborio/driverstation
- ~~Implement network tables~~ **COMPLETED**

## How to Set Up on the Jetson TK1
What you will need: a Jetson TK1, the provided Micro USB cable, a PC running UBUNTU 64 bit, a monitor or TV, ethernet access, a mouse and keyboard with a USB hub, a webcam(works best with an LED ring), and the provided 12v power supply

### Jetson initial setup and flashing:

__Step 1__: Plug micro usb cable into the jetson (small side into the port next to audio jacks, and the large side into your PC running UBUNTU).

__Step 2__: Plug in TV, webcam, ethernet and Mouse/Keyboard to jetson

__Step 3__: Turn jetson into recovery mode by holding down the recovery button and pressing restart (if the jetson is on already) or power off (if the jetson is off)

__Step 4__: Open up console (terminal) on your LINUX computer

__Step 5__: Type lsusb into the ubuntu PC's console and check to make sure that there is a NVidia device plugged in

__Step 6__: Download flashJetson.run Type: 
	
	 mkdir jetsonInstall; cd jetsonInstall; wget https://github.com/FRCTeam3255/NURDVision/raw/master/flashJetson.run; bash ./flashJetson.run

__Step 7__: Continue through the user intuitive flashing process (this may take a few hours) selecting Jetson Tk1 where available

Follow the on screen steps. Where it says Host - Ubuntu, click install, then no action

_**DO NOT INSTALL OPENCV OR ANY SAMPLES AT THIS POINT**_. 

It should say flash succesfull.

~~You may be asked to enter an ip address. On the jetson type: `ifconfig` then type result inte addr: (example 192.168.0.0) into the prompt on your LINUX computer. Click continue.~~

_Jetson intial setup complete!_

### Jetson NURDVision installation: 
A PC is no longer needed as this work is done on the jetson

(alterantively can be done by ssh by typing: ssh ubuntu@tegra-ubuntu password:ubuntu and typing commands there instead)

__Step 1__: download install.sh to jetson. Type: 
	
	wget https://github.com/FRCTeam3255/NURDVision/raw/master/install.sh; bash ./install.sh

(this may take a few hours)

__Step 2__: once complete you can now download the latest code, compile and run it, type:

	nurdall

_Jetson NURDVision installation complete!_

### Custom command info: 	

	nurdget 		downloads latest NURDVision from github
	nurdcompile		compliles NURDVision as runNURDVision
	nurdrun			runs runNURDVision
	nurdrun	-debug		runs runNURDVision with viewer
		
	nurdall			runs nurdget, nurdcompile, and nurdrun -debug;

These commands can be run invidually if needed (i.e. only nurdcompile and nurdrun if disconnected from internet)

Any questions contact Mike at the SuperNURDs

### Additional info:
Our belief is that the Jetson TK1 requires ARMHF libraries as such, those are the libraries installed with the install script.
#### C++ Compiler arguments (nurdcompile)
	g++ NURDVision.cpp -std=c++11 -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -lntcore -lwpiutil -o runNURDVision
