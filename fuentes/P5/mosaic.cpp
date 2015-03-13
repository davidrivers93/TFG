
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


void computeMosaicSize(const CImg<unsigned char> & left, const CImg<unsigned char> & center, const CImg<unsigned char> & right,
		CImg<float> & HLC, CImg<float> & HRC, int & panoramaWidth, int & panoramaHeight , int & basex, int & basey)
{
	// Determine the limits of the big picture
	float xmin = 0.0;
	float ymin = 0.0;
	float xmax = center.width() - 1;
	float ymax = center.height() - 1;


	//Left image: transform four corners
	CImg<float> s(1,3);
	CImg<float> d;
	float x, y;

	s(0) = 0;
	s(1) = 0;
	s(2) = 1; //(0,0)

	d= HLC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);

	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;


	s(0) = left.width()-1;
	s(1) = 0;
	s(2) = 1; //(w,0)

	d= HLC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);

	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;

	s(0) = 0;
	s(1) = left.height()-1;
	s(2) = 1; //(0,h)

	d= HLC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);

	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;

	s(0) = left.width()-1;
	s(1) = left.height()-1;
	s(2) = 1; //(w,h)

	d= HLC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);

	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;



	//right image: transform four corners

	s(0) = 0;
	s(1) = 0;
	s(2) = 1; //(0,0)

	d= HRC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);

	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;


	s(0) = right.width()-1;
	s(1) = 0;
	s(2) = 1; //(w,0)

	d= HRC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);

	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;

	s(0) = 0;
	s(1) = right.height()-1;
	s(2) = 1; //(0,h)

	d= HRC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);

	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;

	s(0) = right.width()-1;
	s(1) = right.height()-1;
	s(2) = 1; //(w,h)

	d= HRC * s;
	x = d(0)/d(2);
	y = d(1)/d(2);
	if(x < xmin)
		xmin = x;
	if(x > xmax)
		xmax = x;
	if(y < ymin)
		ymin = y;
	if (y > ymax)
		ymax = y;


	panoramaHeight = ymax - ymin;
	panoramaWidth = xmax-xmin;
	basex = xmin;
	basey = ymin;
	return;

}





