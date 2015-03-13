/*
 * tgeometry.cpp
 *
 *  Created on: Jun 11, 2012
 *      Author: aalbiol
 */

#include <iostream>
#include <vector>
#include <cv.h>

#define cimg_plugin1 "cimgcvMat.h"
#include <CImg.h>
using namespace cimg_library;

#include "tgeometry.h"

int cimgPerspectiveTransform(const cimg_library::CImg<unsigned char> & in,
		const cimg_library::CImg<float> & H, int method,
		cimg_library::CImg<unsigned char> & out) {

	// Check input args
	if(out.size() == 0) {
		std::cerr << "Output image must be preallocated at cimgPerspectiveTransform\n";
		return -1;
	}

	if(in.spectrum() != out.spectrum() ) {
		std::cerr << "Output image must have the same number of color planes as input,  at cimgPerspectiveTransform\n";
		return -1;
	}

	if(H.width() != 3 || H.height() != 3 || H.size() != 9 ){
		std::cerr << "H must be 3x3,  at cimgPerspectiveTransform\n";
		return -1;
	}

	if( H.det() == 0.0){
		std::cerr << "H have non-nil determinant,   at cimgPerspectiveTransform\n";
		return -1;
	}

	if (method != METHOD_NN && method != METHOD_BILINEAR){
		std::cerr << "method must be bilinear or NN   at cimgPerspectiveTransform\n";
		return -1;
	}

	// Determine Inverse Mapping
	CImg<float> Hinv = H.get_invert();

	int w2 = out.width();
	int h2 = out.height();

	int w1_1 = in.width() - 1;
	int h1_1 = in.height() - 1;

	CImg<float> outpoint(1,3);
	outpoint(2) = 1;

	for( int x = 0; x < w2 ; x++ )
		for(int y=0; y < h2 ; y++) { // For every point in output image

			// Find floating point coordinates in input image
			outpoint[0] = x;
			outpoint[1] = y;
			CImg<float> inpoint = Hinv * outpoint;
			if(inpoint[2] == 0.0)
				continue;
			float xinput = inpoint[0]/inpoint[2];
			float yinput = inpoint[1]/inpoint[2];

			// Check if they fall within input limits
			if( xinput <= 0 || xinput >= w1_1)
				continue;
			if( yinput <= 0 || yinput >= h1_1)
				continue;
			// If they fall into limits compute output value using NN or bilinear
			if( method == METHOD_NN) {
				int xinput_int = int(xinput+0.5);
				int yinput_int = int(yinput+0.5);
				for(int c= 0; c < in.spectrum(); c++)
					out( x , y , 0 , c) = in( xinput_int , yinput_int , 0 , c);
			}

			if( method == METHOD_BILINEAR) {
				int basex = floor(xinput);
				int basey = floor(yinput);
				float fx = xinput - basex;
				float fy = yinput - basey;

				for(int c= 0; c < in.spectrum(); c++) {
					float v = in( basex , basey , 0 , c)* (1.0-fx) * (1.0-fy);
					v += in( basex , basey+1 , 0 , c)* (1.0-fx) * fy;
					v += in( basex+1 , basey , 0 , c)* fx * (1.0-fy);
					v += in( basex+1 , basey +1, 0 , c)* fx * fy;
					out( x , y , 0 , c) = v;
				}
			}


		}

	return 0; // OK
}



//Determine Homography transformation given a series of correspondences
cimg_library::CImg<float> ComputeHomography(std::vector<cimg_library::CImg<float> > &x1, std::vector<cimg_library::CImg<float> > &x2) {

	if( x1.size() != x2.size() )
		return CImg<float>() ;

	int np = x1.size();
	std::vector<cv::Point2f> cv_P1( np );
	std::vector<cv::Point2f> cv_P2( np );
	for(int p=0; p < np ; p++) {
		cv_P1[p].x = x1[p][0];
		cv_P1[p].y = x1[p][1];
		cv_P2[p].x = x2[p][0];
		cv_P2[p].y = x2[p][1];
	}
	cv::Mat cv_H = cv::findHomography(cv_P1, cv_P2, 0); //0::No outliers
	CImg<float> H(cv_H);
	return H;
}



//Determine Homography transformation given a series of correspondences
cimg_library::CImg<float> ComputeHomography(cimg_library::CImg<float> &x1, cimg_library::CImg<float>  &x2) {

	if( x1.height() != x2.height() || x1.width() != 2 || x2.height() != 2)
		return CImg<float>() ;
	int np = x1.height();

	std::vector<cv::Point2f> cv_P1( np );
	std::vector<cv::Point2f> cv_P2( np );
	for(int p=0; p < np ; p++) {
		cv_P1[p].x = x1(0,p);
		cv_P1[p].y = x1(1,p);
		cv_P2[p].x = x2(0,p);
		cv_P2[p].y = x2(1,p);
	}


	cv::Mat cv_H = cv::findHomography(cv_P1, cv_P2, 0); //0::No outliers
	CImg<float> H(cv_H);
	return H;
}






// Takes one CIMg vector of two elements and applyies homography given by H
CImg<float> TransformPoint(const CImg<float> & H, const CImg<float> & src){
	CImg<float> dst(1,2);
	CImg<float> t (1,3);
	t[0] = src[0];
	t[1] = src[1];
	t[2] = 1;
	CImg<float> d = H * t;
	dst[0] = d[0]/d[2];
	dst[1] = d[1]/d[2];
	return dst;
}



// Takes a list of  CIMg vector of two elements and applyies homography given by H
CImgList<float> TransformPoints(const CImg<float> & H, const CImgList<float> & points){
	int np = points.size();
	CImgList<float> dest(np,1,2);
	CImg<float> t (1,3);
	for(int p= 0; p< np ; p++) {
		const CImg<float> & src = points[p];
		CImg<float> & dst = dest[p];
		t[0] = src[0];
		t[1] = src[1];
		t[2] = 1;
		CImg<float> d = H * t;
		dst[0] = d[0]/d[2];
		dst[1] = d[1]/d[2];
	}
	return dest;
}


// Takes one CIMg vector of two elements and applies the INVERSE of homography given by H
CImg<float> InvertPoint(const CImg<float> & H, const CImg<float> & src){
	CImg<float> dst(1,2);
	CImg<float> t (1,3);
	CImg<float> Hinv = H.get_invert();
	t[0] = src[0];
	t[1] = src[1];
	t[2] = 1;
	CImg<float> d = Hinv * t;
	dst[0] = d[0]/d[2];
	dst[1] = d[1]/d[2];
	return dst;
}





// Takes alist of CIMg vectors of two elements and applies the INVERSE of homography given by H
CImgList<float> InvertPoints(const CImg<float> & H, const CImgList<float> & points){
	int np = points.size();
	CImgList<float> dest(np,1,2);
	CImg<float> t (1,3);
	CImg<float> Hinv = H.get_invert();
	for(int p= 0; p< np ; p++) {
		const CImg<float> & src = points[p];
		CImg<float> & dst = dest[p];
		t[0] = src[0];
		t[1] = src[1];
		t[2] = 1;
		CImg<float> d = Hinv * t;
		dst[0] = d[0]/d[2];
		dst[1] = d[1]/d[2];
	}
	return dest;
}
