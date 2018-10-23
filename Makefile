CXX			:= g++
CXX_FLAGS	:= -std=c++11 -O2 -lntcore -lwpiutil -lcscore
OPENCV_FLAGS := -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab

INCLUDE		:= include

LIBRARIES	:= 
EXECUTABLE	:= runNURDVision

all: $(EXECUTABLE)

run: clean all
	clear
	./$(EXECUTABLE)

debug: clean all
	clear
	./$(EXECUTABLE) -debug

$(EXECUTABLE): NURDVision.cpp
	$(CXX) $(CXX_FLAGS) $(OPENCV_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(EXECUTABLE)

#EOF