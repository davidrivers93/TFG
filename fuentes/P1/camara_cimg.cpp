// Example to illustrate
//    Camera Capture (CIMG using opencv)


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE
#include <iostream>

#define cimg_use_opencv
#include <CImg.h>

using namespace cimg_library;


int main( int argc, char** argv )
{
	CImg<unsigned char> img;
	CImgDisplay disp;

	int capt_width = 320.0;
	int capt_height = 240.0;

	int cam = 0;
	bool release_camera = false;
	int skip_frames = 0;

	if(argc > 1) //Optionally select camera
		cam = atoi(argv[1]);

	//First image. Set params
	img.load_camera( cam, skip_frames, release_camera, capt_width, capt_height);
	if ( img.is_empty() ) { //Camera not found
		std::cerr << "Could not capture from camera " << cam << "\n";
		exit(0);
	}

	std::cout <<"Press Key to exit.\n";
	for (int k = 0; 1 ; k++){
		img.load_camera( cam );
		if ( img.is_empty() ) { //Camera error
			break;
		}
		disp.display(img).set_title("CImg-camera: %d", k);
		if ( disp.is_key() )
			break;
	}

	//Release camera
	img.load_camera( cam, skip_frames, true, capt_width, capt_height);
	return 0;
}
