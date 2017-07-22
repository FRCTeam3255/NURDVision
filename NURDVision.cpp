// =======================================================
// NURDVision
// written by Mike with help from Tayler
// =======================================================

#include <opencv2/opencv.hpp>

#include <chrono>
#include <cstdio>
#include <thread>
#include "ntcore.h"

#include "networktables/NetworkTable.h"

#include <iostream>
#include <stdlib.h>

//'namespaces' are used basically before each command, so 'createMask' is actually cv::createMask. This removes need to do so.
using namespace std;
using namespace cv;

// Store a double array for both lower and upper boundaries of the hsl filter, decides what color you're looking for in the first mask
// ========= Constants for Tape tracking ============//
// Store an array: [0] = lower bound, [1] = upper bound
const double Hue[] = {0, 49};
const double Saturation[] = {150, 255};
const double Luminance[] = {245, 255};

const double OBJECT_AREA = 2.5; //Area of the tracking tape in real life
const double PIXEL_AREA = 30000; //Area of the pixels at 10 inches
const double BASE_DISTANCE = 10; //Distance from camera to use as base for calucation
// =================================================//

// ========= Constants for Phone tracking ============//
// Store an array: [0] = lower bound, [1] = upper bound
//const double Hue[] = {0, 0};
//const double Saturation[] = {0, 0};
//const double Luminance[] = {255, 255};
//
//const double OBJECT_AREA = 2.5; //Area of the tracking tape in real life
//const double PIXEL_AREA = 30000; //Area of the pixels at 10 inches
//const double BASE_DISTANCE = 10; //Distance from camera to use as base for calucation
// =================================================//

// ============ Color Constants ============ //
// Scalars writen in BGR format
const Scalar SKY_BLUE = Scalar(250, 206, 135);
const Scalar RED = Scalar(0, 0, 255);
const Scalar GREEN = Scalar(0, 255, 0);
const Scalar BLUE = Scalar(255, 0, 0);
const Scalar YELLOW = Scalar(0, 255, 255);
const Scalar WHITE = Scalar(255, 255, 255);
// ========================================= //

// Math to find the focal length of the camera
const double FOCAL_LENGTH = (PIXEL_AREA * BASE_DISTANCE)/OBJECT_AREA;

// Structure for sorting bounding boxes in descending size order
struct {
	bool operator() (cv::Rect a, cv::Rect b) {
		return b.area() < a.area();
	}
} boundingBoxSortDescending;

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
	for (vector<Point> contour : input) {
		Rect contourBounding = boundingRect(contour);
		if (contourBounding.width < 6 || contourBounding.width > 1000000) continue;
		if (contourBounding.height < 6 || contourBounding.height > 1000000) continue;
		double area = contourArea(contour);
		if (area < 50) continue;
		if (arcLength(contour, true) < 100) continue;
		output.push_back(contour);
 	}
}

