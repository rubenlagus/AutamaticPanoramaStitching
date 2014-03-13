#pragma once

//#include "FeaturesService.h"
#include "Panorama.h"
#include <iostream>
#include <cstdlib>
#define AUTOMATIC 1

using namespace std;
using namespace cv;


int main()
{
	FeaturesService featuresService = FeaturesService();
	string imagename = "0";
	vector<string> imagenames;
	vector<Mat> images, imagesnocolor;
	vector<vector<Matches>> imagesmatches;
	vector<vector<int>> relatedImages;
	vector<set<int>> panoramas;
	vector<vector<Matches>> panoramasmatches;

	unsigned int numImages;
	while (imagename.compare("") != 0 && imagename.compare("-1") != 0) {
			cout << "Insert image path to load (insert -1 to start loading predetermined images or just ENTER to finish the list of images): ";
			cin >> imagename;
			imagenames.push_back(imagename);
	}
	if (imagename.compare("-1") == 0) {
		imagenames.push_back("./yosemite2.jpg");
		imagenames.push_back("./yosemite1.jpg");
		imagenames.push_back("./b.JPG");
		imagenames.push_back("./yosemite3.jpg");
		imagenames.push_back("./a.JPG");			 
		imagenames.push_back("./yosemite4.jpg");
		imagenames.push_back("./yosemite5.jpg");
		imagenames.push_back("./raufusskauz.JPG");
		imagenames.push_back("./yosemite6.jpg");
		imagenames.push_back("./yosemite7.jpg");
		imagenames.push_back("./c.JPG");
		

	}

	cout << "Loading images" << endl;
	imagenames = featuresService.loadImagesArray(imagenames, images, imagesnocolor);
	numImages = imagenames.size();
	cout << "Computing homographies" << endl;
	featuresService.computeMatchesAndHomography(imagesnocolor, imagesmatches);
	featuresService.computeRelatedImages(imagesmatches, relatedImages);
	featuresService.computePanoramas(relatedImages, panoramasmatches, imagesmatches);
	cout << "Related images" << endl;
	
	vector<Panorama> panoramasfinales;
	int lastadded;
	cout << "Creating panoramas" << endl;
	for (int itout = 0; itout < panoramasmatches.size(); itout++) {
		Panorama newpanorama;
		newpanorama.addImage(panoramasmatches[itout][0].getImage2(), images[panoramasmatches[itout][0].getImage2().getImgNumber()]);
		lastadded = panoramasmatches[itout][0].getImage2().getImgNumber();
		panoramasmatches[itout].erase(panoramasmatches[itout].begin());
		int size = panoramasmatches[itout].size();
		for (int i = 0; i < size; i++) {
			for (int itin = 0; itin < panoramasmatches[itout].size(); itin++) {
				if (panoramasmatches[itout][itin].getImage1().getImgNumber() == lastadded) {
					newpanorama.addImage(panoramasmatches[itout][itin].getImage2(), images[panoramasmatches[itout][itin].getImage2().getImgNumber()]);					
					lastadded = panoramasmatches[itout][itin].getImage2().getImgNumber();
					break;
				}
			}
		}
		Mat destiny;
		bilateralFilter(newpanorama.getPanorama(), destiny, 5, 20,20);
		newpanorama.setPanorama(destiny);
		panoramasfinales.insert(panoramasfinales.end(), newpanorama);
	}

	cout << "Showing panoramas" << endl;
	for (unsigned int i = 0; i < panoramasfinales.size(); i++) {
		string nombre = "Resultado";
		nombre.append(to_string(i));
		imshow(nombre, panoramasfinales[i].getPanorama());
	}

	waitKey(0);
	system("pause");
}