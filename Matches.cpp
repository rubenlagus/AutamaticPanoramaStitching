#include "Matches.h"

// Constructors
Matches::Matches() {
}

Matches::Matches(float thresh) {
}

Matches::Matches(const Matches& match) :
img1(match.img1),
img2(match.img2),
goodMatches(match.goodMatches),
Homography(match.Homography),
numberMatches(match.goodMatches.size()),
mask(match.mask)
{
	computeInliers();
}

Matches::Matches(Image im1, Image im2, vector<DMatch> goodMat, Mat H, vector<unsigned char> m) {
	img1 = im1;
	img2 = im2;
	goodMatches = goodMat;
	Homography = H;
	numberMatches = goodMat.size();
	mask = m;
	computeInliers();
}

// Destructor
Matches::~Matches() {

}

// Other methods
void Matches::computeInliers() {
	inliers.clear();
	outliers.clear();

	for (unsigned int i = 0; i < mask.size(); i++) {
		if (mask[i] == 1)
			inliers.push_back(goodMatches[i]);
		else
			outliers.push_back(goodMatches[i]);
	}
}

// Operators
Matches & Matches::operator=(const Matches & match) {
	
	img1 = match.img1;
	img2 = match.img2;

	goodMatches = match.goodMatches;
	Homography = match.Homography;
	numberMatches = match.goodMatches.size();
	mask = match.mask;
	computeInliers();
	return *this;
}

// Setters
void Matches::setImage1(Image im1) {
	img1 = im1;
}

void Matches::setImage2(Image im2) {
	img2 = img2;
}

void Matches::setHomography(Mat homo) {
	Homography = homo;
}

void Matches::setMask(vector<unsigned char> m) {
	mask = m;
	computeInliers();
}

void Matches::setGoodMatches(vector<DMatch> gM) {
	goodMatches = gM;
	numberMatches = gM.size();
}

// Getters
Image Matches::getImage1() {
	return img1;
}

Image Matches::getImage2() {
	return img2;
}

Mat Matches::getHomography() {
	return Homography;
}

vector<DMatch> Matches::getGoodMatches() {
	return goodMatches;
}

int Matches::getNumberMatches() {
	return numberMatches;
}

vector<unsigned char> Matches::getMask() {
	return mask;
}

vector<DMatch> Matches::getInliers() {
	return inliers;
}

vector<DMatch> Matches::getOutliers() {
	return outliers;
}

