// =======================================================
// NURDVision 3.0
// written by Mike with help from Tayler
// =======================================================

#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>

//to my understanding 'namespaces' are used basically before each command, so 'createMask' is actually cv::createMask, and RNG is std::RNG
using namespace std;
using namespace cv;

//Store a double array for either upper or lower boundaries of the hsl filter, decides what color you're looking for in the first mask
// ========= Constants for Ball tracking ============//
// Store an array: [0] = lower bound, [1] = upper bound
//double Hue[] = {29.138490103768287, 58.13721981270055};
//double Saturation[] = {64.20863309352518, 255.0};
//double Luminance[] = {75.67446043165468, 255.0};
// =================================================//

// ========= Constants for Tape tracking ============//
// Store an array: [0] = lower bound, [1] = upper bound
double Hue[] = {79, 91};
double Saturation[] = {170, 255};
double Luminance[] = {46, 255};
// =================================================//


// Converts image to hsl filter, filtering out any color besides retroreflective tapes
void hslThreshold(Mat &input, double hue[], double sat[], double lum[], Mat &output) {
		cvtColor(input, output, COLOR_BGR2HLS);
		inRange(output, Scalar(hue[0], lum[0], sat[0]), Scalar(hue[1], lum[1], sat[1]), output);
	}

// Creates a workable mask to a mat, so we can process the hsl filter
void createMask(Mat &input, Mat &mask, Mat &output) {
	mask.convertTo(mask, CV_8UC1);
	bitwise_xor(output, output, output);
	input.copyTo(output, mask);
}

// Creates contours
void createContours(Mat &input, Mat &output){
	//Generates random number 1,2,3,4, or 5
	RNG rng(12345);
	//makes vectors to store contour information
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//converts our hsl filter into black and white, because findContours requires you to be using black and white, not RGB or HSL	
	cvtColor(input, input, CV_RGB2GRAY);
	//finds the contours of the image
	findContours(input, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//creates mat for the drawing of the contours
	Mat drawing = Mat::zeros(input.size(), CV_8UC3);

	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	output = drawing;
}

// Does the image processing
void processImage(Mat& input, Mat& output){
	Mat hslOutput, maskOutput, contoursOutput;
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	createContours(maskOutput, contoursOutput);
	output = contoursOutput;
}

// Returns true to quit when "ESC" is pressed
bool quit(){
	char key = (char)waitKey(5);
	return( key == 27 || key == 'q' || key == 'Q' ); // 'ESC'
}

int main() {
	//Creates mats for storing image
	Mat raw, processed;
	//Starts video capture of camera 0;
	VideoCapture capture(0);
	
	cout << "Viewer opened" << endl
		 << "Press ESC or Q to terminate" << endl;		
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