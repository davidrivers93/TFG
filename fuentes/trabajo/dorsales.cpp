/* INCLUDES FROM C++ AND EXTERNAL LIBRARIES
 *
 */
#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <time.h>

#ifdef _WIN32 //Windows
#include "getopt.h"
#include "E:\opencv\opencv\build\include\opencv\opencv.hpp"
#else
#include <unistd.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#endif
#include "CImg.h"
#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"


/* OWN LIBRARIES
 *
 */

#include "functions.h"
#include "utilities.h"
#include "proc/misc.h"
#include "proc/proc.h"

using namespace cimg_library;
using namespace std;
using namespace cv;



int showfiles(set<string> images);
void calculate(set<string> images, int contador, int modo);
void create_txt_file();
std::string getOsName();

/* DORSALES.CPP - David Rios Benet
 * PROGRAMA PRINCIPAL DEL PROYECTO.
 * REALIZA LAS LLAMADAS A LAS FUNCIONES ASI COMO LOS DISPLAYS Y LA CREACION DEL .CSV CON LOS RESULTADOS.
 */

/* FUNCION : Ayuda
 *
 * Nos devolvera una serie de mensajes de ayuda para comprender la funcion del programa asi como
 * sus posibles opciones
 *
 */
void ayuda() {
	std::cout << "Introduce en la linea de comandos junto a dorsales el nombre del .txt\n";

	std::cout << "Opciones: \n";
	std::cout << " \t opcion -h : Ayud \n";
	std::cout << " \t opcion -m : Modo de ejecucion -> \n";
	std::cout<< "\t \t 0 = OCR\n";
	std::cout << " \t \t 1 = tesseract \n";
	std::cout << " \t \t 2 = OCR & tesseract \n";
}

int main(int argc, char **argv) {

	int opt;
	double capt_width = 320;
	double capt_height = 240;
	CImgDisplay disp_cimg;

	/*while ((opt = getopt(argc, argv, "hW:H:")) != -1) { //
	 switch (opt) {
	 case 'h':
	 ayuda();
	 exit(0);
	 break;
	 case 'W':
	 capt_width = atoi(optarg);
	 break;
	 case 'H':
	 capt_height = atoi(optarg);
	 break;
	 case 'i':
	 std::string input = argv[optind];
	 break;
	 }
	 }*/

	const int modo = cimg_option("-m", false, 0);
	const bool help = cimg_option("-h", false, 0);

	if (modo==0)
		std::cout << "El modo de ejecucion es OCR. \n";
	if(modo==1)
		std::cout << "El modo de ejecucion es tesseract. \n";
	if(modo==2)
		std::cout << "Se ejectuaran ambos modos. \n";

	std::string input = "prueba.txt";//argv[optind];

	//Si optind==0 salta ayuda
	if (help) {
	 ayuda();
	 std::cout << "¿Quieres seguir utilizando el programa? SI/NO .\n";
	 string opt;
	 cin >> opt;
	 if(opt == "NO")
		 exit(0);
	 }


	//Creamos el txt file que nos parsea las fotos de la carpeta
	create_txt_file();
	// ==== Determine kind of input ===========
	//int camera_number = isCamera(input);
	// camera == -1 if it is not a camera
	//bool is_Video = isVideo(input);

	set<string> images = isImages(input);

	/*if (camera_number >= 0)
		std::cout << "It is Camera number :" << camera_number << "\n";

	if (is_Video)
		std::cout << "It is VideoFile :" << input << "\n";*/
	int contador = showfiles(images);

	calculate(images, contador, modo);

	if(contador == 0){
		std::cout << "No hay ninguna imagen a procesar. \n";
		exit(0);
	}

	//LLAMARIAMOS AL ROUTER DE PROC/PROC QUE RECIBE LA LISTA DE IMAGENES
	//router();

}

int showfiles(set<string> images){

	int contador = 0;
	if (images.size()) {
		int nimags = images.size();
		//std::cout << "Image Files: \n";
		std::set<string>::iterator it;
		std::cout << "Archivos a procesar: \n";

		for (it = images.begin(); it != images.end(); it++, contador++) {
			std::cout << "\t" << *it << " \n";
		}
		std::cout << "\n";
	}

	std::cout << "Numero imagenes a procesar: " << contador << "\n";

	return contador;
}

std::string getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __unix || __unix__
    return "Unix";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #else
    return "Other";
    #endif
}

void create_txt_file(){

	/* create_txt_file
	 * Funcion que crea el .txt de apoyo
	 * DEBERIA DE ESTAR EN PROC/MISC
	 */
	std::string OS = getOsName();
	if(OS == "Linux" || "FreeBSD" || "Mac OSX" || "Unix")
		system("ls *.jpg > prueba.txt");
	else
		system("dir *.jpg> prueba.txt");


}

