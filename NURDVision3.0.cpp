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

//Store a double array for both lower and upper boundaries of the hsl filter, decides what color you're looking for in the first mask
// ========= Constants for Tape tracking ============//
// Store an array: [0] = lower bound, [1] = upper bound
const double Hue[] = {0, 49};
const double Saturation[] = {0, 70};
const double Luminance[] = {245, 255};
const int KNOWN_AREA = 5000; //At 10 inches
// =================================================//
double solution;
bool lockAcquired = false;

struct {
	bool operator() (cv::Rect a, cv::Rect b) {
		return b.area() < a.area();
	}
} rectSortDesc;

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
		Rect bb = boundingRect(contour);
		if (bb.width < 6 || bb.width > 1000000) continue;
		if (bb.height < 6 || bb.height > 1000000) continue;
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
	vector<Vec4i> hierarchy;
	//converts our hsl filter into black and white, because findContours requires you to be using black and white, not RGB or HSL	
	cvtColor(input, input, CV_RGB2GRAY);
	//finds the contours of the image
	findContours(input, contoursInput, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
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
	line(input, Point(input.cols / 2, 0), Point(input.cols / 2, input.rows), (lockAcquired ? Scalar(0, 255, 0) : Scalar(0, 0, 255)), 1);
	line(input, Point(0, input.rows / 2), Point(input.cols, input.rows / 2), (lockAcquired ? Scalar(0, 255, 0) : Scalar(0, 0, 255)), 1);
}

// Finds targets
void findTargets(Mat &imageInput, vector<vector<Point> > &input, Mat &output){	
	output = Mat::zeros(imageInput.size(), CV_8UC3);
	// Creates Bounding Boxes //
	// Bounding rectangles around contours
	vector<Rect> rects(input.size());
	// Rotated bounding rectangles around contours
	vector<RotatedRect> rotRects(input.size());
	for (int i = 0; i< input.size(); i++) {
		// Adds rectangles
		Rect conRect = boundingRect(input[i]);
		rects[i] = conRect;
		rotRects[i] = minAreaRect(input[i]);
		
		Scalar contourColor = Scalar(250, 206, 135);
		Scalar rectangleColor = Scalar(255, 255, 0);
		drawContours(output, input, i, contourColor, 2);
		rectangle(output, conRect, rectangleColor, 2);
		
		// Creates aimPoints
		Point2f offset = findXYOffset(centerPoint(conRect), output.size());
		
		// Changes lockAcquired if the target is locked on
		(-0.1 < offset.x < 0.1 && -0.1 < offset.y < 0.1) ? lockAcquired = true : lockAcquired = false; //TODO: Do better target aquition
		
		Scalar textColor = Scalar(255, 255, 255);
		
		double area = conRect.area();
		double distance = (area/KNOWN_AREA)*10;
		putText(output, "Angle X: "+ to_string(offset.x), centerPoint(conRect), FONT_HERSHEY_PLAIN, 1, textColor, 1);
		putText(output, "Angle Y: "+ to_string(offset.y), centerPoint(conRect) + Point2f(0, 15), FONT_HERSHEY_PLAIN, 1, textColor, 1);
		putText(output, "Distance: " + to_string(distance), centerPoint(conRect) + Point2f(0, 30), FONT_HERSHEY_PLAIN, 1, textColor, 1);	
	}
	if (rects.size() >= 2) {
		sort(rects.begin(), rects.end(), rectSortDesc); // Sort rectangles in descending order based on their area.

		// Pick out two largest targets - these will most likely be what we're looking for (Note to reader: I hate assumptions)
		cv::Rect targ1 = rects[0];
		cv::Rect targ2 = rects[1];

		// TODO: Calculate rectangle to encapsulate entire target...

		cv::Point2f cPoint1 = centerPoint(targ1);	// Calculate center point of both targets...
		cv::Point2f cPoint2 = centerPoint(targ2);

		cv::Point2f midPoint((cPoint1.x + cPoint2.x) / 2, (cPoint1.y + cPoint2.y) / 2); // Calculate mid points between centers of rects
		cv::Point2f tl(midPoint.x - 2, midPoint.y - 2);	// Calculate top-left point of center dot
		cv::Point2f tr(midPoint.x + 2, midPoint.y + 2); // Calculate bottom right point of center dot

		double t1Width = targ1.width;
		double t1Height = targ1.height;

		double t2Width = targ2.width;
		double t2Height = targ2.height;

		cv::line(output, cPoint1, cPoint2, cv::Scalar(0, 0, 255), 1); // Draw a line between center point of vision targets
		cv::rectangle(output, tl, tr, cv::Scalar(255, 0, 0), 3); // Draw a point at center of line drawn above

		cv::Point2f midPointNormal = findXYOffset(midPoint, output.size()); // Calculate the normalized mid point
		cv::putText(output, "Mid: "+ to_string(midPointNormal.x) + ", " + to_string(midPointNormal.y), midPoint, cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(255, 255, 255), 1);

		solution = midPointNormal.x; // Calculate a solution for turning the robot. This value will ultimately be plugged into a PID loop with inputs of -1 to 1 controlling the rotation rate of the robot.
	}
}
// Does the image processing
void processImage(Mat& input, Mat& output){
	Mat hslOutput, maskOutput, contoursImageOutput, targetsOutput;
	vector<vector<Point> > contoursValueOutput;
	
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	createContours(maskOutput, contoursValueOutput);
	findTargets(maskOutput, contoursValueOutput, targetsOutput);
	output = targetsOutput;
	showCrosshairs(output);
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