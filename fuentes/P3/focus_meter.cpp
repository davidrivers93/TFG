// Example program expands contrast of every frame in video
// Antonio Albiol 2012
// Universidad Politecnica Valencia, SPAIN

#include <time.h>


#include <iostream>
#include <vector>
#include <cv.h>
#include <highgui.h>

#define cimg_plugin1 "cimgcvMat.h"
#include <CImg.h>
using namespace cv;
using namespace cimg_library;

#include "linfilters.h"

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

int main( int argc, char** argv )
{
	unsigned char yellow[] = { 255, 255, 0};
	unsigned char white[] = { 255, 255, 255};
	unsigned char black[] = { 0, 0, 0};

	float opacity = 1.0;
	unsigned int pattern = ~0L; //32 bits are equal to 1

	float fraction_size = 1/4.0;
	float sigma = 3.0;

	CImgDisplay cdisplay;
	CImgDisplay viewfinder_display;

	if( argc < 2 )
	{
		std::cout << "No video filename given \n" ;
		return -1;
	}

	VideoCapture capture( argv[1] );
	if( !capture.isOpened() )
	{
		std::cerr << "Could not open video file:  " << argv[1] << "\n";
		exit(0);
	}
	Mat cv_image;

	CImg<unsigned char> best_focus_img;
	float Emax = -1.0;
	int kmax = -1;
	char txt[100];

	std::vector <float> energies;
	int k = 0;
	clock_t totalTicks = 0;
	while( capture.read(cv_image) ) {

		CImg<unsigned char> input(cv_image);
		CImg<unsigned char> output;
		int xmin = (0.5 - fraction_size / 2.0) * input.width();
		int xmax = (0.5 + fraction_size / 2.0) * input.width();
		int ymin = (0.5 - fraction_size / 2.0) * input.height();
		int ymax = (0.5 + fraction_size / 2.0) * input.height();

		CImg<unsigned char> viewer_image = input.get_resize_halfXY().resize_halfXY();

		//This is the function that you have to implement in linfilters.cpp
		clock_t t0 = clock();
		float E = highFrequencyEnergy(input, sigma, xmin, xmax, ymin, ymax);
		clock_t t1 = clock();
		totalTicks += (t1-t0);
		energies.push_back(E);

		//Draw rectangle on input image
		input.draw_rectangle (xmin, ymin, xmax, ymax, yellow, opacity, pattern);
		if( E > Emax ) {
			Emax = E;
			best_focus_img = input;
			kmax = k;
		}
		//Draw Energy text
		sprintf( txt, "%.2f", E);
		input.draw_text(xmin, ymax, txt, black, white);

		//Displays
		cdisplay.display(input).set_title("Focus Meter");
		viewfinder_display.display( viewer_image).set_title("ViewFinder");
		k++;
	}

	float totalTime = totalTicks;
	totalTime /= CLOCKS_PER_SEC;
	totalTime /= k;

	std::cout << "Average Time per image: " << totalTime << "\n";


	//Save Energies of every frame into file
	CImg<float> cimg_energies( energies.size() );

	for (int k=0; k < energies.size(); k++){
		cimg_energies[k] = energies[k];
	}
	cimg_energies.save_dlm("energies.dlm");
	std::cout << "energies.dlm saved. Open in MATLAB\n";


	//Display best focus image
	sprintf( txt, "Best focus at frame %d", kmax);
	best_focus_img.display(txt,false);


	return 0;
}
