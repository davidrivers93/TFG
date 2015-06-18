/*
 * binarizador.cpp
 *
 *  Created on: 17/6/2015
 *      Author: David
 */

#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <stdlib.h>

#define cimg_plugin1 "cimgcvMat.h"
#include "CImg.h"
#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"
#include <sstream>
#include <string>

#include "functions.h"
#include "utilities.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

int main(int argc, char **argv) {

	system("ls *.jpg *.JPG > prueba.txt");
	std::string input = "prueba.txt";
	set<string> images = isImages(input);

	if (images.size()) {
		int nimags = images.size();
		//std::cout << "Image Files: \n";
		std::set<string>::iterator it;
		std::cout << "Archivos a procesar: \n";

		for (it = images.begin(); it != images.end(); it++) {
			std::cout << "\t" << *it << " \n";
		}
		std::cout << "\n";
	}

	std::set<string>::iterator it;
	for (it = images.begin(); it != images.end(); it++) {
		string imgname = *it;
		CImg<unsigned char> img(imgname.c_str());

		CImg<unsigned char> img_out_binarizacion(img.width(),img.height(),1,1,0);

		img_out_binarizacion = img.get_RGBtoYCbCr().get_channel(0).get_threshold(128);

		//binarizacion_adaptativa(img, img_out_binarizacion);

		string save = "bin_" + imgname;
		//img_out_binarizacion.display();
		(255*img_out_binarizacion).save(save.c_str());

	}


}
