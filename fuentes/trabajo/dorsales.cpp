/* INCLUDES FROM C++ AND EXTERNAL LIBRARIES
 *
 */
#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <stdlib.h>
#define cimg_plugin1 "cimgcvMat.h"
#include "CImg.h"
#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"

#include <zbar.h>
using namespace zbar;

#define CARACTER_SEPARADOR_CSV ";"	// Carácter para separar campos en archivo csv. España = ";" pero en el resto es ",".


/* OWN LIBRARIES
 *
 */

#include "functions.h"
#include "utilities.h"
#include "Qr_proc.h"
#include "database_mng.h"


using namespace cimg_library;
using namespace std;
using namespace cv;


int showfiles(set<string> images);
void calculate(set<string> images, int contador);
void create_txt_file();
void list_races(std::vector < string > & list_races, database_mng & database);
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
	database_mng database;

	const bool help = cimg_option("-h", false, 0);

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

	database.connect();

	create_txt_file();
	std::vector < string > vector_list_races;
	list_races(vector_list_races, database);
	set<string> images = isImages(input);

	int contador = showfiles(images);

	std::cerr << "Datos carrera: " << endl;

	std::cerr << "\t" << "Nombre carrera: " << database.race_data_query.race_data << "\n";
	std::cerr << "\t" << "Fecha carrera: " << database.race_data_query.date_data << endl;

	calculate(images, contador);

	if(contador == 0){
		std::cout << "No hay ninguna imagen a procesar. \n";
		exit(0);
	}

}

void list_races(std::vector < string > & list_races, database_mng & database ){

	string db_races = "prueba";
	database.switchDb(db_races);
    database.execute("SELECT * FROM races");

    while (database.fetch()) {
    	std::cout << " Race " << database.print(1) << " -> " << database.print(2) << " " << database.print(3) << "\n";


    }
    std::cerr << "Elige la carrera: \n";
    string index_race;
    std::cin >> index_race;
    string query_race = "Select * FROM races WHERE idraces= \'" + index_race + "\'";
    database.execute(query_race);
    database.fetch();
	database.race_data_query.race_data = database.print(2);
	database.race_data_query.date_data = database.print(3);

    std::cout << std::endl;
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
    reutnr "Linux";
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

void calculate(set<string> images, int contador){

	CImg<int> contenedor_numobj(contador);
	CImg<int> contenedor_tiempo(contador);
	float totalTime;

	std::vector<std::vector<std::vector<int> > > contenedor_dorsales;

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

			/* Binarizacion adaptativa
			 *
			 * Retorna una imagen binarizada a traves del metodo de binarizacion adaptativa.
			 */
			binarizacion_adaptativa(img, img_out_binarizacion);

			CImg<int> bbox;
			CImg<int> areas;
			CImg<int> seg;
			CImg<float> cdg;

			/* Segmentacion
			 *
			 * Segmenta la imagen binarizada mediante los vecinos. Devuelve la imagen segmentada,
			 * los bbox y las areas de cada objeto.
			 */

			segmentacion(img_out_binarizacion, seg, bbox, areas, cdg);

			//seg.display("Segmentada", false);
			std::vector<int> v_candidates;
			candidates(v_candidates, seg, bbox);

			std::cerr << "Candidatos iniciales: " << v_candidates.size() << endl;

			std::vector<std::vector<int> > comienzos;

			/*busqueda_marcadores
			 *
			 * buscamos parejas de marcadores que cumplan varias condiciones:
			 * 	1-> Los centros esten juntos.
			 * 	2-> los centros de masas sean parecidos.
			 * 	3-> Que el marcador pequeño este dentro del grande.
			 */

			busqueda_marcadores(bbox, comienzos, areas,cdg, v_candidates);
			std::cerr << "Candidatos de comienzos: " << comienzos.size() << endl;
			std::vector<std::vector<int> > comienzos_seleccionados;

			/*Seleccion marcadores
			 *
			 *Filtramos los comienzos anteriores con las siguientes caracteristicas:
			 *	1->Esten centrados
			 *	2->Que el area del bbox no sea 1,2 veces el area real del objeto.
			 */
			seleccion_marcadores(comienzos, comienzos_seleccionados, seg,bbox,areas);

			std::cout << "Tam comienzos seleccionados: " << comienzos_seleccionados.size() << endl;

			std::vector<std::vector<std::vector <int > > > target_marks_index;

			/* Busca trios de marcadores QR respecto de los comienzos seleccionados.
			 *
			 */

			target_marks(comienzos_seleccionados, target_marks_index ,seg, bbox, areas);

			int numobj = bbox.height();
			CImg<int> tabla(numobj);
			tabla.fill(0);

			for (int h = 0; h < target_marks_index.size(); h++) {

				for (int h2 = 3; h2 < target_marks_index[h].size(); h2++) {
					for (int h3 = 0; h3 < target_marks_index[h][h2].size(); h3++) {
						int indice_objeto = target_marks_index[h][h2][h3];
						tabla[indice_objeto] = 1;
					}
				}
			}

			CImg<int> seg2(seg);
			SeleccionarEtiquetas_cimg(seg2, tabla, numobj);
			//seg2.display("A", false);

			std::vector < String > string_result;
			if(target_marks_index.size()!=0){
				qr_processing(img,target_marks_index, bbox, string_result) ;
				if(string_result.size() == 0){
					std::cerr << "No hemos encontrado ningun QR. \n";
				}
			}
			else{
				std::cerr << "No hay candidatos. \n";
			}

			contenedor_dorsales.push_back(vector_nivel_medio);
			clock_t t1 = clock();

			totalTime = t1 - t0;

			totalTime /= CLOCKS_PER_SEC;

			contenedor_tiempo[contador2] = totalTime;

		}
		system("rm -rf temp.jpg");
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
