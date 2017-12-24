# NURDVision
**[Documentation](https://github.com/FRCTeam3255/NURDVision/wiki/) | 
[Jetson Setup](https://github.com/FRCTeam3255/NURDVision/wiki/NVIDIA-Jetson-Setup) | 
[Jetson Installation](https://github.com/FRCTeam3255/NURDVision/wiki/Installation-on-Jetson) | 
[PC Installation](https://github.com/FRCTeam3255/NURDVision/wiki/Installation-on-PC) | 
[Notes and Info](https://github.com/FRCTeam3255/NURDVision/wiki/Notes-and-Info) | 
[FRC Robot Implementation](https://github.com/FRCTeam3255/NURDVisionRobot)**

## SuperNURDs implementation of vision co-processing

**This code was primarily developed for use on a [NVIDIA Jetson TK1](http://www.nvidia.com/object/tegra-k1-processor.html) connected over a network to a [FIRST Robotics Competition](https://www.firstinspires.org/robotics/frc) Robot.**

Please read our [wiki](https://github.com/FRCTeam3255/NURDVision/wiki/) for documentaion and tutorials on how to use NURDVision.

**Note:** This is the runnable program and setup of NURDVision, to see a simple implementation example on an FRC Robot visit: 
[NURDVisionRobot GitHub](https://github.com/FRCTeam3255/NURDVisionRobot)

### TO DO:
- [ ] Add boiler vs peg detection
- [ ] Have jetson send failed camera message to roborio
- [ ] Add java install and outline viewer/smartdashboard to pc install
- [ ] Run NURDVision on Jetson boot (Check to see if working properly on fresh jetson)
- [x] Move readme conents to wiki pages
- [x] Add how to see jetson from roboRIO
- [x] Have HSL constants get data from robotPreferences or main code for tuning at competition
- [x] Implement CSCore to send video data to roborio/driverstation for tuning at competition
- [x] Implement network tables
