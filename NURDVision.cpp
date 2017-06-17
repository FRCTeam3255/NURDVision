// =======================================================
// NURDVision
// =======================================================

#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

// ========= Constants for Ball tracking ============//
//Store an array: [0] = lower bound, [1] = upper bound
double Hue[] = {29.138490103768287, 58.13721981270055};
double Saturation[] = {64.20863309352518, 255.0};
double Luminance[] = {75.67446043165468, 255.0};
// =================================================//

// Converts the image to be within the hslThreshold
void hslThreshold(Mat &input, double hue[], double sat[], double lum[], Mat &output) {
		cvtColor(input, output, COLOR_BGR2HLS);
		inRange(output, Scalar(hue[0], lum[0], sat[0]), Scalar(hue[1], lum[1], sat[1]), output);
	}

// Creates a mask	
void createMask(Mat &input, Mat &mask, Mat &output) {
	mask.convertTo(mask, CV_8UC1);
	bitwise_xor(output, output, output);
	input.copyTo(output, mask);
}

// Does the image processing
void processImage(Mat& input, Mat& output){
	Mat hslOutput, maskOutput;
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	output = maskOutput;
}

// Returns true to quit when "ESC" is pressed
bool quit(){
	int k = waitKey(5);
	return (k == 27);
}

int main() {
	//Creates mats for storing image
	Mat raw, processed;
	//Starts video capture of camera 0;
	VideoCapture capture(0);
	
	cout << "Viewer opened" << endl
		 << "Press ESC to terminate" << endl;		
	// While the quit fucntion does not return true run image functions
	while (!quit()) {
		//Stores capture to ram mat
		capture.read(raw);
		//Runs image processing - give mats raw and proccessed
		processImage(raw, processed);
		// show processed image
		imshow("Processed image", processed);
	}
	cout << "Viewer closed successfully";
	
	return 0;
}