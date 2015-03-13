/*
 * plantilla.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: aalbiol
 */

#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>

#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"
#include "CImg.h"

#if cimg_os==2 //Windows
#include "getopt.h"
#else
#include <unistd.h>
#include <stdlib.h>
#endif

#include "utilities.h"


using namespace cimg_library;
using namespace std;
using namespace cv;

void ayuda() {
	std::cout << "Poner algo para recordar las opciones\n";
}

int main(int argc, char **argv)
{

	int opt;
	double capt_width =  320;
	double capt_height = 240;
	CImgDisplay disp_cimg;

	//Read command line options
	/*while((opt=getopt(argc,argv,"hW:H:"))!=-1) {//
		switch(opt) {

		case 'h':
			ayuda();
			exit(0);
			break;
		case 'W':
			capt_width = atoi(optarg);
			break;
		case 'H':
			capt_height = atoi(optarg);
			break;
		}
	}*/



	if(optind==argc){
		ayuda();
		exit(0);
	}

	std::string input = argv[optind];

	// ==== Determine kind of input ===========
	int camera_number = isCamera(input);
	// camera == -1 if it is not a camera
	bool is_Video = isVideo(input);
	set <string> images = isImages(input);


	if ( camera_number >= 0)
		std::cout << "It is Camera number :" << camera_number << "\n";

	if( is_Video )
		std::cout << "It is VideoFile :" << input << "\n";

	if( images.size() ) {
		int nimags = images.size();
		std::cout << "Image Files: \n";
		std::set<string>::iterator it;
		for (it = images.begin(); it != images.end(); it++) {
			std::cout << "\t" << *it<< " \n";
		}
		std::cout << "\n";
	}

	// ----- Video File Section ----------------------------
	if (is_Video) {

		CImg<unsigned char> imgv;
		int skip_frames = 0;
		bool release_video = false;
		int count;
		for( count = 0; 1 ; count++ ) {
			imgv.load_video( input, skip_frames, release_video);
			if ( imgv.is_empty() ) {
				if ( count == 0) {//Video not found
					std::cerr << "Could not read from video " << input << "\n";
					exit(0);
				}
				else
					break; //End of video
			}

			//Here you can do whatever you want with imgv. The following is just an example
			CImg<unsigned char> out = 255 - imgv;
			out = imgv.get_append(out,'x');
			disp_cimg.display(out).set_title( "Video" );
		}
		release_video = true;
		imgv.load_video( input, skip_frames, release_video);
	}


	// ----- Camera Section ----------------------------
	if (camera_number >= 0 ) {
		//First image. Set params
		CImg<unsigned char> cimg_img;
		cimg_img.load_camera( camera_number, 0, false, capt_width, capt_height);
		if ( cimg_img.is_empty() ) {
			std::cerr << "Could not capture from camera " << camera_number << "\n";
			exit(0);
		}

		while(1) {
			// Load image
			cimg_img.load_camera(camera_number);
			if ( cimg_img.is_empty() )
				break;

			// Do what you want with  cimg_img
			CImg<unsigned char> out = 255-cimg_img;
			out = cimg_img.get_append(out,'x');
			disp_cimg.display(out).set_title( "Camera" );
		}

		//Release camera
		cimg_img.load_camera( camera_number, 0, true, capt_width, capt_height);
	}



	// ----- Images Section ----------------------------
	if (images.size() ) {
		std::set<string>::iterator it;
		for (it = images.begin(); it != images.end(); it++) {
			string imgname = *it;
			CImg<unsigned char> img ( imgname.c_str() );

			//Here you can do whatever you want with img
			CImg<unsigned char> out = 255- img;
			out = img.get_append(out,'x');
			disp_cimg.display(out).set_title( imgname.c_str() );
			cimg::sleep(1000);
		}
	}

	// End of program. Write results to disk...
}
