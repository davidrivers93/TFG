

/*
 * dorsal2.cpp
 *
 *  Created on: 9/6/2015
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

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <zbar.h>
using namespace zbar;

#define CARACTER_SEPARADOR_CSV ";"	// Carácter para separar campos en archivo csv. España = ";" pero en el resto es ",".

#include "utilities.h"
#include "database_mng.h"
#include "db_functions.h"
#include "img_functions.h"
#include "misc_functions.h"
#include "functions.h"


using namespace cimg_library;
using namespace std;
using namespace cv;

void calculate(set<string> images, int contador,database_mng & database );


int main(int argc, char **argv) {

	int opt;

	CImgDisplay disp_cimg;

	const bool help = cimg_option("-h", false, 0);

	database_mng database;


	std::string input = "prueba.txt"; //argv[optind];

	//Si optind==0 salta ayuda
	if (help) {
		//ayuda();
		std::cout << "¿Quieres seguir utilizando el programa? SI/NO .\n";
		string opt;
		cin >> opt;
		if (opt == "NO")
			exit(0);
	}

	//database.connect();

	create_txt_file();

	std::vector<string> vector_list_races;
	//list_races(vector_list_races, database);
	set<string> images = isImages(input);
	int contador = showfiles(images);
	if(images.size()){
		calculate(images, contador,database);
	}
	else {
		std::cout << "No hay ninguna imagen a procesar. \n";
		exit(0);
	}
	std::cout << "He llegado al final "  << endl;
	return 0;

}


