// ===========================================================
// NURDVision - FRC Team 3255 SuperNURD 2017 Vision Processing
// written by Mike and Tayler
// ===========================================================

#include <iostream>
#include <opencv2/opencv.hpp>
#include <networktables/NetworkTable.h>
#include <ntcore.h>
#include <cscore.h>

//'namespaces' are used basically before each command, so 'createMask' is actually cv::createMask. This removes need to do so.
using namespace std;
using namespace cv;
using namespace cs;

// FIRST Robotics Competition team number
const int teamNumber = 3255;
// Camera input (default is 0 for jetson use)
const int camerainput = 0;
// Store a double array for both lower and upper boundaries of the hsl filter, decides what color you're looking for in the first mask
// ========= Constants for Tape tracking ============//
// Store an array: [0] = lower bound, [1] = upper bound
//const double Hue[] = {0, 49};
//const double Saturation[] = {150, 255};
//const double Luminance[] = {245, 255};

//const double OBJECT_AREA = 2.5; //Area of the tracking tape in real life
//const double PIXEL_AREA = 30000; //Area of the pixels at 10 inches
//const double BASE_DISTANCE = 10; //Distance from camera to use as base for calucation
// =================================================//

// ========= Constants for SykoraBot ============//
// Store an array: [0] = lower bound, [1] = upper bound
//My laptop values
//HSL VALUES FOR CAMERA WITH EXPOSURE SET AT 20; PASTE THIS COMMAND IN TO CHANGE EXPOSURE 'v4l2-ctl -c exposure_auto=(camera to change exposure of, most of the time 0 or 1) -c exposure_absolute=20' 
const double Hue[] = {63, 113};
const double Saturation[] = {236, 255};
const double Luminance[] = {40, 80};
//
//
const double OBJECT_AREA = 18.5; //Area of the tracking tape in real life
const double PIXEL_AREA = 8190; //Area of the pixels at base distance
const double BASE_DISTANCE = 26; //Distance from camera to use as base for calucation
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
void findTargets(Mat &imageInput, vector<vector<Point> > &input, Mat &output, double &avgDistance, double &angle, double &offset){	
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
		
		// Distance and angle calculation
		Point2f midPointNormal = findXYOffset(midPoint, output.size());
		double target1Width = target1.width;
		double target2Width = target2.width;
		double target1Height = target1.height;
		double target2Height = target2.height;
		avgDistance = 1000/((target1Width+target2Width)/2);
		offset = -midPointNormal.x;

		if (centerPoint1.x > midPoint.x){
			angle = target1Height - target2Height;	
	    }
		else{
			angle = target2Height - target1Height;
		}
		// Put text on image (used for debugging)
		putText(output, "Final Target Data (Blue Dot):", Point2f(15, 1*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Angle: "+ to_string(angle), Point2f(30,2*15), cv::FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		putText(output, "Avg Distance: "+ to_string(avgDistance), Point2f(30,3*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
		
		putText(output, "Debug Data:", Point2f(15, 5*15), FONT_HERSHEY_PLAIN, 0.8, WHITE, 1);
	}
}

// Does the image processing
void processImage(Mat& input, Mat& output,Mat& hslOutput , double &distance, double &angle, double &offset){
	// Mats for processed outputs
	Mat maskOutput, contoursImageOutput, targetsOutput;
	// Create a HSL Mask
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	
	// Vector for storing contour values
	vector<vector<Point> > contoursValueOutput;
	// Create Contours
	createContours(maskOutput, contoursValueOutput);
	
	// Finds targets and passes back distance and angle
	findTargets(maskOutput, contoursValueOutput, targetsOutput, distance, angle, offset);
	output = targetsOutput;
	
	// Draw crosshairs on image
	showCrosshairs(output);
}

//Initalize Network Tables to team 3255 and returns table to use
shared_ptr<NetworkTable> InitalizeNetworkTables(int teamNumber) {
	NetworkTable::SetClientMode();
	NetworkTable::SetTeam(teamNumber);
	NetworkTable::Initialize();
	return NetworkTable::GetTable("NURDVision");
}

//Publish Network Tables to table in use
void PublishNetworkTables(shared_ptr<NetworkTable> table, double distance,double angle, double offset) {
	table->PutNumber("Distance", distance);
	table->PutNumber("Angle", angle);
	table->PutNumber("offset x", offset);
}

// Returns true to quit when "ESC" is pressed
bool quit(){
	char key = (char)waitKey(5);
	return( key == 27 || key == 'q' || key == 'Q' ); // key 27 = 'ESC'
}

int main(int argc, char *argv[]) {
	cout << "\e[1mRunning \e[34mNURD\e[31mVision\e[0m \tFRCTeam 3255 SuperNURD 2017 Vision Processing\n" << 
	"\t\t\tCreated by Mike Smith & Tayler Uva\n\n";
					
	// Checks run arguments
	bool local=false;
	bool debug=false;
	if(argc >= 2){
		if(string(argv[1]) == "-local") local=true;
		if(string(argv[1]) == "-debug") debug=true;
	}
	if(argc >= 3){
		if(string(argv[2]) == "-local") local=true;
		if(string(argv[2]) == "-debug") debug=true;
	}
	cout << "Debug: " << (debug ? "\e[32mtrue\e[0m" : "\e[31mfalse\e[0m") << endl;
	cout << "localhost: " << (local ? "\e[32mtrue\e[0m" : "\e[31mfalse\e[0m") << endl;
	cout << "Team Number: " << "\e[34m" << teamNumber << "\e[0m" << endl;
	cout << endl;
				
	// Initalizes distance and angle to 0.0;
	double distance = 0.0;
	double angle = 0.0;
	double offset = 0.0;

	CvSource cvSource = CvSource("src", cs::VideoMode::PixelFormat::kMJPEG, 640, 480, 15);
	MjpegServer cvMjpgServer = MjpegServer("server", 1180);
	cvMjpgServer.SetSource(cvSource);
	
	//Initalizes Networktables
	shared_ptr<NetworkTable> ntable = InitalizeNetworkTables(teamNumber);
	// If run argument -local is present, set ip address to localhost
	if(local) NetworkTable::SetIPAddress("localhost");

	// Creates mats for storing image
	Mat raw, processed, hslOutput;
	// Starts video capture of camera 0;
	VideoCapture capture(camerainput);
	if (!capture.isOpened()) {
		cerr << "\n\e[31mERROR! Unable to open camera\nERROR! Is the camera connected?\e[0m\n";
		NetworkTable::Shutdown();
		return 1;
	}
	
	cout << "\nSTARTING IMAGE PROCESSING\n" << endl;
	
	if(debug){
	cout << "VIEWER OPENED" << endl
		 << "Press ESC or Q to terminate\n" << endl;		
	}	

	cout << "MJpeg stream available at port " << (1180) << endl;

	// While the quit fucntion does not return true run image functions
	while (!quit()) {
		// Stores capture to raw mat
		capture.read(raw);
		// Runs image processing - pass mats raw, returns and stores mat processed, doubles distance and angle
		processImage(raw, processed, hslOutput, distance, angle, offset);
		// Publisheds Data to NetworkTable - Vision
		PublishNetworkTables(ntable, distance, angle, offset);
		// Runs if debug is true
		
		cvSource.PutFrame(processed);

		if(debug){
			// Display processed image
			imshow("Processed image", processed);
			imshow("Raw Image", raw);
			imshow("HSL Image", hslOutput);
			// Output data to console
			cout << "Distance: "<< distance << "\tAngle: " << angle << "\tOffset: " << offset << endl;
			
		}
	}
	NetworkTable::Shutdown();
	cout << "\nNURDVision stopped successfully\n";
	return 0;
}
