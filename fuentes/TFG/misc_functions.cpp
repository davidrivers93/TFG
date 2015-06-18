/*
 * misc_functions.cpp
 *
 *  Created on: 18/6/2015
 *      Author: David
 */


#include "database_mng.h"
#include "db_functions.h"
#include "img_functions.h"
#include "misc_functions.h"

#include "CImg.h"
#include <opencv2/opencv.hpp>

using namespace cimg_library;
using namespace std;
using namespace cv;

bool fileExists ( const char * filename){

	FILE *fd=fopen(filename,"r");
	if(fd) {
		fclose(fd);
		return true;

	}
	return false;
//	std::ifstream infile(filename);
//	return infile.good();

}

std::string search_number(std::string result){
	std::string number;
	std::cout << "Tess " << result <<endl;
	std::vector <std::string> vector_posibles;
	bool flag_estado = false;
	int first_position = 0 ;

	for(int i2=0 ; i2<result.length(); i2++){
		if(!isspace(result[i2])) continue;

		std::string temp = result.substr(first_position, i2);
		vector_posibles.push_back(temp);

	}

	std::cout << "Tamaño posibles" << vector_posibles.size() << endl;
	number = vector_posibles[0];

	return number;
}

void create_txt_file() {

	/* create_txt_file
	 * Funcion que crea el .txt de apoyo
	 * DEBERIA DE ESTAR EN PROC/MISC
	 */
	std::string OS = getOsName();
	if (OS == "Linux" || "FreeBSD" || "Mac OSX" || "Unix")
		system("ls *.jpg *.JPG > prueba.txt");
	else
		system("dir *.jpg *.JPG> prueba.txt");

}

std::string getOsName() {
#ifdef _WIN32
	return "Windows 32-bit";
#elif _WIN64
	return "Windows 64-bit";
#elif __unix || __unix__
	return "Unix";
#elif __APPLE__ || __MACH__
	return "Mac OSX";
#elif __linux__
	reutnr "Linux";
#elif __FreeBSD__
	return "FreeBSD";
#else
	return "Other";
#endif
}

int showfiles(std::set<std::string> images){

	int contador = 0;
	if (images.size()) {
		int nimags = images.size();
		//std::cout << "Image Files: \n";
		std::set<std::string>::iterator it;
		std::cout << "Archivos a procesar: \n";

		for (it = images.begin(); it != images.end(); it++, contador++) {
			std::cout << "\t" << *it << " \n";
		}
		std::cout << "\n";
	}

	std::cout << "Numero imagenes a procesar: " << contador << "\n";

	return contador;
}

void process_results(std::vector <int> resultados, std::vector <int> & resultados_norepeat){


	for(int i=0; i<resultados.size(); i++){


		int temp=resultados[i];

		if(i==0) resultados_norepeat.push_back(temp);
		else{
			for(int i2=0; i2<resultados_norepeat.size(); i2++){

				int temp2=resultados_norepeat[i2];
				if(temp==temp2) break;
				if(i2==resultados_norepeat.size()-1) resultados_norepeat.push_back(temp2);

			}

		}

	}




}
