#include "FeaturesService.h"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "iostream"

using namespace cv;
using namespace std;
using namespace cv::detail;

// Constructors
FeaturesService::FeaturesService() :
NUMBERFEATUES(500),
NEARESTNEIGHBOURSNUMBER(2),
GOODMATCHTHRESHOLD(0.6),
RANSACTHRESHOLD(4.0)
{
	detector = SiftFeatureDetector(NUMBERFEATUES);
	extractor = SiftDescriptorExtractor();
	matcher_Knn = FlannBasedMatcher();
}

// Destructor
FeaturesService::~FeaturesService() {
}

// Private Methods
void FeaturesService::addtocurrentpanorama(Matches matche, vector<Matches> &results) {
	int imgtosearch = matche.getImage2().getImgNumber();
	int imgorigin = matche.getImage1().getImgNumber();
	bool checked = false;
	bool isadded = false;
	for (int it = 0; !checked && it < results.size(); it++) {
		if (results[it].getImage2().getImgNumber() == imgtosearch) {
			if (matche.getInliers().size() > results[it].getInliers().size()) {
				for (int i = 0; i < results.size(); i++){
					if (results[it].getImage1().getImgNumber() == imgorigin && results[it].getImage1().getImgNumber() != results[it].getImage2().getImgNumber()){
						results.erase(results.begin() + it);
					}
				}
				results.push_back(matche);
				checked = true;
				isadded = true;
				break;
			}
			else{
				if (imgorigin != imgtosearch) {
					isadded = true;
					break;
				}
			}
		}
		else {
			if (results[it].getImage1().getImgNumber() == imgorigin && results[it].getImage1().getImgNumber() != results[it].getImage2().getImgNumber()){
				if (matche.getInliers().size() > results[it].getInliers().size()) {
					results.erase(results.begin() + it);
					results.push_back(matche);
					checked = true;
					isadded = true;
					break;
				}
				else
					checked = true;
			} 
		}
	}

	if (!isadded && !checked)
		results.push_back(matche);

}

int FeaturesService::findpanoramatoadd(vector<set<int>> panoramaslist, int imgtoadd, vector<int> imgsrelated) {
	for (int imagen = 0; imagen < imgsrelated.size(); imagen++) {
		for (int i = 0; i < panoramaslist.size(); i++) {
			for (set<int>::iterator it = panoramaslist[i].begin(); it != panoramaslist[i].end(); it++){
				if ((*it) == imgsrelated[imagen])
					return i;
			}
		}
	}
	return -1;
}

// Methods
void FeaturesService::findKeyPoints(Mat image, vector<KeyPoint> &keypoints) {
	detector.detect(image, keypoints);
}

void FeaturesService::findDescriptors(Mat image, vector<KeyPoint> keypoints, Mat &descriptors) {
	extractor.compute(image, keypoints, descriptors);
}

void FeaturesService::knn_matcher(Mat descriptorsimg1, Mat descriptorsimg2, vector<DMatch> &goodMatches) {
	matcher_Knn.knnMatch(descriptorsimg1, descriptorsimg2, matches, NEARESTNEIGHBOURSNUMBER);
	goodMatches.clear();
	goodMatches.reserve(matches.size());
	for (unsigned int i = 0; i < matches.size(); i++) {
		if (matches[i].size() < 2)
			continue;
		DMatch m1 = matches[i][0];
		DMatch m2 = matches[i][1];
		if (m1.distance <= GOODMATCHTHRESHOLD * m2.distance)
			goodMatches.push_back(m1);
	}
}

void FeaturesService::findHomographyFromMatches(vector<KeyPoint> keypointsimg1, vector<KeyPoint> keypointsimg2, vector<DMatch> matches, Mat &H, vector<unsigned char> &mask){
	vector<Point2f> img1, img2;
	for (unsigned int i = 0; i < matches.size(); i++) {
		img1.push_back(keypointsimg1[matches[i].queryIdx].pt);
		img2.push_back(keypointsimg2[matches[i].trainIdx].pt);
	}
		if (img1.size() > 4)
			H = findHomography(img2, img1, CV_RANSAC, RANSACTHRESHOLD, mask);
}

vector<string> FeaturesService::loadImagesArray(vector<string> imagesnames, vector<Mat> &imagescolor, vector<Mat> &imagesnocolor) {
	vector<string> namesvector;
	for (unsigned int i = 0; i < imagesnames.size(); i++) {
		if (imagesnames[i] == "")
			continue;
		Mat newimage = imread(imagesnames[i], CV_LOAD_IMAGE_COLOR);
		if (newimage.data == 0)
			continue;
		Mat newimagenocolor;
		cvtColor(newimage, newimagenocolor, CV_BGR2GRAY);
		if (newimagenocolor.data == 0)
			continue;
		namesvector.push_back(imagesnames[i]);
		imagescolor.push_back(newimage);
		imagesnocolor.push_back(newimagenocolor);
	}
	return namesvector;
}

