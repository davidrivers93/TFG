// Example program to that computes the average of the frames in a video
// Antonio Albiol 2012
// Universidad Politecnica Valencia, SPAIN

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
	CImgDisplay cdisplay;
	CImgDisplay cdisplay_input;
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
	CImg<float> sum_image;
	int k = 0;
	while( capture.read(cv_image) ) {

		CImg<unsigned char> cimg(cv_image);
		cdisplay_input.display(cimg).set_title("Input");
		if (k == 0){
			sum_image = cimg;
		}
		else
			sum_image += cimg;
		k++;
		if( k%50 == 0) {
			CImg<unsigned char> showimage = sum_image / k;
			cdisplay.display(showimage).set_title("Average of %d images",k);
		}
	}
	cdisplay.close();
	cdisplay_input.close();
	//The loop ends when the video is finished. Show result
	CImg<unsigned char> showimage = sum_image / k;
	showimage.display("Final Result",false);
	return 0;
}
