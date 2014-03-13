#pragma once

#include <opencv2/features2d/features2d.hpp>

using namespace cv;

/**
 * Data type to store every data that are specific of an image
 *
 */
class Image {
private:
	// Attributes
	int imgnumber; //< Image index in image vector
	vector<KeyPoint> keypoints; //< Current image's keypoints
	Mat descriptors; //< Current image's descriptors
	int numberKeyPoints; //< Number of Keypoints
public:
	// Constructors
	
	/**
	 * Default constructor
	 */
	Image();

	/**
	 * Copy constructor
	 */
	Image(const Image &img);

	/**
	 * Constructor with params
	 * @param number Image index
	 * @param kp Image's keypoints
	 * @param des Mat with the image's descriptors
	 */
	Image(int number, vector<KeyPoint> kp, Mat des);

	// Destructor
	
	/**
	 * Defualt destrucor
	 */
	~Image();

	// operators

	/**
	 * Overload of assignament operator
	 * @param img Image to be assigned
	 * @return An image with the same data as img
	 */
	Image & operator=(const Image & img);

	// getters

	/**
	 * Return the image index
	 */
	int getImgNumber();

	/**
	* Return the image keypoints
	*/
	vector<KeyPoint> getKeypoints();

	/**
	* Return the image descriptors
	*/
	Mat getDescriptors();

	/**
	* Return the image keypoints number
	*/
	int getNumberKeyPoints();

	// Setters

	/**
	* Setter for image index
	* @param number new image index
	*/
	void setImgNumber(int number);

	/**
	* Setter for image keypoints
	* @param number new image keypoints
	*/
	void setKeypoints(vector<KeyPoint> kp);

	/**
	* Setter for image descriptors
	* @param number new descriptors
	*/
	void setDescriptors(Mat descrip);
	
};

