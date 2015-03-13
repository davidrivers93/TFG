// Example to illustrate
//    Reading video (CIMG using opencv)


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE
#include <iostream>

#define cimg_use_opencv
#define cimg_plugin "opencv.h"
#include <CImg.h>

using namespace cimg_library;

int main( int argc, char** argv )
{
	CImg<unsigned char> img;
	CImgDisplay disp;
	int skip_frames = 0;
	bool release_video = false;
	std::string filename;

	if(argc < 2) {
		std::cerr << "One argument must be provided\n";
		exit(0);
	}
	filename = argv[1];

	std::cout << "Press Key to exit.\n";
	for (int k = 0; 1 ; k++){
		//Get next frame from video
		img.load_video( filename, skip_frames, release_video);
		if ( img.is_empty() ) {
			if (k == 0) {//Video not found
				std::cerr << "Could not read from video " << filename << "\n";
				exit(0);
			}
			else
				break; //End of video
		}

		disp.display(img).set_title("CImg-camera: %d", k);
		if ( disp.is_key() )
			break;
	}

	//Release video
	release_video = true;
	img.load_video( filename, skip_frames, release_video);
	return 0;
}
