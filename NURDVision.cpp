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

Mat camera, mask, processed;
VideoCapture capture(0);

// ========= Constants for Ball tracking ============//
//Store an array: [0] = lower bound, [1] = upper bound
double Hue[] = {29.138490103768287, 58.13721981270055};
double Saturation[] = {64.20863309352518, 255.0};
double Luminance[] = {75.67446043165468, 255.0};
// =================================================//

void hslThreshold(Mat &input, double hue[], double sat[], double lum[], Mat &out) {
		cvtColor(input, out, COLOR_BGR2HLS);
		inRange(out, Scalar(hue[0], lum[0], sat[0]), Scalar(hue[1], lum[1], sat[1]), out);
	}
	
void createMask(Mat &input, Mat &mask, Mat &output) {
	mask.convertTo(mask, CV_8UC1);
	bitwise_xor(output, output, output);
	input.copyTo(output, mask);
}

// Does the image processing
void processImage(Mat& input, Mat& output){
	hslThreshold(input, Hue, Saturation, Luminance, mask);
	createMask(input, mask, output);
}

// returns true to quit when "ESC" is pressed
bool quit(){
	int k = waitKey(5);
	return (k == 27);
}

int main() {
		cout << "Start grabbing" << endl
				<< "Press ESC to terminate" << endl;
				
		while (!quit()) {
			capture.read(camera);
			processImage(camera, processed);
			// show output
//			imshow("input", input);
			imshow("Processed image", processed);
		}
		cout << "Viewer closed";
	
	return 0;
}