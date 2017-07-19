##SETUP OPENCV

#get ubuntu up to date and add repo

sudo add-apt-repository universe
sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y dist-upgrade
sudo apt-get -y autoremove

# Some general development libraries
sudo apt-get -y install build-essential make cmake cmake-curses-gui g++
# libav video input/output development libraries
sudo apt-get -y install libavformat-dev libavutil-dev libswscale-dev
# Video4Linux camera development libraries
sudo apt-get -y install libv4l-dev
# Eigen3 math development libraries
sudo apt-get -y install libeigen3-dev
# OpenGL development libraries (to allow creating graphical windows)
sudo apt-get -y install libglew1.6-dev
# GTK development libraries (to allow creating graphical windows)
sudo apt-get -y install libgtk2.0-dev

cd ~/Downloads
rm 3.2.0.zip
wget https://github.com/opencv/opencv/archive/3.2.0.zip
cd ~/Downloads
unzip 3.2.0.zip
mv opencv-3.2.0 ~/
cd ~/opencv-3.2.0
mkdir build
cd build

cmake -DWITH_CUDA=ON -DCUDA_ARCH_BIN="3.2" -DCUDA_ARCH_PTX="" -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF ..

cmake -DCMAKE_BUILD_TYPE=RELEASE -DWITH_CUBLAS=ON -DENABLE_FAST_MATH=ON -DCUDA_FAST_MATH=ON ..

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

#SETUP OUR CODE
cd
cd ~/Desktop
rm NURDVision.cpp
wget https://github.com/FRCTeam3255/NURDVision/blob/master/NURDVision.cpp
g++ NURDVision.cpp -std=c++11 -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -o runNURDVision
#CODE SETUP DONE

#alias setup
cd
rm ./.bash_aliases
wget https://raw.githubusercontent.com/FRCTeam3255/NURDVision/master/.bash_aliases





