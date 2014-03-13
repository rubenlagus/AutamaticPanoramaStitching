#pragma once

#include "Matches.h"
#include <iostream>
#include <set>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class FeaturesService {
private:
	const int NUMBERFEATUES;
	const int NEARESTNEIGHBOURSNUMBER;
	const double GOODMATCHTHRESHOLD;
	const double RANSACTHRESHOLD;

	SiftFeatureDetector detector;
	SiftDescriptorExtractor extractor;
	FlannBasedMatcher matcher_Knn;
	vector<vector<DMatch> > matches;

	// Private methods
	void addtocurrentpanorama(Matches matche, vector<Matches> &results);
	int findpanoramatoadd(vector<set<int>> panoramaslist, int imgtoadd,vector<int> imgsrealted);
public:
	// Constructors
	FeaturesService();

	// Destructor
	~FeaturesService();

	// Methods

	void findKeyPoints(Mat image, vector<KeyPoint> &keypoints);

	void findDescriptors(Mat image, vector<KeyPoint> keypoints, Mat &descriptors);

	void knn_matcher(Mat descriptorsimg1, Mat descriptorsimg2, vector<DMatch> &goodMatches);

	void findHomographyFromMatches(vector<KeyPoint> keypointsimg1, vector<KeyPoint> keypointsimg2, vector<DMatch> matches, Mat &H, vector<unsigned char> &mask);

	vector<string> loadImagesArray(vector<string> imagesnames, vector<Mat> &imagescolor, vector<Mat> &imagesnocolor);

	void computeMatchesAndHomography(vector<Mat> images, vector<vector<Matches>> &imagesmatches);

	void computeRelatedImages(vector<vector<Matches>> imagesmatches, vector<vector<int>> &related);

	void computePanoramas(vector<vector<int>> relatedImages, vector<vector<Matches>> &panoramasmatches, vector<vector<Matches>> &imagesmatches);

	void computePanorama(Mat panorama, Mat images, Mat &newpanorama, Mat H);
};

