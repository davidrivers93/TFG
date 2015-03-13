// Example program to show how to read frames from video file .AVI, .MPEG, etc

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{

	namedWindow( "OpenCV-Display", CV_WINDOW_AUTOSIZE );

	if( argc < 2 )
	{
		printf( "No video filename given \n" );
		return -1;
	}


	VideoCapture capture( argv[1] );

	if( !capture.isOpened() )
	{
		std::cerr << "Could not open video file:  " << argv[1] << "\n";
		exit(0);
	}
	Mat cv_image;
	int k = 0;
	while( capture.read(cv_image) ) {
		std::cout << "Reading " << k << " frames\n";
		imshow( "OpenCV-Display", cv_image );
		cv::waitKey(40); //msec
		//Here you can do whatever you like with pixels
		k++;
	}

	//The loop ends when the video is finished

	return 0;
}
