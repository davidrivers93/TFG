/*
 *  main_prueba_deteccion_objectos.cpp
 *  ProjectXcode
 *
 *  Created by Alberto Albiol on 6/17/10.
 *  Copyright 2010 Universidad Politecnica Valencia. All rights reserved.
 *
 */
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "cv.h"

#define cimg_plugin1 "cimgcvMat.h"
#include "CImg.h"

#include "smartdisplay3.h"
using namespace cimg_library;
using namespace cv;

void drawMotion(CImg<unsigned char> & curr, int step, const CImg<float> & motion);
void motion2color(const CImg<float> & cimg_motion, CImg<unsigned char> & ucharMotion) ;


int main(int argc,char *argv[])
{

	smartdisplay3<unsigned char>  colorDisplay;
	smartdisplay3<unsigned char>  vectorDisplay;

	VideoCapture cap;


	char texto[300 ];
	void *handle_in = 0;


	const char *input =   cimg_option("-i", (char*)0, "Input  VideoFilename or Cam number.");
	const int levels = cimg_option("-levels", 4, "Number of pyramid layers");
	const int winsize = cimg_option("-winsize", 11, "Window Size.");
	const int stepDraw = cimg_option("-stepDraw", 12, "Step for drawing vectors");
	const double capt_width = cimg_option("-W", 320, "Capture Width");
	const double capt_height = cimg_option("-H", 240, "Capture Height");

	int exitFlag = 0;
	if( 0 == input)
	{
		std::cerr<<"An input must be specified in commmand line using -i option\n";
		exit(0);
	}
	if(   strlen(input) == 1 && isdigit(input[0]) ) {
		cap.open( int( input[0] - '0') );
		cap.set(CV_CAP_PROP_FRAME_WIDTH , capt_width);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, capt_height);
	}
	else
		cap.open(input);


	if( !cap.isOpened() )
		return -1;




	if(exitFlag)
		exit(0);





	cimg_library::CImg<unsigned char> curr;
	cimg_library::CImg<unsigned char> prev;

	double pyrScale = 0.5; //size reduction factor between pyramid layers
	int polyN = 5; //or 7
	int iterations = 2;

	//Values from opencv documentation
	double polySigma;
	if( polyN == 5)
		polySigma = 1.1;
	else if(polyN == 7)
		polySigma = 1.5;
	else
	{
		std::cerr << "Invalid polyN value. Must be 5 or 7\n";
		exit(0);
	}
	int flags = 0;

	CImg<unsigned char> ucharMotion;
	Mat prevgray, gray, flowmat, cflow, frame;
	clock_t t_total = 0;
	int k;
	for( k = 0 ; 1 ; k++)
	{
		std::swap(prevgray, gray);

		cap >> frame;
		if( frame.empty() )
			break;
		cv::cvtColor(frame, gray, CV_BGR2GRAY);


		if (k == 0)
			continue;


		clock_t t1 = clock();
		cv::calcOpticalFlowFarneback(prevgray, gray, flowmat, pyrScale, levels, winsize,  iterations,  polyN,  polySigma, flags);
		clock_t t2 = clock();

		t_total += (t2-t1) ;
		//const Point2f& fxy = flowmat.at<Point2f>(y, x);
		CImg<float> cimg_motion( (float  *)flowmat.data, 2, flowmat.cols, flowmat.rows, 1, true); //Sin copia
		//Componente X del vector de movimiento en el pixel (x,y): cimg_motion(0,x,y);
		//Componente Y del vector de movimiento en el pixel (x,y): cimg_motion(1,x,y);

		motion2color( cimg_motion, ucharMotion);

		CImg<unsigned char> curr(frame);

		drawMotion(curr, stepDraw, cimg_motion);


		colorDisplay.display( ucharMotion ,"Dense Motion");
		vectorDisplay.display( curr ,"Vector Motion");
		if( k%100 ==99){
			float ips = float(k) / (t_total / float(CLOCKS_PER_SEC));
			std::cout << "IPS = " << ips << "images/sec\n";
			std::cout << "Tproc = " << 1000/ips << "msec/img\n\n";
		}

	}


	return 0;
}

void motion2color(const CImg<float> & cimg_motion, CImg<unsigned char> & ucharMotion) {

	ucharMotion.assign(cimg_motion.height() , cimg_motion.depth() , 1 , 3 ).fill(128);

	cimg_forXY(ucharMotion , x , y)
	{
		float motx = cimg_motion(0,x,y) * 8.0;
		float moty = cimg_motion(1,x,y) * 8.0;
		int intmotx = ucharMotion(x,y,0,0) + motx;
		if (intmotx < 0)
			intmotx = 0;
		if (intmotx>255)
			intmotx = 255;

		int intmoty = ucharMotion(x,y,0,1) + moty;
		if (intmoty < 0)
			intmoty = 0;
		if (intmoty>255)
			intmoty = 255;
		ucharMotion(x,y,0,0) = intmotx;
		ucharMotion(x,y,0,2) = intmotx;
		ucharMotion(x,y,0,1) = intmoty;
	}
}


void drawMotion(CImg<unsigned char> & curr, int step, const CImg<float> & motion)
{
	int offset = step / 2;
	unsigned char white[ ]= {255, 255, 255};
	unsigned char yellow[ ]= {255, 255, 0};
	unsigned char red[ ]= {255, 0, 0};
	unsigned char green[ ]= {0, 255, 0};
	unsigned char black[ ]= {0, 0, 0};
	for(int x = offset; x < curr.width()-offset; x+= step)
	{
		for(int y = offset; y < curr.height()-offset; y+= step)
		{
			int mx = motion(0,x,y);
			int my = motion(1,x,y);
			if(mx == 0 && my ==0)
				curr.draw_circle(x,y,1,red );
			else {
				curr.draw_circle(x,y,2,green );
				int endx = x + 2* motion(0,x,y);
				int endy = y + 2*motion(1,x,y);
				curr.draw_line(x , y , endx, endy, white);
				curr.draw_point(x, y,black);

			}
		}

	}

}


