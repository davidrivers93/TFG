/*
 * autorange_one.cpp
 *
 *  Created on: May 31, 2012
 *      Author: aalbiol
 */


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

#include <iostream>
#include <CImg.h>

#include "autorange.h"

using namespace cimg_library;

int main(int argc, char **argv) 
{
	float saturation_proportion = 0.02;
	if(argc < 2) {
		std::cerr << "One argument needed\n";
		exit(0);
	}

	CImg<unsigned char> input(argv[1]);
	CImg<unsigned char> output;

	//This is the function that you have to implement in autorange.cpp
	autorange(input, saturation_proportion, output);

	input.append(output,'x').display("AutoRange Result");

}
