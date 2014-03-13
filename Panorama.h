#include "FeaturesService.h"

using namespace cv;

class Panorama
{
private:
	Mat panorama;
	vector<KeyPoint> keypoints;
	Mat descriptors;
	vector<int> imagesnumbers;
	bool firstimageadded;
public:
	// Constructors
	Panorama();

	// Destructors
	~Panorama();

	// Getters
	Mat getPanorama();
	vector<KeyPoint> getKeypoints();
	Mat getDescriptors();
	vector<int> getImagesNumbers();

	// Setters
	void setPanorama(Mat newpanorama);
	void setKeypoints(vector<KeyPoint> newkeypoints);
	void setDescriptors(Mat newdescriptors);

	// Methods
	void addImage(Image imagedata, Mat imagetoadd);
};

