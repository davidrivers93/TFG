
#include <iostream>

#include <getopt.h>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/video/tracking.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define cimg_plugin1 "cimgcvMat.h"

#include <CImg.h>
#include "hough.h"
#include <math.h>
using namespace cv;
using namespace std;
#include "upv_defaults.h"
#include <vector>



void help()
{
	std::cout << "surf_opencv_two [options] imagefile1 imagefile2\n";
	std::cout << "surf_opencv_two [options] imagefile1 camera_number\n";
	std::cout << "-O #: number of octaves. Default=" << DEFAULT_OCTAVES_SURF_OPENCV <<"\n";

	std::cout << "-T #: blob response threshold.Default= "<< DEFAULT_THRESH_SURF_OPENCV <<"\n";
	std::cout << "-u 0/1: 0:Estimate point orientation. 1: Assume same orientation. Default = " << 0 <<"\n";;
}

int main(int argc, char** argv)
{
	cimg_library::CImgDisplay cdisp;

	int flags = CV_WINDOW_AUTOSIZE;
	cv::namedWindow( "output_win", flags);
	int opt;

	double hessianThreshold = DEFAULT_THRESH_SURF_OPENCV; //OpenCV Doc recommends 300--500
	int nOctaves = DEFAULT_OCTAVES_SURF_OPENCV;
	int nOctaveLayers = 2;
	bool extended=false; // if false 64 elements descriptor. If true 128 elements descriptor
	bool upright=false ; // Orientation of descriptor is estimated if false

	float matchRatio = DEFAULT_MATCH_RATIO;

	bool cameraMode = false;

	int screen_width = cdisp.screen_width();
//	int screen_height = cdisp.screen_height();


	// Process cmd line options
	while((opt=getopt(argc,argv,"hO:T:u:"))!=-1)
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

		}
	}

	//Not enough args
	if(optind +1 >= argc)
	{
		help();
		exit(0);
	}


	Mat img1 = imread(argv[optind]);
	if(img1.empty() ){
		std::cerr <<  "Can't read  the image\n";
		return -1;
	}



	Mat img2;
	VideoCapture capture;

	if( strlen (argv[optind+1]) > 1) // 2 images
		img2 = imread(argv[optind+1], CV_LOAD_IMAGE_GRAYSCALE);
	else { // 1 image + camera input
		cameraMode = true;
		int cam = atoi(argv[optind+1]);

		capture.open(cam);
		if (!capture.isOpened() ) {
			std::cerr << "Can't open camera " << cam << "\n";
			exit(0);
		}
		float capt_width = 800;
		float capt_height = 600;
		capture.set(CV_CAP_PROP_FRAME_WIDTH , capt_width);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, capt_height);
		capture >> img2;
	}

	int total_width = img1.cols+img2.cols;


	// Check if screen is large enough. If not, compute scale factor for output window
	double scale = double(screen_width) / total_width;


	if (scale < 1) {
		scale *=  0.8;
	}


	// Set up detector.
	SURF detector( hessianThreshold, nOctaves, nOctaveLayers, extended, upright);



	vector<KeyPoint> keypoints1;
	Mat descriptors1;
	Mat mask;


	clock_t t1 = clock();
	//Detect points on image 1 and descriptors
	detector(img1, mask, keypoints1, descriptors1);
	clock_t t2 = clock();


	//Scale points 1 for representation
	Mat im1e, im2e; // Images for output window
	im1e=img1;
	vector<KeyPoint> keypoints1e = keypoints1;
	if (scale < 1) {
		resize(img1,im1e, Size(), scale, scale, INTER_NEAREST );
		for (int n =0; n< keypoints1e.size(); n++) {
			KeyPoint & p = keypoints1e[n];
			p.pt.x *= scale;
			p.pt.y *= scale;
		}
	}



	// Create MAtchers
	Ptr<DescriptorMatcher> dmatcher = DescriptorMatcher::create ("BruteForce");

	int distance_type = NORM_L2;
	bool crossCheck=true;
	BFMatcher bfmatcher(distance_type, crossCheck);


	//Loop images
	for(int n = 0; (n< 1) || cameraMode;  n++) {

		if(cameraMode)
			capture >> img2;

		//Detect points on image 2
		vector<KeyPoint> keypoints2;
		Mat descriptors2;

		clock_t t3 = clock();
		detector(img2, mask, keypoints2, descriptors2);
		clock_t t4 = clock();


		vector<DMatch> matches12;
		vector<vector<DMatch> > matches12nn;
		vector<DMatch> matches12ratio;
		vector<DMatch>  houghMatches;

		if (keypoints1.size() > 20 && keypoints2.size() > 20 &&  !descriptors2.empty()) {

			//NN with crosscheck
			clock_t t5 = clock();
			bfmatcher.match( descriptors1, descriptors2, matches12 );
			clock_t t6 = clock();

			// 2-NN with ratio validation
			int knn = 2;
			clock_t t7 = clock();
			dmatcher->knnMatch( descriptors1, descriptors2, matches12nn, knn);

			//Select Matches by distance ratio and cross-check


			for(int m = 0; m< matches12nn.size() ; m++)
			{
				vector<DMatch> & mm = matches12nn[m];
				if ( mm.size() == 1 )
					matches12ratio.push_back( mm.front() );
				else if (mm.size() > 1 )
				{
					DMatch & candidate_match = mm.front();
					float d1 = candidate_match.distance;
					const DMatch & mm2 = mm[1];
					float d2 = mm2.distance;
					if ( d1 / d2 < matchRatio ) {
						//Now cross-check
						for(int j= 0; j < matches12.size(); j++) {
							DMatch & bfmatch = matches12[j];
							if ( (bfmatch.trainIdx == candidate_match.trainIdx) && (bfmatch.queryIdx == candidate_match.queryIdx)) {
								matches12ratio.push_back( mm.front() );
								break;
							}
						}
					}

				}
			}

			clock_t t8 = clock();
			clock_t t9 = clock();
			//Hough
			int nscales = 5;
			std::vector< float > scales(nscales);
			int nrotations = 1;
			if ( !upright ) //if upright I know that there is no rotation
				nrotations = 12;
			std::vector < float > rotations(nrotations);

			for (int r = 0; r < rotations.size() ; r++)
				rotations[r] = -150 + 30.0 * r;

			if (nrotations == 1)
				rotations[0] = 0.0;

			int centrals = nscales / 2;
			for (int s = 0; s < nscales; s++){
				float ss = s - centrals;
				scales[s] = powf( float(2.0), float(ss / 2.0) );
			}

			int minMatchNumber = 7;
			int displayH = 1;
			if (cameraMode)
				displayH = 0;
			HoughUPV houghupv(   rotations,  scales, img1.cols, img1.rows,  img2.rows, img2.cols,  minMatchNumber, displayH);

			houghupv.process( keypoints1, keypoints2, matches12ratio, houghMatches);
			clock_t t10 = clock();
			if(houghMatches.size() < 5)
				houghMatches.clear();


			//Generate output

			if (!cameraMode) {
				std::cout << "Descriptor Size =" << descriptors2.cols << " cols x " << descriptors2.rows << " rows\n";
				std::cout << "Detected in img1 " << keypoints1.size() << "\n";
				std::cout << "Detected in img2 " << keypoints2.size() << "\n";
				std::cout << "2NN-Ratio Matches " << matches12ratio.size() << "\n";
				std::cout << "Hough Matches " << houghMatches.size() << "\n";


				std::cout << float(t2 - t1 + t4 - t3) /2.0 / CLOCKS_PER_SEC << "seconds/image to detect points and features\n";
				std::cout << float(t6 - t5) / CLOCKS_PER_SEC << " seconds to compute BF matching with crosscheck\n";
				std::cout << float(t8 - t7) / CLOCKS_PER_SEC << " seconds for 2-NN matching\n";
				std::cout << float(t10 - t9) / CLOCKS_PER_SEC << " seconds for Hough\n";
			}

		}

		vector< KeyPoint > keypoints2e = keypoints2;
		im2e = img2;
		if (scale < 1) {
			for (int n =0; n< keypoints2e.size(); n++) {
				KeyPoint & p = keypoints2e[n];
				p.pt.x *= scale;
				p.pt.y *= scale;
			}
			resize(img2,im2e, Size(), scale, scale, INTER_NEAREST );
		}
//		Scalar color = Scalar::all(-1);
//		int flagsp =DrawMatchesFlags::DRAW_RICH_KEYPOINTS;
		cv::Mat img_matchesnn2;
		cv::Mat img_matchesbf;
		drawMatches(im1e, keypoints1e, im2e, keypoints2e, houghMatches, img_matchesnn2);
		if (! cameraMode ) {
			cimg_library::CImg<unsigned char> cim_nn2( img_matchesnn2);
			cim_nn2.display("Matches_after_Hough",false);
			continue;
		}

		imshow("output_win", img_matchesnn2);
		char c = cv::waitKey(20) ; //Wait 20 mseconds
		if( c == 27 )
			break;
		if( c == ' ' )
			cv::waitKey(0);

	}
	return 0;
}






