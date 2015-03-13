// Antonio Albiol 2012
// Universidad Politecnica Valencia, SPAIN

// This file shows how to estimate an homography transform using OpenCV
// from  two sets of correspoding points

#include <time.h>
#include <iostream>
#include <vector>

#include <cv.h>
#define cimg_plugin1 "cimgcvMat.h"
#include <CImg.h>
using namespace cv;

using namespace cimg_library;


#include "tgeometry.h"
// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

int main(int argc, char **argv) 
{


	const char * P1filename = cimg_option("-P1", (char *) 0, "Points1 filename");
	const char * P2filename = cimg_option("-P2", (char *) 0, "Points2 filename");
	cimg_usage("This program estimates the Homography Matrix so that P2 = H P1");

	if(P1filename == 0) {
		std::cerr << "Points1 file needed. Option -P1 \n";
		exit(0);
	}


	if(P2filename == 0) {
		std::cerr << "Points2 file needed. Option -P1 \n";
		exit(0);
	}


	CImg<float> P1, P2;

	P1.load_dlm(P1filename);
	P2.load_dlm(P2filename);


// Check input data
	if(P1.width()!=2 ) {
		std::cerr << "Points in " << P1filename << " must have two columns\n";
		exit(0);
	}

	if(P2.width()!=2 ) {
		std::cerr << "Points in " << P2filename << " must have two columns\n";
		exit(0);
	}

	if(P1.height() < 4 ) {
		std::cerr << "Points in " << P1filename << " must have at least four rows";
		exit(0);
	}


	if(P2.height() < 4 ) {
		std::cerr << "Points in " << P2filename << " must have at least four rows";
		exit(0);
	}


	if(P1.height() != P2.height() ) {
		std::cerr << "Points in " << P1filename << " and " << P2filename << " must have the same number of rows";
		exit(0);
	}


// Convert Points to OpenCV
	std::vector<Point2f> cv_P1( P1.height() );
	std::vector<Point2f> cv_P2( P2.height() );
	for( int p = 0 ; p < cv_P1.size() ; p++) {
		cv_P1[p].x = P1( 0 , p );
		cv_P1[p].y = P1( 1 , p );
		cv_P2[p].x = P2( 0 , p );
		cv_P2[p].y = P2( 1 , p );
	}

// Estimate H
	int method = 0; //Use all the points. Assume no outlier. See OpenCV doc for other options
	cv::Mat cvH = findHomography(cv_P1, cv_P2, method);


//Convert Matrix to CIMG
	CImg<float> H( cvH );


//Print and save
	H.print("H matrix");
	H.save_dlm("H.dlm");


}
