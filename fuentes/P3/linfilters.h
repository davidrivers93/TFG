/*
 * linfilters.h
 *
 *  Created on: Jun 6, 2012
 *      Author: aalbiol
 */

#ifndef LINFILTERS_H_
#define LINFILTERS_H_


float highFrequencyEnergy(cimg_library::CImg<unsigned char> & input, float sigma, int xmin, int xmax, int ymin, int ymax);

void integral_image(const cimg_library::CImg<unsigned char> & input, cimg_library::CImg<double> & integral);

void box_average(const cimg_library::CImg<unsigned char> & input, int filtersize, cimg_library::CImg<unsigned char> & output) ;

#endif /* LINFILTERS_H_ */
