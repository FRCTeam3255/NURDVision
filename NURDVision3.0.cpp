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

bool lockAcquired = false;
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
	
//* ************ COPIED ************** *//
	// Bounding rectangles around contours
	vector<Rect> rects(contoursOutput.size());
	vector<RotatedRect> rotRects(contoursOutput.size());	// Rotated bounding rectangles around contours	
	
	// Cross hairs
	line(drawing, Point(drawing.cols / 2, 0), Point(drawing.cols / 2, drawing.rows), (lockAcquired ? Scalar(0, 255, 0) : Scalar(0, 0, 255)), 1);
	line(drawing, cv::Point(0, drawing.rows / 2), cv::Point(drawing.cols, drawing.rows / 2), (lockAcquired ? Scalar(0, 255, 0) : Scalar(0, 0, 255)), 1);
//* ************ COPIED ************** *//
	
	Scalar color = Scalar(250, 206, 135);
	for (int i = 0; i< contoursOutput.size(); i++)
	{
		drawContours(drawing, contoursOutput, i, color, 2);
		
//* ************ COPIED ************** *//
		// Adds rectangles
		Rect conRect = boundingRect(contoursOutput[i]);
		rects[i] = conRect;
		rotRects[i] = minAreaRect(contoursOutput[i]);
		rectangle(drawing, conRect, Scalar(255, 255, 0), 2);
		
		
		Point2f aimPoint = aimCoordsFromPoint(centerPoint(conRect), drawing.size());
		(aimPoint.x < 0.1 && aimPoint.y < 0.1) ? lockAcquired = true : lockAcquired = false;
		double area = conRect.area();
		putText(drawing, "Aim: "+ to_string(aimPoint.x) + ", " + to_string(aimPoint.y), centerPoint(conRect), FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255), 1);
		putText(drawing, "Area: " + to_string(conRect.area()) + " = " + to_string(conRect.width) + "*" + to_string(conRect.height), centerPoint(conRect) + Point2f(0, 15), cv::FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255), 1);
//* ************ COPIED ************** *//

	}
	output = drawing;
}

// Does the image processing
void processImage(Mat& input, Mat& output){
	Mat hslOutput, maskOutput, contoursImageOutput;
	vector<vector<Point> > contoursValueOutput;
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	createContours(maskOutput, contoursImageOutput, contoursValueOutput);
	output = contoursImageOutput;
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