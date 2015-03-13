// Example to illustrate
//    Camera Capture (OpenCV)


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

#include <cv.h>
#include <highgui.h>

#define cimg_plugin1 "cimgcvMat.h"


using namespace cv;


int main( int argc, char** argv )
{

	//Define an OPENCV  window to view camera
	const std::string win_name = "OpenCV-Display";
	int flags = CV_WINDOW_AUTOSIZE;
	cv::namedWindow( win_name, flags);

	double capt_width = 320.0;
	double capt_height = 240.0;

	int cam = 0;

	if(argc > 1)
		cam = atoi(argv[1]);

	VideoCapture capture(cam);

	capture.set(CV_CAP_PROP_FRAME_WIDTH , capt_width);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, capt_height);

	Mat cv_image;
	std::cout << "This program stops after 300 images or press ESC Key \n";
	for (int k = 0; k < 300; k++){
		capture >> cv_image; //Get image from camera
		imshow(win_name, cv_image);
		char c = cv::waitKey(10) ; //Wait 200 mseconds
		  if( c == 27 )
		     break;

	}

	return 0;
}
