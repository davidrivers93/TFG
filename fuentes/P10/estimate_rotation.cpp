// This programs should take as input a binary image
// and estimate its main orientation



// TO BE COMPLETED in the course
// Antonio Albiol, Jan 2013

#include <vector>
#include <iostream>
#include <CImg.h>
#include "P8/segmentutils.h"
using namespace cimg_library;

int main(int argc, char **argv) 
{

	if( argc < 2 ){
		std::cout << "At least one argument needed\n";
		exit(0);
	}
//Read input image
CImg<unsigned char> input(argv[1]);

//Make sure it's binary
CImg<int> seg = input.get_threshold(128);

// Solo se considera un único objeto

// Calcular momentos

// Estimar orientacion a partir de matriz de covarianza

// Sacar por pantalla ángulo de rotación

// Rotar Imagen para alinearla

// Mostrar imagen con rotación compensada


}
