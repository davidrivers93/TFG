// Example to illustrate

//    Conversion CIMG <--> OpenCV using plugin cimgcvMat.h

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE
#include <iostream>
#include <cv.h>
#include <highgui.h>

#define cimg_plugin1 "cimgcvMat.h"
#include <CImg.h>

using namespace cv;
using namespace cimg_library;

int main( int argc, char** argv )
{

	namedWindow( "OpenCV-Display", CV_WINDOW_AUTOSIZE );
	CImgDisplay cimg_display1;

	if(argc < 2) {
		std::cerr << "An argument must be given\n";
		exit(0);
	}

//Read CIMG Image
	CImg<unsigned char> cimg( argv[1] );
	cimg.display("Original CIMG Image in CIMG Window\n",false);

//Create an OPENCV copy	from CIMG
	Mat cv_image = cimg.get_MAT();
	imshow("OpenCV-Display", cv_image);
	std::cout << "Press key...\n";
	cv::waitKey(0); //Wait for key press
	destroyWindow("OpenCV-Display"); //Close OpenCV Window

// Convert to CIMG from OPENCV
	CImg<unsigned char> cimg2(cv_image);
	cimg2.display("Final CIMG Image in CIMG Window\n",false);

	return 0;
}
