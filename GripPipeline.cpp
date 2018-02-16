#include "GripPipeline.h"

namespace grip {

GripPipeline::GripPipeline() {
}
/**
* Runs an iteration of the pipeline and updates outputs.
*/
void GripPipeline::Process(cv::Mat& source0, cv::CascadeClassifier& source1){
	//Step New_Size0:
	//input
	double newSizeWidth = 100.0;  // default Double
	double newSizeHeight = 10.0;  // default Double
	newSize(newSizeWidth, newSizeHeight, this->newSizeOutput);
	//Step Cascade_Classifier0:
	//input
	cv::Mat cascadeClassifierImage = source0;
	cv::CascadeClassifier cascadeClassifierClassifier = source1;
	double cascadeClassifierScaleFactor = 1.01;  // default Double
	double cascadeClassifierMinNeighbors = 1.0;  // default Double
	cv::Size cascadeClassifierMinSize = newSizeOutput;
	cv::Size cascadeClassifierMaxSize(0, 0);
	cascadeClassifier(cascadeClassifierImage, cascadeClassifierClassifier, cascadeClassifierScaleFactor, cascadeClassifierMinNeighbors, cascadeClassifierMinSize, cascadeClassifierMaxSize, this->cascadeClassifierOutput);
}

/**
 * This method is a generated getter for the output of a New_Size.
 * @return Size output from New_Size.
 */
cv::Size* GripPipeline::GetNewSizeOutput(){
	return &(this->newSizeOutput);
}
/**
 * This method is a generated getter for the output of a Cascade_Classifier.
 * @return RectsReport output from Cascade_Classifier.
 */
std::vector<cv::Rect>* GripPipeline::GetCascadeClassifierOutput(){
	return &(this->cascadeClassifierOutput);
}
	/**
	 * Fills a size with given width and height.
	 * @param width the width of the size.
	 * @param height the height of the size.
	 * @param size the size to fill.
	 */
	void GripPipeline::newSize(double width, double height, cv::Size &size) {
		size.height = height;
		size.width = width;
	}

	/**
	* Runs a cascade classifier on an image.
	*
	* @param input          the image to run the classifier on
	* @param classifier     the classifier to use
    * @param scaleFactor    the scale factor of each successive downsized image
    * @param minNeighbors   how many neighbors each candidate rectangle should have to retain it
    * @param minSize        the minimum possible object size
    * @param maxSize        the maximum possible object size. If (0, 0), it is assumed to be unbounded
    * @param detections     the vector of Rects to store the detected regions in
	*/
	void GripPipeline::cascadeClassifier(cv::Mat &input, cv::CascadeClassifier &classifier, double scaleFactor, double minNeighbors, cv::Size &minSize, cv::Size &maxSize, std::vector<cv::Rect> &detections) {
		classifier.detectMultiScale(input, detections, scaleFactor, minNeighbors, 0, minSize, maxSize);
	}


} // end grip namespace

