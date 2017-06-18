// =======================================================
// NURDVision 3.0
// written by Mike with help from Tayler
// =======================================================

#include <opencv2/opencv.hpp>

// ****** CUDA ******* //
//#include <opencv2/gpu/gpu.hpp>
// ****** CUDA ******* //

#include <iostream>
#include <stdlib.h>

//to my understanding 'namespaces' are used basically before each command, so 'createMask' is actually cv::createMask, and RNG is std::RNG
using namespace std;
using namespace cv;

//Store a double array for either upper or lower boundaries of the hsl filter, decides what color you're looking for in the first mask
// ========= Constants for Tape tracking ============//
// Store an array: [0] = lower bound, [1] = upper bound
const double Hue[] = {0, 49};
const double Saturation[] = {0, 70};
const double Luminance[] = {245, 255};
// =================================================//

// Converts image to hsl filter, filtering out any color besides retroreflective tapes
void hslThreshold(Mat &input, const double hue[], const double sat[], const double lum[], Mat &output) {
	cvtColor(input, output, COLOR_BGR2HLS);
	inRange(output, Scalar(hue[0], lum[0], sat[0]), Scalar(hue[1], lum[1], sat[1]), output);
}

// Creates a workable mask to a mat, so we can process the hsl filter
void createMask(Mat &input, Mat &mask, Mat &output) {
	mask.convertTo(mask, CV_8UC1);
	bitwise_xor(output, output, output);
	input.copyTo(output, mask);
}

// Filters out contours 
void filterContours(vector<vector<Point> > &input, vector<vector<Point> > &output) {
	output.clear();
	for (vector<cv::Point> contour : input) {
		cv::Rect bb = boundingRect(contour);
		if (bb.width < 6 || bb.width > 1000000) continue;
		if (bb.height < 6 || bb.height > 1000000) continue;
		double area = contourArea(contour);
		if (area < 50) continue;
		if (arcLength(contour, true) < 100) continue;
		output.push_back(contour);
 	}
}



// IMPORTED *******
Point2f centerPoint(cv::Rect rect) {
	return cv::Point2f(rect.x + (rect.width / 2), rect.y + (rect.height / 2));
}
double aimCoords(double pos, double res) {
		return (pos - (res / 2)) / (res / 2);
}

Point2f aimCoordsFromPoint(cv::Point2f point, cv::Size res) {
		return Point2f(aimCoords(point.x, (double)res.width), aimCoords(point.y, (double)res.height));
}
// IMPORTED *******

// Creates contours
void createContours(Mat &input, Mat &output, vector<vector<Point> > &contoursOutput){
	//makes vectors to store contour information
	vector<vector<Point> > contoursInput;
	vector<Vec4i> hierarchy;
	//converts our hsl filter into black and white, because findContours requires you to be using black and white, not RGB or HSL	
	cvtColor(input, input, CV_RGB2GRAY);
	//finds the contours of the image
	findContours(input, contoursInput, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//creates mat for the drawing of the contours
	Mat drawing = Mat::zeros(input.size(), CV_8UC3);
	
	filterContours(contoursInput, contoursOutput);
	
	
	Scalar color = Scalar(250, 206, 135);
	for (int i = 0; i< contoursOutput.size(); i++)
	{
		drawContours(drawing, contoursOutput, i, color, 2);
		
//* ************ COPIED ************** *//
		// Adds rectangles
		rects[i] = conRect;
		
		
		double area = conRect.area();
//* ************ COPIED ************** *//

	}
	output = drawing;
}

// Does the image processing
void processImage(Mat& input, Mat& output){
	vector<vector<Point> > contoursValueOutput;
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	createContours(maskOutput, contoursImageOutput, contoursValueOutput);
}

// Returns true to quit when "ESC" is pressed
bool quit(){
	char key = (char)waitKey(5);
	return( key == 27 || key == 'q' || key == 'Q' ); // 'ESC'
}

int main() {
	
	// ****** CUDA ******* //
//	gpu::setDevice(0);
	// ****** CUDA ******* //
	
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