void calculate(set<string> images, int contador, int modo){

	CImg<int> contenedor_numobj(contador);
	CImg<int> contenedor_tiempo(contador);
	float totalTime;

	std::vector<std::vector<std::vector<int> > > contenedor_dorsales;

	/* ZONA CARGA OCR
	 *
	 * Recordar que OCR es una matriz de 9 filas y 10 columnas.
	 *
	 * col 0 -> matriz 3x3 del numero 0.
	 */

	// CARGAMOS OCR -> REPASARRRRRR!!!

	CImg<float> vectores;
	vectores.resize(9, 10);
	load_dlm(vectores);
	vectores.display("OCR", false);
	std::cout << "DLM cargado. \n";

	std::vector<std::vector<int> > vector_nivel_medio;

	if (images.size()) {
		std::set<string>::iterator it;
		int contador2 = 0;

		for (it = images.begin(); it != images.end(); it++, contador2++) {
			clock_t t0 = clock();
			vector_nivel_medio.clear();
			string imgname = *it;
			CImg<unsigned char> img(imgname.c_str());
			CImg<unsigned char> img_out_binarizacion = img;

			//Realizamos un display de la imagen de entrada.
			//img.display("Entrada", false);

			//router(it, modo);

			//ZONA DE SEGMENTACION Y BINARIZACION

			binarizacion_adaptativa(img, img_out_binarizacion);

			/*img_out_binarizacion es la imagen binarizada
			 * bbox -> Rectangulo que contiene al objeto i
			 * areas -> Estima el area del objeto gracias al bounding box
			 * seg -> imagen segmentada
			 */
			CImg<int> bbox;
			CImg<int> areas;
			CImg<int> seg;

			segmentacion(img_out_binarizacion, seg, bbox, areas);

			//seg.display("Segmentada", false);

			//ZONA DE DETECCION

			std::vector<std::vector<int> > comienzos;
			/* Comenzamos la zona de deteccion invocando la funcion busqueda
			 * la cual le pasamos los bbox asi como el vector de comienzos.
			 */

			busqueda(bbox, comienzos);

			std::vector<std::vector<int> > comienzos_seleccionados;

			//SACAMOS POR PANTALLA LAS PAREJAS DE COMIENZOS DE DORSALES

			std::cout << "Comienzos.\n";

			for (int h = 0; h < comienzos.size(); h++) {
				for (int h2 = 0; h2 < comienzos[h].size(); h2++) {
					if (h2 == 0)
						std::cout << "Pareja: " << comienzos[h][h2];
					if (h2 != 0)
						std::cout << " " << comienzos[h][h2];
				}
				std::cout << "\n";
			}

			seleccion_comienzos(comienzos, comienzos_seleccionados, seg, bbox,
					areas);

			//SACAMOS POR PANTALLA LAS PAREJAS DE DORSALES SELECCIONADAS PARA PROCESAR
			std::cout << "Comienzos seleccionados.\n";
			for (int h = 0; h < comienzos_seleccionados.size(); h++) {
				for (int h2 = 0; h2 < comienzos_seleccionados[h].size(); h2++) {
					if (h2 == 0)
						std::cout << "Pareja: "
								<< comienzos_seleccionados[h][h2];
					if (h2 != 0)
						std::cout << " " << comienzos_seleccionados[h][h2];
				}
				std::cout << "\n";
			}

			//BUSCAMOS UNA POSIBLE TERCERA CIFRA PARA CADA PAREJA DE DORSAL SELECCIONADA
			busqueda_tercera_cifra(comienzos_seleccionados, bbox);

			std::cout << "Comienzos seleccionados con tercera cifra.\n";
			int numobj = bbox.height();
			CImg<int> tabla(numobj);
			tabla.fill(0);

			for (int h = 0; h < comienzos_seleccionados.size(); h++) {
				for (int h2 = 0; h2 < comienzos_seleccionados[h].size(); h2++) {
					int indice_objeto = comienzos_seleccionados[h][h2];
					tabla[indice_objeto] = 1;
				}
				std::cout << "\n";
			}
			CImg<int> seg2(seg);
			SeleccionarEtiquetas_cimg(seg2, tabla, numobj);
			//seg2.display("A", false);

			seg2.save("temp.jpg");
			std::cout << "Imagen guardada temporalmente.";

			//SACAMOS POR PANTALLA LOS COMIENZOS SELECCIONADOS FINALES
			for (int h = 0; h < comienzos_seleccionados.size(); h++) {
				for (int h2 = 0; h2 < comienzos_seleccionados[h].size(); h2++) {
					if (h2 == 0)
						std::cout << "Pareja: "
								<< comienzos_seleccionados[h][h2];
					if (h2 != 0)
						std::cout << " " << comienzos_seleccionados[h][h2];
				}
				std::cout << "\n";
			}

			char txt[100];
			int interpolation_method = 2;

			//ZONA DE DETECCION DE CADA DIGITO POR PARTE DEL OCR
			for (int puntero_OCR1 = 0;
					puntero_OCR1 < comienzos_seleccionados.size();
					puntero_OCR1++) {

				std::vector<int> vector_bajo_nivel;

				for (int puntero_OCR2 = 0;
						puntero_OCR2
								< comienzos_seleccionados[puntero_OCR1].size();
						puntero_OCR2++) {

					int indice_objeto =
							comienzos_seleccionados[puntero_OCR1][puntero_OCR2];

					CImg<unsigned char> objeto_extraido;
					extractObject(seg, bbox, indice_objeto, 0, objeto_extraido);
					sprintf(txt, "Object %d", indice_objeto);
					//objeto_extraido.display("object", false);
					CImg<float> lowres = objeto_extraido;
					lowres.resize(3, 3, -100, -100, interpolation_method);
					//lowres.display("Lowres",false);
					lowres.transpose();

					int digito = OCR(vectores, lowres);
					std::cout << "Digito detectado: " << digito << "\n";

					vector_bajo_nivel.push_back(digito);

				}

				vector_nivel_medio.push_back(vector_bajo_nivel);
			}

			//Zona de deteccion por parte de tesseract

			//IMPLEMENTAR IMAGENES POR SEPARADO COGIENDO EN CADA IMAGEN RISTRAS DE 2 O 3 OBJETOS. HACERRRR


			char *outText;

			/*seg2.display("Imagen a enviar a Tesseract", false);
			tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
			// Initialize tesseract-ocr with English, without specifying tessdata path
			if (api->Init(NULL, "eng")) {
				fprintf(stderr, "Could not initialize tesseract.\n");
				exit(1);
			}

			// Open input image with leptonica library
			Pix *image = pixRead("temp.jpg");
			//pixDisplay(image, 100, 100);
			api->SetImage(image);
			// Get OCR result
			outText = api->GetUTF8Text();
			//std::cout << "Output OCR" + outText + "\n";
			if (outText == NULL) {
				std::cout << "No detecta nada. \n";
			}
			printf("OCR output:\n%s", outText);

			// Destroy used object and release memory
			api->End();
			//delete [] outText;
			pixDestroy(&image);*/

			//seg2.display("A", false);

			contenedor_dorsales.push_back(vector_nivel_medio);
			clock_t t1 = clock();

			totalTime = t1 - t0;

			//Dividimos por clocks_per_sec para sacar el tiempo real y no el de iteraciones!

			totalTime /= CLOCKS_PER_SEC;

			//guardamos el tiempo en su puntero correcto.
			contenedor_tiempo[contador2] = totalTime;

		}

		//xml_write(&contenedor_dorsales, &contenedor_tiempo, &contenedor_numobj);
		//ZONA DE GUARDADO DE LOS RESULTADOS.

		//HAY QUE QUITARLA........ SUSTITUIMOS POR ESCRITURA EN XML.

		//PEDIR A BELEN METODO ESCRITURA EN XML!!!
		std::cout << "Tamano contenedor:" << contenedor_dorsales.size() << "\n";

		FILE *fd = 0;
		fd = fopen("resultado.csv", "w");
		if (!fd) {
			std::cerr << "Can't open resultado.csv for writing\n";
			exit(0);
		}

		fprintf(fd, "Label , ");
		fprintf(fd, "imagen ");
		fprintf(fd, "Numero OCR");
		fprintf(fd, "Tiempo ");
		fprintf(fd, "numobj");
		fprintf(fd, "Dorsal ");
		fprintf(fd, "\n");
		for (int e = 0; e < contenedor_dorsales.size(); e++) {
			fprintf(fd, "%d ", e); // Label
			if (contenedor_dorsales[e].size() == 0) {
				fprintf(fd, " "); // Numero
				fprintf(fd, "%i ", contenedor_tiempo[e]); // Tiempo
				fprintf(fd, "%i ", contenedor_numobj[e]); // Label
				fprintf(fd, "\n");
				continue;
			}
			for (int e2 = 0; e2 < contenedor_dorsales[e].size(); e2++) {
				fprintf(fd, "%d ", e2); // Numero
				fprintf(fd, "%i ", contenedor_tiempo[e]); // Tiempo
				fprintf(fd, "%i ", contenedor_numobj[e]); // Label
				for (int e3 = 0; e3 < contenedor_dorsales[e][e2].size(); e3++) {
					fprintf(fd, "%i ", contenedor_dorsales[e][e2][e3]); // Label
				}
			}
			fprintf(fd, "\n");
		}
		if (fd)
			fclose(fd);

		std::cout << "Archivo de resultados guardado. \n";

	}
}
