// Example program expands contrast of every frame in video
// Antonio Albiol 2012
// Universidad Politecnica Valencia, SPAIN


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

#include <iostream>
#include <string>
#include <cv.h>
#include <highgui.h>

#define cimg_plugin1 "cimgcvMat.h"
#include <CImg.h>
using namespace cv;
using namespace cimg_library;

#include "autorange.h"
#include <smartdisplay3.h>

int main( int argc, char** argv )
{
	float saturation_proportion = 0.02;
	smartdisplay3<unsigned char> cdisplay;
	std::string	title = "Auto-range";

	if( argc < 2 )
	{
		printf( "No video filename given \n" );
		return -1;
	}


	VideoCapture capture( argv[1] );
	if( !capture.isOpened() )
	{
		std::cerr << "Could not open video file:  " << argv[1] << "\n";
		exit(0);
	}
	Mat cv_image;

	int k = 0;
	while( capture.read(cv_image) ) {

		CImg<unsigned char> input(cv_image);
		CImg<unsigned char> output;

		//This is the function that you have to implement in autorange.cpp
		autorange(input, saturation_proportion, output);

		//Put input and output together
		input.append(output,'y');
		cdisplay.display(input,title);

	}

	return 0;
}
