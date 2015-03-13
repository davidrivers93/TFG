/*
 * otsu.cpp
 *
 *  Created on: Jun 28, 2012
 *      Author: aalbiol
 */


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

#include <iostream>
#include <CImg.h>
#include "segmentutils.h"

using namespace cimg_library;



int main(int argc, char **argv)
{
	unsigned char green[] ={0, 255, 0};
	unsigned char white[] ={255, 255, 255};
	if(argc < 2) {
		std::cerr << "One argument needed\n";
		exit(0);
	}

	CImg<unsigned char> img( argv[1] );

	if( img.spectrum() > 1)
	{
		std::cerr << "Image can not be colour\n";
		exit(0);
	}


	CImg<float> hist = img.get_histogram(256,0,255);

	CImg<unsigned char> visu(500,400,1,3,0); // To create plot
	float opacity = 1.0;
	int plot_type = 1;
	int vertex_type = 0;
	visu.draw_graph(hist,green,opacity,plot_type,vertex_type,hist.max(),0);
	int th = otsu(hist);

	std::cout << "Otsu th = " << th << "\n";
	int xx = visu.width() / 255.0 * th;
	visu.draw_line( xx,0, xx, visu.height()-1, white, opacity/2);
	visu.display("Histogram",false);

	CImg<unsigned char>  imth = 255 * img.get_threshold(th);
	img.get_append(imth,'x').display("Thresholded", false);
	if(argc >= 3) {
		imth.save(argv[2]);
		std::cout << "Saving result at " << argv[2] << "\n";
	}

}
