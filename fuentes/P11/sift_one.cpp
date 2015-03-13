#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>

#include <getopt.h>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/video/tracking.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

#define DEFAULT_TH 0.04
void help()
{
	std::cout << "sift_one [options] imagefile\n";
	std::cout << " -T # : Hessian Threshold. Default: " << DEFAULT_TH << "\n";

}

int main(int argc, char** argv)
{
	int opt;
	cv::namedWindow("points");

	int nfeatures = 0;
	int nOctaveLayers = 3; //Lowe
	double contrastThreshold = 0.04;
	double edgeThreshold = 10.0;
	double sigma = 1.6;
	std::string  outputfilename;
	while((opt=getopt(argc,argv,"hT:o:"))!=-1)
	{

		switch(opt)
		{
		case 'h':
			help();
			exit(0);
			break;
		case 'T':
			contrastThreshold = atof( optarg );
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


	cv::Mat image = cv::imread(argv[optind]);
	cv::Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);

	if(gray.empty() )
	{
		std::cerr <<  "Can't read  the image\n";
		return -1;
	}


	// detecting keypoints. Default OpenCV Values

	cv::SIFT siftDetector( nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);



	std::vector<cv::KeyPoint> keypoints1;
	cv::Mat descriptors1;
	Mat mask;

	//Detect points and descriptors
	clock_t t1 = clock();
	siftDetector(gray, mask, keypoints1, descriptors1);
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