void FeaturesService::computeMatchesAndHomography(vector<Mat> images, vector<vector<Matches>> &imagesmatches) {
	for (unsigned int i = 0; i < images.size(); i++) {
		vector<Matches> vecmatches;
		for (unsigned int j = 0; j < images.size(); j++) {
			cout << "Starting new relation (" << i << ", " << j << ") ----> ";
			vector<KeyPoint> keypointsimg1, keypointsimg2;
			vector<DMatch> matchesvec;
			vector<unsigned char> mask;
			Mat H, descriptorsimg1, descriptorsimg2;
			findKeyPoints(images[i], keypointsimg1);
			findKeyPoints(images[j], keypointsimg2);
			findDescriptors(images[i], keypointsimg1, descriptorsimg1);
			findDescriptors(images[j], keypointsimg2, descriptorsimg2);
			knn_matcher(descriptorsimg1, descriptorsimg2, matchesvec);
			findHomographyFromMatches(keypointsimg1, keypointsimg2, matchesvec, H, mask);
			Image img1 = Image(i, keypointsimg1, descriptorsimg1);
			Image img2 = Image(j, keypointsimg2, descriptorsimg2);
			Matches matche = Matches(img1, img2, matchesvec, H, mask);
			vecmatches.push_back(matche);
			cout << "Relation finished" << endl;
		}
		imagesmatches.push_back(vecmatches);
	}
}

void FeaturesService::computeRelatedImages(vector<vector<Matches>> imagesmatches, vector<vector<int>> &related) {
	related.clear();
	for (unsigned int i = 0; i < imagesmatches.size(); i++) {
		vector<int> auxvec;
		for (unsigned int j = 0; j < imagesmatches[i].size(); j++) {
			double aux = 8.0 + 0.3*imagesmatches[i][j].getNumberMatches();
			if (aux < imagesmatches[i][j].getInliers().size())
				auxvec.push_back(j);
		}
		related.push_back(auxvec);
	}
}

void FeaturesService::computePanoramas(vector<vector<int>> relatedImages , vector<vector<Matches>> &panoramasmatches, vector<vector<Matches>> &imagesmatches) {
	vector<set<int>> panoramas;
	cout << endl << "Searching panoramas" << endl;
	int currentpanorama = -1;

	for (int i = 0; i < relatedImages.size(); i++) {
		currentpanorama = findpanoramatoadd(panoramas, i, relatedImages[i]);

		if (currentpanorama == -1){
			set<int> newpan;
			vector<Matches> newvec;
			newpan.insert(i);
			newvec.push_back(imagesmatches[i][i]);
			panoramasmatches.push_back(newvec);
			panoramas.push_back(newpan);
			currentpanorama = panoramas.size() - 1;
		}
		for (int j = 0; j < relatedImages[i].size(); j++) {
			
			if (i != relatedImages[i][j]) {
				addtocurrentpanorama(imagesmatches[i][relatedImages[i][j]], panoramasmatches[currentpanorama]);
				panoramas[currentpanorama].insert(relatedImages[i][j]);
			}
		}
	}
}

void FeaturesService::computePanorama(Mat panorama, Mat images, Mat &newpanorama, Mat H) {
	
	Mat result;	
	warpPerspective(images, result, H, Size((images.cols + panorama.cols), (images.rows + panorama.rows)));
	
	Mat final(Size((panorama.cols) * 4, (panorama.rows) * 4), CV_8UC3);
	Mat roi1(final, Rect(0, 0, panorama.cols, panorama.rows));
	Mat roi2(final, Rect(0, 0, result.cols, result.rows));
	result.copyTo(roi2);
	Mat nocolorfinal;
	cvtColor(final, nocolorfinal, CV_BGR2GRAY);
	int firstoverlap = 0;
	bool firstoverlapfound = false;
	double overlapweight = 0;
	for (int i = 0; i < panorama.rows; i++) {
		overlapweight = 0;
		firstoverlapfound = false;
		for (int j = 0; j < panorama.cols; j++) {
			if (nocolorfinal.at<uchar>(i, j) <= 0) {
				roi1.at<Vec3b>(i, j) = panorama.at<Vec3b>(i, j);
			}
			else {
				if (!firstoverlapfound) {
					firstoverlapfound = true;
					firstoverlap = j;
				}
				else
					overlapweight += 1.0 / ((panorama.cols - 0) - firstoverlap);
				roi1.at<Vec3b>(i, j)[0] = roi1.at<Vec3b>(i, j)[0] * (overlapweight)+panorama.at<Vec3b>(i, j)[0] * (1 - overlapweight);
				roi1.at<Vec3b>(i, j)[1] = roi1.at<Vec3b>(i, j)[1] * (overlapweight)+panorama.at<Vec3b>(i, j)[1] * (1 - overlapweight);
				roi1.at<Vec3b>(i, j)[2] = roi1.at<Vec3b>(i, j)[2] * (overlapweight)+panorama.at<Vec3b>(i, j)[2] * (1 - overlapweight);
			}
		}
	}
	int cols = final.cols;
	int rows = final.rows;
	bool fin = false;
	unsigned int finrow = final.rows - 1;
	int sum = 0;
	for (int i = 0; sum < final.cols && i < final.rows; i++) {
		sum = 0;
		fin = false;
		for (int j = 0; !fin && j < final.cols; j++) {
			if (final.at<Vec3b>(i, j)[0] == 0 && final.at<Vec3b>(i, j)[1] == 0 && final.at<Vec3b>(i, j)[2] == 0) {
				sum++;
			}
			else{
				fin = true;
			}

		}
		if (sum == final.cols)
				finrow = i;
	}
	final = final.rowRange(0, finrow);
	fin = false;
	unsigned int fincol = final.cols - 1;

	sum = 0;
	for (int j = 0; sum < final.rows && j < final.cols; j++) {
		sum = 0;
		fin = false;
		for (int i = 0; !fin && i < final.rows; i++) {
			if (final.at<Vec3b>(i, j)[0] == 0 && final.at<Vec3b>(i, j)[1] == 0 && final.at<Vec3b>(i, j)[2] == 0) {
				sum++;
			}
			else
				fin = true;
		}
		if (sum == final.rows)
			fincol = j;
	}
	final.colRange(0, fincol).copyTo(newpanorama);
}
