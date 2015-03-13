// This program estimates the orientation of a picture of a text page.

// TO BE COMPLETED in the course

#include <vector>
#include <iostream>
#include <CImg.h>
#include "P8/segmentutils.h"
using namespace cimg_library;

int main(int argc, char **argv) 
{

	if( argc < 2 ){
		std::cout << "At least one argument needed\n";
		exit(0);
	}
//Read input image
CImg<unsigned char> input(argv[1]);

//Make sure it's gray
CImg<int> gray = input.get_resize(-100,-100,-100,1,2);

//First morphological filter to compensate paper shadows

//Second morphological filter to group chars in the same word


// Otsu binarization


//Binary Segmentation


//Moments computation and orientation and eigenvalues determination



//Rotation angle estimation


//Original Image rotation

}
