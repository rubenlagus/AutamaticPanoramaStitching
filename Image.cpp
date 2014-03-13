#include "Image.h"

// Constructors
Image::Image() {

}

Image::Image(const Image &img) :
imgnumber(img.imgnumber)
{
	keypoints = img.keypoints;
	descriptors = img.descriptors;
	numberKeyPoints = img.keypoints.size();
}

Image::Image(int number, vector<KeyPoint> kp, Mat des) {
	imgnumber = number;
	keypoints = kp;
	descriptors = des;
	numberKeyPoints = kp.size();
}

// Destructor
Image::~Image() {

}

// operators
Image& Image::operator=( const Image & img)
{
	imgnumber = img.imgnumber;
	keypoints = img.keypoints;
	img.descriptors.copyTo(descriptors);
	numberKeyPoints = img.numberKeyPoints;
	return *this;
}

// getters
int Image::getImgNumber() {
	return imgnumber;
}

vector<KeyPoint> Image::getKeypoints() {
	return keypoints;
}

Mat Image::getDescriptors() {
	return descriptors;
}

int Image::getNumberKeyPoints() {
	return numberKeyPoints;
}

// Setters
void Image::setImgNumber(int number) {
	imgnumber = number;
}

void Image::setKeypoints(vector<KeyPoint> kp) {
	keypoints = kp;
}

void Image::setDescriptors(Mat descrip) {
	descriptors = descrip;
}
