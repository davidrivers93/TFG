#ifndef PROC_H_
#define PROC_H_

#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <time.h>

#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"
#include "CImg.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

#define HEIGHT_RATIO_TOLERANCE 1.5
#define MAX_HEIGHT 30
#define MIN_HEIGHT 12
#define RATIO_TOLERANCE 1.8
#define TYPICAL_RATIO 1.2
#define WIDTH_RATIO_TOLERANCE 1.7

void router(set<string> images, int modo);
void proc_general(CImg<unsigned char> img_loaded);
void show_parejas(std::vector<std::vector<int> > comienzos);
void binarizacion_adaptativa(const cimg_library::CImg<unsigned char> & input, cimg_library::CImg<unsigned char> & out_bin);
bool busqueda_derecha(int o1, int o2, const cimg_library::CImg<int> & bbox) ;
bool busqueda_izquierda(int o1, int o2, const cimg_library::CImg<int> & bbox) ;
void busqueda(const cimg_library::CImg<int> & bbox, std::vector<std::vector<int> > & comienzos) ;
void seleccion_comienzos(std::vector<std::vector<int> > & comienzos, std::vector<std::vector<int> > & comienzos_seleccionados, cimg_library::CImg<int> & seg,
		cimg_library::CImg<int> & bbox, cimg_library::CImg<int> & areas);
void busqueda_tercera_cifra(std::vector<std::vector<int> > & comienzos_seleccionados, cimg_library::CImg<int> & bbox);
int SeleccionarEtiquetas_cimg(CImg<int> & segment, CImg<int> & tabla, int & numobj_out) ;



#endif
