/*
 * linfilters.cpp
 *
 *  Created on: Jun 6, 2012
 *      Author: aalbiol
 */

#include <iostream>

#include <CImg.h>
using namespace cimg_library;

#include "linfilters.h"


// *** THIS SECTION SHOULDN'T BE CHANGED IN THE COURSE

float highFrequencyEnergy(cimg_library::CImg<unsigned char> & input, float sigma, int xmin, int xmax, int ymin, int ymax)
{
	CImg<unsigned char> im;
	if(input.spectrum() == 3) {
		im = input.get_shared_channel(1);
	}
	else
		im = input.get_shared(); //No pixel copy

	CImg<float> rectangle = im.get_crop( xmin, ymin, xmax, ymax);
	CImg<float> im_lowpas = rectangle.get_blur(sigma);
	CImg<float> im_highpass = rectangle - im_lowpas;

	float E= im_highpass.sqr().sum();
	E /= rectangle.size();
	return E;
}





void  integral_image(const cimg_library::CImg<unsigned char> & input, cimg_library::CImg<double> & integral) {

	integral.assign( input.width() , input.height(), 1, input.spectrum() );

	for(int c = 0; c < input.spectrum(); c++ ) {
		CImg<unsigned char> in_channel = input.get_shared_channel(c);
		CImg<double> integral_channel = integral.get_shared_channel(c);

		//First pixel
		integral_channel(0,0) = in_channel(0,0);

		//First row
		for (int x = 1; x< integral.width() ; x++) {
			integral_channel(x,0) = integral_channel(x-1,0) + in_channel(x,0);
		}

		//The rest of rows
		for(int y = 1; y < integral_channel.height(); y++) {

			float rowSum = in_channel(0,y);
			integral_channel(0,y) = rowSum + integral_channel(0,y-1);

			for (int x = 1; x< integral_channel.width() ; x++) {
				rowSum += in_channel(x,y);
				integral_channel(x,y) = rowSum + integral_channel(x,y-1);
			}
		}
	}

}



// *** THIS SECTION MUST BE COMPLETED IN THE COURSE . LABTDI

void box_average(const cimg_library::CImg<unsigned char> & input, int filtersize, cimg_library::CImg<unsigned char> & output) {

	//This line reserves memory for the output image and makes a copy of the input image
	output.assign( input );


	double area = filtersize * filtersize;


	//Compute integral image
	CImg<double> integral;
	integral_image( input, integral );

	//TODO: Use integral image to implement filtering

}










