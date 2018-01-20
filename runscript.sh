#! /bin/bash                                                                                                                                                                                                
if ! screen -list | grep -q "NURDVision"; then
	v4l2-ctl -c exposure_auto=01 -c exposure_absolute=01
	cd ~/Desktop/
	screen -S NURDVision -d -m ./runNURDVision
fi
