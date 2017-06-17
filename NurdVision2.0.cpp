/*
NURDVISION 2.0
written by Mike with help from Taylor
*/
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

//Store a double array for either upper or lower boundaries of the hsl filter, basically what color youre looking for in the first mask
double Hue[] = { 79, 91 };
double Saturation[] = { 170, 255 };
double Luminance[] = { 46, 255 };
RNG rng(12345);

//is a thing used in processImage Converts image to hsl filter, basically filtering out any color besides retroreflective tapes
void hslThreshold(Mat &input, double hue[], double sat[], double lum[], Mat &output) {
	cvtColor(input, output, COLOR_BGR2HLS);
	inRange(output, Scalar(hue[0], lum[0], sat[0]), Scalar(hue[1], lum[1], sat[1]), output);
}

//is a thing used in processImage creates a workable mask to a mat, so we can process the hsl filter
void createMask(Mat &input, Mat &mask, Mat &output) {
	mask.convertTo(mask, CV_8UC1);
	bitwise_xor(output, output, output);
	input.copyTo(output, mask);
}

//is a command to process the original to hsl, using input and output
void hslImage(Mat& input, Mat& output) {
	Mat hslOutput, maskOutput;
	hslThreshold(input, Hue, Saturation, Luminance, hslOutput);
	createMask(input, hslOutput, maskOutput);
	output = maskOutput;
}

//basically checks if esc is pressed, used in main to loop video as a loop to keep looping over each frame
bool quit() {
	char key = (char)waitKey(5);
	return(key == 27 || key == 'q' || key == 'Q'); // 'ESC'
}

//main function
int main() {
	//creates 'mat's to store images
	Mat raw, hsl, bwImage;
	//makes vectors to store contour information
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//inports camera feed
	VideoCapture capture(0);
	//while loop to keep the process going
	while (!quit()) {
		//reads camera footage
		capture.read(raw);
		//processes raw footage into hsl filter
		hslImage(raw, hsl);
		//converts our hsl filter into black and white, because findContours requires you to be using black and white, not RGB or HSL
		
		cvtColor(hsl, bwImage, CV_RGB2GRAY);
		//finds the contours of the image
		
		findContours(bwImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		
		//creates mat for the drawing of the contours
		Mat drawing = Mat::zeros(hsl.size(), CV_8UC3);

		for (int i = 0; i< contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}

		//shows the drawn image
		imshow("image", drawing);
	}
	
	//i dont know why i have this here to be honest
	return 0;
}
