
#include <iostream>
#include <vector>
#include <string>

#include <getopt.h>
#include <cv.h>

#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"




using namespace cv;
using namespace std;

#define DEFAULT_THRESH_SURF_OPENCV 2000 //OpenCV Doc recommends 300--500
#define DEFAULT_OCTAVES_SURF_OPENCV 5

void help()
{
	std::cout << "surf_opencv_two [options] imagefile\n";
	std::cout << "-O #: number of octaves. Default=" << DEFAULT_OCTAVES_SURF_OPENCV <<"\n";

	std::cout << "-T #: blob response threshold.Default= "<< DEFAULT_THRESH_SURF_OPENCV <<"\n";
	std::cout << "-u 0/1: run in rotation invariant mode. Default = " << true <<"\n";;
}

int main(int argc, char** argv)
{
	int opt;
	cv::namedWindow("points");
	double hessianThreshold = DEFAULT_THRESH_SURF_OPENCV;
	int nOctaves = DEFAULT_OCTAVES_SURF_OPENCV;
	int nOctaveLayers = 2;
	bool extended = false; // if false 64 elements descriptor. If true 128 elements descriptor
	bool upright = false ; // Orientation of descriptor is estimated if false

	std::string  outputfilename;
	while((opt=getopt(argc,argv,"hO:T:u:o:"))!=-1)
	{

		switch(opt)
		{
		case 'h':
			help();
			exit(0);
			break;
		case 'O':
			nOctaves=atoi(optarg);
			break;
		case 'T':
			hessianThreshold = atof(optarg);
			break;
		case 'u':
			upright=atoi(optarg);
			break;
		case 'o':
			outputfilename = optarg;
			break;

		}
	}

	if(optind>=argc)
	{
		help();

		exit(0);
	}


	Mat image = imread(argv[optind] );
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	if(gray.empty() )
	{
		std::cerr <<  "Can't read  the image\n";
		return -1;
	}


	// detecting keypoints. Default OpenCV Values

	SURF detector( hessianThreshold, nOctaves, nOctaveLayers, extended, upright);



	vector<KeyPoint> keypoints1;
	Mat descriptors1;
	Mat mask;
	//Detect points and descriptors
	clock_t t1 = clock();
	detector(gray, mask, keypoints1, descriptors1);
	clock_t t2 = clock();


	Scalar color = Scalar::all(-1);
	int flagsp =DrawMatchesFlags::DRAW_RICH_KEYPOINTS;
	cv::Mat imgpoints1;
	drawKeypoints(image, keypoints1, imgpoints1, color, flagsp );
	std::cout << "Detected " << keypoints1.size() << "\n";
	std::cout << "Time to detect points and compute descriptors: " << (t2-t1)*1000.0/CLOCKS_PER_SEC << " ms\n";


	if(outputfilename.size() > 0)
		imwrite( outputfilename.c_str(), imgpoints1);


	imshow("points", imgpoints1);
	waitKey(0);

	return 0;
}






