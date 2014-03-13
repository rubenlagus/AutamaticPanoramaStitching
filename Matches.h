#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Image.h"

using namespace cv;


class Matches {
private:
	// Attributes
	Image img1, img2;
	vector<DMatch> goodMatches;
	int numberMatches;
	Mat Homography;
	vector<unsigned char> mask;
	vector<DMatch> inliers, outliers;

	// other methods
	void computeInliers();
public:
	// Constructors
	Matches();
	Matches(float thresh);
	Matches(const Matches &match);
	Matches(Image im1, Image im2, vector<DMatch> goodMat, Mat H, vector<unsigned char> m);

	// Destructor
	~Matches();

	// Operators
	Matches & operator=(const Matches & match);

	// Setters
	void setImage1(Image im1);
	void setImage2(Image im2);
	void setHomography(Mat homo);
	void setGoodMatches(vector<DMatch> gM);
	void setMask(vector<unsigned char> m);

	// Getters
	Image getImage1();
	Image getImage2();
	Mat getHomography();
	vector<DMatch> getGoodMatches();
	int getNumberMatches();
	vector<unsigned char> getMask();
	vector<DMatch> getInliers();
	vector<DMatch> getOutliers();
};