int main(int argc, char **argv)
{


	const char * PLfilename = cimg_option("-PL", (char *) 0, "PointsL filename: points in left image shared with center");
	const char * PRfilename = cimg_option("-PR", (char *) 0, "PointsR filename");
	const char * PCLfilename = cimg_option("-PCL", (char *) 0, "PointsCL filename: points in central image shared with left");
	const char * PCRfilename = cimg_option("-PCR", (char *) 0, "PointsCR filename points in central image shared with right");

	const char * leftimage = cimg_option("-l", (char *) 0, "Left Image Filename");
	const char * centerimage = cimg_option("-c", (char *) 0, "Center Image Filename");
	const char * rightimage = cimg_option("-r", (char *) 0, "Right Image Filename");


	cimg_usage("This program  creates a panorama of 3 images");

	if(PLfilename == 0) {
		std::cerr << "Points file needed. Option -PL \n";
		exit(0);
	}


	if(PRfilename == 0) {
		std::cerr << "Points file needed. Option -PR \n";
		exit(0);
	}


	if(PCRfilename == 0) {
		std::cerr << "Points file needed. Option -PCR \n";
		exit(0);
	}


	if(PCLfilename == 0) {
		std::cerr << "Points file needed. Option -PCL \n";
		exit(0);
	}


	if(leftimage == 0) {
		std::cerr << "Input image needed. Option -l \n";
		exit(0);
	}

	if(centerimage == 0) {
		std::cerr << "Input image needed. Option -c \n";
		exit(0);
	}

	if(rightimage == 0) {
		std::cerr << "Input image needed. Option -c \n";
		exit(0);
	}



	CImg<float> PL, PR, PCR,PCL;

	PL.load_dlm(PLfilename);
	PR.load_dlm(PRfilename);
	PCL.load_dlm(PCLfilename);
	PCR.load_dlm(PCRfilename);
//Each CImg has 2 columns y N (points) rows

	{
		// Check input data
		if(PL.width()!=2 ) {
			std::cerr << "Points in " << PLfilename << " must have two columns\n";
			exit(0);
		}

		if(PR.width()!=2 ) {
			std::cerr << "Points in " << PRfilename << " must have two columns\n";
			exit(0);
		}


		if(PCL.width()!=2 ) {
			std::cerr << "Points in " << PCLfilename << " must have two columns\n";
			exit(0);
		}

		if(PCR.width()!=2 ) {
			std::cerr << "Points in " << PCRfilename << " must have two columns\n";
			exit(0);
		}


		if(PL.height() < 4 ) {
			std::cerr << "Points in " << PRfilename << " must have at least four rows";
			exit(0);
		}


		if(PR.height() < 4 ) {
			std::cerr << "Points in " << PRfilename << " must have at least four rows";
			exit(0);
		}


		if(PL.height() != PCL.height() ) {
			std::cerr << "Points in " << PLfilename << " and " << PCLfilename << " must have the same number of rows";
			exit(0);
		}

		if(PR.height() != PCR.height() ) {
			std::cerr << "Points in " << PLfilename << " and " << PCLfilename << " must have the same number of rows";
			exit(0);
		}

	}


	// Convert Points to OpenCV
	std::vector<Point2f> cv_PL( PL.height() );
	std::vector<Point2f> cv_PR( PR.height() );
	std::vector<Point2f> cv_PCL( PCL.height() );
	std::vector<Point2f> cv_PCR( PCR.height() );


	for( int p = 0 ; p < cv_PL.size() ; p++) {
		cv_PL[p].x = PL( 0 , p );
		cv_PL[p].y = PL( 1 , p );
		cv_PCL[p].x = PCL( 0 , p );
		cv_PCL[p].y = PCL( 1 , p );
	}


	for( int p = 0 ; p < cv_PR.size() ; p++) {
		cv_PR[p].x = PR( 0 , p );
		cv_PR[p].y = PR( 1 , p );
		cv_PCR[p].x = PCR( 0 , p );
		cv_PCR[p].y = PCR( 1 , p );
	}


	// Estimate HLC and HRC
	int method = 0; //Use all the points. Assume no outlier. See OpenCV doc for other options
	cv::Mat cvHLC = findHomography(cv_PL, cv_PCL, method);
	cv::Mat cvHRC = findHomography(cv_PR, cv_PCR, method);


	//Convert Matrix to CIMG
	CImg<float> HLC( cvHLC );
	CImg<float> HRC( cvHRC );

	//Load Images

	CImg<unsigned char> left(leftimage);
	CImg<unsigned char> center(centerimage);
	CImg<unsigned char> right(rightimage);

	int panoramaWidth;
	int panoramaHeight;
	int basex, basey;

	computeMosaicSize( left,  center, right,
			HLC, HRC,  panoramaWidth,  panoramaHeight ,  basex,  basey);

	// Create translation matrix to xmin, ymin

	CImg<float> HC(3,3);
	HC.fill(0.0);
	HC(0,0)= 1.0;
	HC(1,1)= 1.0;
	HC(2,2)= 1.0;
	HC(2,0) = -basex;
	HC(2,1) = -basey;

	HLC = HC * HLC;
	HRC = HC * HRC;

	int interpolation_method = METHOD_BILINEAR;
	CImg<unsigned char> lo,co,ro;

	lo.assign( panoramaWidth, panoramaHeight, 1 , left.spectrum() );
	int r = cimgPerspectiveTransform(left, HLC,  interpolation_method, lo);
	if (r < 0) {//Error
		std::cerr<< "Error Transforming Left\n";
		exit(0);
	}
	co.assign( panoramaWidth, panoramaHeight, 1 , left.spectrum() );
	r = cimgPerspectiveTransform(center, HC,  interpolation_method, co);
	if (r < 0) {//Error
		std::cerr<< "Error Transforming center\n";
		exit(0);
	}
	ro.assign( panoramaWidth, panoramaHeight, 1 , left.spectrum() );
	r = cimgPerspectiveTransform(right, HRC,  interpolation_method, ro);
	if (r < 0) {//Error
		std::cerr<< "Error Transforming Right\n";
		exit(0);
	}

	lo.save("leftout.jpg");
	co.save("centerout.jpg");
	ro.save("rightout.jpg");


}
