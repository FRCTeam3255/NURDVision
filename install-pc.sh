#!/bin/bash
##NURDVISION INSTALLATION
echo "$(tput bold)Starting $(tput setaf 4)NURD$(tput setaf 1)Vision $(tput setaf 2)installation - INTERNET CONNECTION REQUIRED"

#UPDATES AND OTHER INSTALLS
#get ubuntu up to date and add repo

sudo add-apt-repository universe
sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y dist-upgrade
sudo apt-get -y autoremove

# LibsSTDC++6 linking libraries (to compile NURDVision properly with g++)
sudo apt-get -y install libstdc++6
# Some general development libraries
sudo apt-get -y install build-essential make cmake cmake-curses-gui g++
# libav video input/output development libraries
sudo apt-get -y install libavformat-dev libavutil-dev libswscale-dev
# Video4Linux camera development libraries
sudo apt-get -y install libv4l-dev v4l-utils
# Eigen3 math development libraries
sudo apt-get -y install libeigen3-dev
# OpenGL development libraries (to allow creating graphical windows)
sudo apt-get -y install libglew1.6-dev
# GTK development libraries (to allow creating graphical windows)
sudo apt-get -y install libgtk2.0-dev
# Emacs and nano
sudo apt-get -y install emacs nano

# Refresh installed libraries (if error @GLIBCXX_3.4.20 occurs follow steps: https://askubuntu.com/a/409831
sudo apt-get -y dist-upgrade

#UPDATES AND OTHER INSTALLS DONE

#OPENCV SETUP
cd ~/Downloads
rm 3.2.0.zip
wget https://github.com/opencv/opencv/archive/3.2.0.zip
cd ~/Downloads
unzip 3.2.0.zip
mv opencv-3.2.0 ~/
cd ~/opencv-3.2.0
mkdir build
cd build

cmake -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF ..

cmake -DCMAKE_BUILD_TYPE=RELEASE -DENABLE_FAST_MATH=ON ..

cd ~/Downloads
rm 3.2.0.zip
wget https://github.com/opencv/opencv_contrib/archive/3.2.0.zip
unzip 3.2.0.zip
mv opencv_contrib-3.2.0 ~/
cd ~/opencv-3.2.0/build
cmake -DOPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.2.0/modules ..

make -j4
sudo make install
#OPENCV SETUP DONE

echo "$(tput bold)$(tput setaf 2)OpenCV setup complete"

#LIBS & INCLUDE SETUP
#Libs & include setup is based on: https://github.com/SMblyRequired/Computer-Vision-2017/blob/master/installDeps.sh
cd
mkdir /tmp/libinstall
cd /tmp/libinstall
wget -O wpiutil.zip http://first.wpi.edu/FRC/roborio/maven/release/edu/wpi/first/wpilib/wpiutil/1.0.2/wpiutil-1.0.2-desktop.zip
wget -O ntcore.zip http://first.wpi.edu/FRC/roborio/maven/release/edu/wpi/first/wpilib/networktables/cpp/NetworkTables/3.1.7/NetworkTables-3.1.7-desktop.zip

unzip wpiutil.zip
unzip ntcore.zip

sudo cp -r /tmp/libinstall/include/* /usr/local/include/
sudo cp -r /tmp/libinstall/Linux/amd64/* /usr/local/lib #use arm folder since JetsonTK1 is 32bit
sudo ldconfig
rm -rf /tmp/libinstall
#LIBS & INCLUDE SETUP DONE

#GRIP SETUP
cd
wget -O grip.deb https://github.com/WPIRoboticsProjects/GRIP/releases/download/v1.5.2/grip-1.5.2-x64.deb
sudo apt-get install ./grip.deb
#GRIP SETUP DONE

#ALIAS SETUP - CURRENTLY DISABLED
cd
wget https://raw.githubusercontent.com/FRCTeam3255/NURDVision/master/nurdcommands.txt
cat nurdcommands.txt >> .bash_aliases
#ALIAS SETUP DONE

#SETUP OUR CODE
cd
cd ~/Desktop
rm NURDVision.cpp
wget https://github.com/FRCTeam3255/NURDVision/raw/master/NURDVision.cpp
g++ NURDVision.cpp -std=c++11 -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -lntcore -lwpiutil -o runNURDVision
#CODE SETUP DONE

echo "$(tput bold)$(tput setaf 4)NURD$(tput setaf 1)Vision $(tput setaf 2)installation complete"
