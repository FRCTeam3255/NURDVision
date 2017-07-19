alias nurdcompile='cd ~/Desktop;g++ NURDVision.cpp -std=c++11 -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -o runNURDVision; echo "$(tput bold)$(tput setaf 4)NURD$(tput setaf 1)Vision $(tput setaf 2)Complied successfully as runNURDVision"'
alias nurdrun='cd ~/Desktop;./runNURDVision'
alias nurdget='cd ~/Desktop; rm NURDVision.cpp;wget https://github.com/FRCTeam3255/NURDVision/raw/master/NURDVision.cpp;  echo "$(tput bold)$(tput setaf 4)NURD$(tput setaf 1)Vision $(tput setaf 2)Downloaded successfully"'
alias nurdall='nurdget;nurdcompile;nurdrun'
