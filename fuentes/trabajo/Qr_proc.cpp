/*
 * Qr_proc.cpp
 *
 *  Created on: 25/5/2015
 *      Author: David
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

//Constantes identificacion

const int CV_QR_NORTE=0;
const int CV_QR_ESTE=1;
const int CV_QR_SUR=2;
const int CV_QR_OESTE=3;

//recibe una imagen en cv
int main(int argc, char **argv){
	//Esto sobra cuando pasemos la imagen directamente
	Mat image = imread(argv[1]);

	//Creamos imagenes intermedias
	Mat gray(image.size(), CV_MAKETYPE(image.depth(),1));
	Mat edges(image.size(), CV_MAKETYPE(image.depth(),1));
	//CV_8UC3->Unsigned int 8 bit long
	Mat traces(image.size(), CV_8UC3);
	Mat qr,qr_raw,qr_gray,qr_thres;

	traces = Scalar(0,0,0);
	qr_raw = Mat::zeros(100,100,CV_8UC3);
	qr = qr_raw;
	qr_gray = Mat::zeros(100,100,CV_8UC1);
	qr_thres = qr_gray;



}
