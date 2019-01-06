// =======================================================
// HW#: 
// Your name: Tayler Uva
// Compiler:  g++
// File type: ** file
// =======================================================

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool quit(){
	char key = (char)waitKey(5);
	return( key == 27 || key == 'q' || key == 'Q' ); // key 27 = 'ESC'
}


int main() {
	Mat raw, processed;
		// Starts video capture of camera 0;
		VideoCapture capture(0);
		if (!capture.isOpened()) {
			cerr << "\n\e[31mERROR! Unable to open camera\nERROR! Is the camera connected?\e[0m\n";
			return 1;
		}
		// While the quit fucntion does not return true run image functions
		while (!quit()) {
			// Stores capture to raw mat
			capture.read(raw);
			imshow("Processed image", raw);
			}
	return 0;
}