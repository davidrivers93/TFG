// Antonio Albiol 2012
// Universidad Politecnica Valencia, SPAIN

#include <time.h>
#include <iostream>
#include <CImg.h>

using namespace cimg_library;
#include "linfilters.h"

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

int main(int argc, char **argv) 
{

	const int box_size = cimg_option("-s", 15, "Size of the averaging filter");
	const char * filename = cimg_option("-i", (char *) 0, "Input image name");
	const int ntimes = cimg_option("-n", 1, "Number of repetitions");
	float alfa = cimg_option("-a",0.5,"Alfa for unsharp enhancement");
	cimg_usage("This program computes the moving average of an image");

	if(filename == 0) {
		std::cerr << "One argument needed. Option -i \n";
		exit(0);
	}
	CImg<unsigned char> img( filename );
	CImg<unsigned char> input(img);
	CImg<unsigned char> output;
	clock_t t0 = clock();
	for (int n= 0; n < ntimes; n ++) {
		box_average(input, box_size, output);

		input.swap(output);
	}
	//Final result is in input
	clock_t t1 = clock();

	float totalTime = t1 - t0;
	totalTime /= CLOCKS_PER_SEC;


	std::cout << "Computing Time per image: " << totalTime << "\n";

	input.display("Filtered",false);
	input.save_tiff("lowpass.tif");

	//Now, let's see what has been filtered out by computing difference

	//Declare image and allocate memory for pixels
	CImg<unsigned char> difference(img) ;

	for(int o=0; o < difference.size(); o++) {
		int d = img[o] - input[o] + 128;
		if(d > 255)
			d = 255;
		else if(d < 0)
			d= 0;
		difference[o] = d;
	}
	difference.display("Difference",false);

	CImg<unsigned char> enhanced(img) ;

	for(int o=0; o < enhanced.size(); o++) {
		int d = img[o] + alfa *(difference[o] - 128);
		if(d > 255)
			d = 255;
		else if(d < 0)
			d= 0;
		enhanced[o] = d;
	}
	enhanced.display("Enhanced",false);
}
