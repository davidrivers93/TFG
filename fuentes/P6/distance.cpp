#include <time.h>
#include <iostream>
#include <CImg.h>

using namespace cimg_library;


int main(int argc, char **argv) 
{

	const int size = cimg_option("-s", 15, "Size of the erosion/dilation filter");
	const char * filename = cimg_option("-i", (char *) 0, "Input image name");
	cimg_usage("This program computes erosions and dilation using distance image");

	if(filename == 0) {
		std::cerr << "One argument needed. Option -i \n";
		exit(0);
	}
	CImg<unsigned char> img( filename );
	if(img.spectrum() > 1) {
		std::cerr << "Image must be grayscale\n";
		exit(0);
	}

	//Make sure it is binary
	img.threshold(128);


	//Pixels that are not == 1 are changed by the distance to the nearest pixel that is ==1
	//Pixels that are == 1 are set to 0
	CImg<float> distancedilate = img.get_distance(1);
	distancedilate.display("Distance dilate",false);
	CImg<unsigned char> dilat =  (1 - distancedilate.threshold(size)) ;

	//Pixels that are  == 1 are changed by the distance to the nearest pixel that is ==0
	//Pixels that are == 0 are set to 0
	CImg<float> distanceerode = img.get_distance(0);
	distanceerode.display("Distance erode",false);
	CImg<unsigned char> erode =  distanceerode.threshold(size) ;

	img.get_append(dilat,'x').get_append(erode,'x').display("Erosion/Dilation",false);

	img += erode + dilat;
	img *= 80;
	img.display("Juntas",false);

}
