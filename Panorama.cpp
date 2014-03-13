#include "Panorama.h"

// Constructor
Panorama::Panorama() {
	firstimageadded = false;
}

// Destructors
Panorama::~Panorama() {
}

// Getters
Mat Panorama::getPanorama() {
	return panorama;
}

vector<KeyPoint> Panorama::getKeypoints() {
	return keypoints;
}

Mat Panorama::getDescriptors() {
	return descriptors;
}

vector<int> Panorama::getImagesNumbers() {
	return imagesnumbers;
}

// Setters
void Panorama::setPanorama(Mat newpanorama) {
	newpanorama.copyTo(panorama);
}

void Panorama::setKeypoints(vector<KeyPoint> newkeypoints) {
	keypoints = newkeypoints;
}

void Panorama::setDescriptors(Mat newdescriptors) {
	descriptors = newdescriptors;
}

// Methods
void Panorama::addImage(Image imagedata, Mat imagetoadd) {
	bool alreadyadded = false;
	for (unsigned int i = 0; i < imagesnumbers.size(); i++) {
		if (imagedata.getImgNumber() == imagesnumbers[i])
			alreadyadded = true;
	}
	if (!alreadyadded){
		if (!firstimageadded) {
			setPanorama(imagetoadd);
			firstimageadded = true;
			imagesnumbers.push_back(imagedata.getImgNumber());
		}
		else {
			Mat nocolorpanorama, nocolorimage;
			cvtColor(imagetoadd, nocolorimage, CV_BGR2GRAY);
			cvtColor(panorama, nocolorpanorama, CV_BGR2GRAY);
			FeaturesService featuresService = FeaturesService();
			vector<KeyPoint> keypointsimg;
			vector<DMatch> matchesvec, matchesvec2;
			vector<unsigned char> mask;
			Mat H, descriptorsimg, H2;
			featuresService.findKeyPoints(nocolorpanorama, keypoints);
			keypointsimg = imagedata.getKeypoints();
			featuresService.findDescriptors(nocolorpanorama, keypoints, descriptors);
			descriptorsimg = imagedata.getDescriptors();
			featuresService.knn_matcher(descriptors, descriptorsimg, matchesvec);
			featuresService.findHomographyFromMatches(keypoints, keypointsimg, matchesvec, H, mask);

			Mat newpanorama;

			if (H.at<double>(0, 2) < 0.0) {

				featuresService.knn_matcher(descriptorsimg, descriptors, matchesvec2);
				featuresService.findHomographyFromMatches(keypointsimg, keypoints, matchesvec2, H2, mask);
				featuresService.computePanorama(imagetoadd, panorama, newpanorama, H2);
			}
			else {
				featuresService.computePanorama(panorama, imagetoadd, newpanorama, H);
			}

			imagesnumbers.push_back(imagedata.getImgNumber());
			setPanorama(newpanorama);
		}
	}
}
