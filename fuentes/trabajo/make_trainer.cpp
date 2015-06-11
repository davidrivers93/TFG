#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <map>

#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"
#include "CImg.h"

#if cimg_os==2 //Windows
#include "getopt.h"
#else
#include <unistd.h>
#include <stdlib.h>
#endif

#include "functions.h"
#include "utilities.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

/* MAKE TRAINER - David Rios Benet
 *
 * PROGRAMA QUE DADO UN CONJUNTO DE  EJEMPLOS EN ARCHIVOS .TIF
 * CREA UN VECTOR DE 9 COMPONENTES QUE SERA EL OCR DE UN NUMERO.
 *
 */

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "At least one argument needed\n";
		exit(0);
	}
	std::string input = argv[optind];
	set<string> images = isImages(input);
	if (images.size()) {
		int nimags = images.size();
		std::cout << "Image Files: \n";
		std::set<string>::iterator it;

		for (it = images.begin(); it != images.end(); it++) {
			std::cout << "\t" << *it << " \n";
		}
		std::cout << "\n";
	}
	if (images.size()) {
		std::set<string>::iterator it;
		CImg<float> matriz;
		matriz.resize(9, images.size());
		matriz.fill(0);
		int i = 0;
		for (it = images.begin(); it != images.end(); it++, i++) {
			string imgname = *it;
			string path = "/home/tdi/Desktop/TDI_database/Fotos/CTV/Umbral2/";
			string extension = ".tif";
			CImg<unsigned char> img(imgname.c_str());
			CImg<int> seg = img.get_threshold(128);

			int numobj = 1;
			std::cout << "Numero objetos: " << numobj << "\n";
			CImg<int> bbox;
			BoundingBox_cimg(seg, numobj, bbox);
			char txt[100];
			int interpolation_method = 2;
			for (int o = 1; o <= numobj; o++) {
				CImg<unsigned char> object_img;
				extractObject(seg, bbox, o, 0, object_img);
				CImg<float> lowres = object_img;
				lowres.resize(3, 3, -100, -100, interpolation_method);
				lowres.transpose();
				for (int o = 0; o < lowres.size(); o++) {
					matriz(o, i) = lowres[o];
				}
			}
		}
		matriz.display("Matriz",false);
		CImg<float> salida;
		salida.resize(9);
		salida.fill(0);
		cimg_forX(matriz,x)
		{
			float suma = 0;
			cimg_forY(matriz,y)
			{
				suma += matriz(x, y);
			}
			salida(x, 0) = suma / matriz.height();
		}
		salida.display("Salida",false);
		salida.save_dlm("salida.dlm");

	}
}