// Creates contours
void createContours(Mat &input, vector<vector<Point> > &contoursOutput){
	//makes vectors to store contour information
	vector<vector<Point> > contoursInput;
	//converts our hsl filter into black and white, because findContours requires you to be using black and white, not RGB or HSL	
	cvtColor(input, input, CV_RGB2GRAY);
	//finds the contours of the image
	findContours(input, contoursInput, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//creates mat for the drawing of the contours
	filterContours(contoursInput, contoursOutput);
}

// Targeting Info //
// Finds Center point
Point2f centerPoint(Rect rect) {
	return Point2f(rect.x + (rect.width / 2), rect.y + (rect.height / 2));
}
// Finds offset from center point
double findOffset(double position, double resolution) {
	return (position - (resolution / 2)) / (resolution / 2);
}
// Finds x and y offsets
Point2f findXYOffset(Point2f point, Size res) {
	return Point2f(findOffset(point.x, (double)res.width), findOffset(point.y, (double)res.height));
}
// Shows crosshairs
void showCrosshairs(Mat &input){
	line(input, Point(input.cols / 2, 0), Point(input.cols / 2, input.rows), WHITE, 1);
	line(input, Point(0, input.rows / 2), Point(input.cols, input.rows / 2), WHITE, 1);
}

// Finds targets
void findTargets(Mat &imageInput, vector<vector<Point> > &input, Mat &output, double &avgDistance, double &angle){	
	// Readys Output mat for contour use
	output = Mat::zeros(imageInput.size(), CV_8UC3);
	// Creates Vector of Bounding Boxes
	vector<Rect> boundingBoxes(input.size());
	
	// Finds all the targets
	for (int i = 0; i< input.size(); i++) {
		// Adds rectangles
		Rect contourBounding = boundingRect(input[i]);
		// Stores rectangles into bounding box vector
		boundingBoxes[i] = contourBounding;
		//Draws contours onto output
		drawContours(output, input, i, SKY_BLUE, 2);
	}
	
	// If there are more than 2 targets found
	if (boundingBoxes.size() >= 2) {
		// Sort rectangles in descending order based on their area.
		sort(boundingBoxes.begin(), boundingBoxes.end(), boundingBoxSortDescending); 

		// Pick out two largest targets - these will most likely be what we're looking for.
		Rect target1 = boundingBoxes[0];
		Rect target2 = boundingBoxes[1];
		
		//Draws two rectanges onto output
		rectangle(output, target1, GREEN, 2);
		rectangle(output, target2, GREEN, 2);
		
		// Calculate center point of both targets
		Point2f centerPoint1 = centerPoint(target1);	
		Point2f centerPoint2 = centerPoint(target2);
		
		// Calculate mid points between centers of boundingBoxes
		Point2f midPoint((centerPoint1.x + centerPoint2.x) / 2, (centerPoint1.y + centerPoint2.y) / 2); 
		Point2f tl(midPoint.x - 2, midPoint.y - 2);	// Calculate top-left point of center dot
		Point2f tr(midPoint.x + 2, midPoint.y + 2); // Calculate bottom right point of center dot
		
		// Draw a line between center point of vision targets
		line(output, centerPoint1, centerPoint2, RED, 1); 
		// Draw a point at center of line drawn above
		rectangle(output, tl, tr, BLUE, 3);
		
		// Calculate the normalized mid point
		Point2f midPointNormal = findXYOffset(midPoint, output.size());
		double target1Area = target1.area();
		double target2Area = target2.area();
		double target1Distance = (OBJECT_AREA * FOCAL_LENGTH)/target1Area;
		double target2Distance = (OBJECT_AREA * FOCAL_LENGTH)/target2Area;
		avgDistance = (target1Distance+target2Distance)/2;
		angle = midPointNormal.x;
		
		// Put text on image (used for debugging)
		putText(output, "Final Target Data (Blue Dot):", Point2f(15, 1*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Angle: "+ to_string(angle), Point2f(30,2*15), cv::FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Avg Distance: "+ to_string(avgDistance), Point2f(30,3*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		
		putText(output, "Debug Data:", Point2f(15, 5*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Distance Target 1: "+ to_string(target1Distance), Point2f(30,6*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Distance Target 2: "+ to_string(target2Distance), Point2f(30,7*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Area Target 1: "+ to_string(target1Area), Point2f(30, 8*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Area Target 2: "+ to_string(target2Area), Point2f(30, 9*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
	}
}
// Does the image processing
void processImage(Mat& input, Mat& output, double &distance, double &angle){
	// Mats for processed outputs
	Mat hslOutput, maskOutput, contoursImageOutput, targetsOutput;
	// Create a HSL Mask
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	
	// Vector for storing contour values
	vector<vector<Point> > contoursValueOutput;
	// Create Contours
	createContours(maskOutput, contoursValueOutput);
	
	// Finds targets and passes back distance and angle
	findTargets(maskOutput, contoursValueOutput, targetsOutput, distance, angle);
	output = targetsOutput;
	
	// Draw crosshairs on image
	showCrosshairs(output);
}

// Returns true to quit when "ESC" is pressed
bool quit(){
	char key = (char)waitKey(5);
	return( key == 27 || key == 'q' || key == 'Q' ); // key 27 = 'ESC'
}

int main( int argc, char *argv[] ) {
	//sets up networktables
	auto nt = NetworkTable::GetTable("NURDVision");
		
	nt->SetIPAddress("10.28.39.200\n");
		
	nt->Initialize();
	
	// Initalizes distance and angle to 0.0;
	double distance = 0.0;
	double angle = 0.0;
	
	// Creates mats for storing image
	Mat raw, processed;
	// Starts video capture of camera 0;
	VideoCapture capture(0);
	
	cout << "Viewer opened" << endl
		 << "Press ESC or Q to terminate" << endl;		
		
	// While the quit fucntion does not return true run image functions
	while (!quit()) {
		// Stores capture to raw mat
		capture.read(raw);
		// Runs image processing - pass mats raw, returns and stores mat processed, doubles distance and angle
		processImage(raw, processed, distance, angle);
		//Outputs data to network tables
		nt->PutNumber("Distance", distance);
		nt->PutNumber("Angle", angle);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		// Display processed image
		if(argc > 1)if(string(argv[1]) == "-debug"){
			imshow("Processed image", processed);
			cout << "Distance: "<< distance << "    Angle: " << angle << endl;
		}
	}
	
	cout << "Viewer closed successfully";
	return 0;
}