#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <time.h>

#include </opt/local/include/tesseract/baseapi.h>
#include </opt/local/include/leptonica/allheaders.h>

#if cimg_os==2 //Windows
#include "getopt.h"
#else
#include <unistd.h>
#include <stdlib.h>
#endif

#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"
#include "CImg.h"

/* OWN LIBRARIES
 *
 */

#include "functions.h"
#include "utilities.h"
#include "opencv.h"
#include "proc/misc.h"
#include "proc/proc.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

/*	function: router
 *
 * 	funcion que rutea todas las llamadas y centraliza todo.
 *
 */

void router(set<string> images, int modo){

//RECIBIMOS LA LISTA DE IMAGENES Y EL MODO!

	int proc_size = images.size();
	std::cout << "Comprobamos que el size es igual a antes: " << proc_size;

	std::set<string>::iterator it;

	int contador2=0;

	for (it = images.begin(); it != images.end(); it++, contador2++) {

	}

}

/* function: thread_creator
 *
 * Crea los hilos para procesar las imagenes.
 * 		hilo 1->principal
 * 		hilo 2->OCR
 * 		hilo 3->tesseract
 */

void thread_creator(){



}

/*	function: proc_OCR
 *
 * 	funcion que realiza el procesado OCR en hilo 2
 *
 */

void proc_OCR(){



}

/*	function: proc_OCR
 *
 * 	funcion que realiza el procesado tesseract en hilo 3
 *
 */

void proc_Tesseract(){



}

void load_dlm(){

}
