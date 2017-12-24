# NURDVision
## SuperNURDs implementation of vision co-processing on the Jetson TK1

**This code was primarily developed for use on a NVIDIA Jetson TK1 connected over a network to a FIRST Robotics Competition Robot.**

**Note:** This is the runnable program and setup of NURDVision, to see a simple implementation example on an FRC Robot visit: 
[NURDVisionRobot GitHub](https://github.com/FRCTeam3255/NURDVisionRobot)

Major resource used - [SMblyRequired](https://github.com/SMblyRequired/Computer-Vision-2017)

## Tutorials and Info
- [How to Set Up on the Jetson TK1](https://github.com/FRCTeam3255/NURDVision/wiki/NVIDIA-Jetson-Setup)
- [Jetson NURDVision Installation](https://github.com/FRCTeam3255/NURDVision/wiki/Installation-on-Jetson)
- [Linux PC NURDVision Installation](https://github.com/FRCTeam3255/NURDVision/wiki/Installation-on-PC)
- [Additional Notes and Info](https://github.com/FRCTeam3255/NURDVision/wiki/Notes-and-Info)

### TO DO:
- [ ] Add boiler vs peg detection
- [ ] Have jetson send failed camera message to roborio
- [ ] Add java install and outline viewer/smartdashboard to pc install
- [ ] Run NURDVision on Jetson boot (Check to see if working properly on fresh jetson)
- [x] Move readme conents to wiki pages. **COMPLETED**
- [x] Add how to see jetson from roboRIO. **COMPLETED**
- [x] Have HSL constants get data from robotPreferences or main code for tuning at competition **COMPLETED**
- [x] Implement CSCore to send video data to roborio/driverstation for tuning at competition **COMPLETED**
- [x] Implement network tables **COMPLETED**