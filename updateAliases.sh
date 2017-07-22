#Libs setup
cd
cd ~/Desktop
wget https://github.com/FRCTeam3255/NURDVision/raw/master/libs.zip
unzip libs.zip
#Libs setup done

#SETUP OUR CODE
cd
cd ~/Desktop
rm NURDVision.cpp
wget https://github.com/FRCTeam3255/NURDVision/raw/master/NURDVision.cpp
g++ NURDVision.cpp -Wall -std=c++11  -L ./Linux/arm/ -lstdc++ -lntcore -pthread  -Iinclude/ -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -o runNURDVision
#CODE SETUP DONE

#alias setup
cd
rm ./.bash_aliases
wget https://raw.githubusercontent.com/FRCTeam3255/NURDVision/master/.bash_aliases
source ~/.bash_aliases

echo "$(tput bold)$(tput setaf 4)NURD$(tput setaf 1)Vision $(tput setaf 2)installation complete"





