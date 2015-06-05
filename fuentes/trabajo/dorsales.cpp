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
#include <sstream>
#include <string>

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
void list_races(std::vector<string> & list_races, database_mng & database);
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
	std::cout << "\t \t 0 = OCR\n";
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

	std::string input = "prueba.txt"; //argv[optind];

	//Si optind==0 salta ayuda
	if (help) {
		ayuda();
		std::cout << "¿Quieres seguir utilizando el programa? SI/NO .\n";
		string opt;
		cin >> opt;
		if (opt == "NO")
			exit(0);
	}

	database.connect();

	create_txt_file();
	std::vector<string> vector_list_races;
	list_races(vector_list_races, database);
	set<string> images = isImages(input);

	int contador = showfiles(images);

	std::cerr << "Datos carrera: " << endl;

	std::cerr << "\t" << "Nombre carrera: " << database.race_data_query.race_data << "\n";
	std::cerr << "\t" << "Fecha carrera: " << database.race_data_query.date_data << endl;

	calculate(images, contador);

	if (contador == 0) {
		std::cout << "No hay ninguna imagen a procesar. \n";
		exit(0);
	}

}

void list_races(std::vector<string> & list_races, database_mng & database) {

	string db_races = "prueba";
	database.switchDb(db_races);
	database.database_name = db_races;
	database.execute("SELECT * FROM races");
	int index_bbdd_races = 0;
	while (database.fetch()) {
		std::cout << " Race " << database.print(1) << " -> " << database.print(2) << " " << database.print(3) << " " << database.print(4) << "\n";
		index_bbdd_races++;
	}
	std::cerr << "En el caso de que la carrera no este metida en la base de datos escribe 0." << endl;
	std::cerr << "Elige la carrera: \n";
	std::cerr << "El indice es " << index_bbdd_races << endl;
	string index_race;
	std::cin >> index_race;
	if(index_race == "0"){

		std::cout << "Introduce el nombre de la carrera : ";
		string new_race_name;
		std::cin >> new_race_name;
		database.race_data_query.race_data = new_race_name;
		std::cerr << "Introduce la fecha de la carrera (formato dd-mm-aa 31-07-15 ) : ";
		string new_race_date;
		std::cin >> new_race_date;
		database.race_data_query.race_data = new_race_date;
		index_bbdd_races++;

		stringstream new_race_table;
		new_race_table << new_race_date.at(0) << new_race_date.at(1) << new_race_name.at(0) << new_race_name.at(1) << new_race_date.at(6) << new_race_date.at(7);
		string new_race_tables = new_race_table.str();
		database.prepare("INSERT INTO prueba.races (idraces, races_name, races_date, race_dbname) VALUES (?, ?, ?, ?)");
		database.setInt(1,index_bbdd_races);
		database.setString(2,new_race_name);
		database.setString(3,new_race_date);
		database.setString(4,new_race_tables);

		//actualizo struct
		database.race_data_query.race_data = new_race_name;
		database.race_data_query.date_data =  new_race_date;
		database.race_data_query.tablen_data = new_race_tables;

		database.execute();


	}
	else{

		string query_race = "Select * FROM races WHERE idraces= \'" + index_race + "\'";
		database.execute(query_race);
		database.fetch();
		database.race_data_query.race_data = database.print(2);
		database.race_data_query.date_data = database.print(3);
		database.race_data_query.tablen_data = database.print(4);

		//std::cout << new_race_date.at(0) << new_race_date.at(1) << new_race_name.at(0) << new_race_name.at(1) << new_race_date.at(6) << new_race_date.at(7);
	}

	//Compruebo si existe la tabla de la carrera

	database.prepare("SELECT * FROM ?");
	database.setString(1, database.race_data_query.tablen_data);
	database.execute();

	std::cout << "Valor del flag " << database.flag_created << endl;
	//CREAMOS LA TABLA
	/*if(database.flag_created){

		database.prepare("CREATE TABLE ?.? (`dorsal` INT NOT NULL,`path_img` VARCHAR(45) NOT NULL, PRIMARY KEY (`dorsal` , `path_img`)");
		database.setString(1, db_races);
		database.setString(2, database.race_data_query.tablen_data);
		database.execute();
		std::cout << "Tabla creada." << endl;
	}*/
	//td::cout << endl;
}

int showfiles(set<string> images) {

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

void create_txt_file() {

	/* create_txt_file
	 * Funcion que crea el .txt de apoyo
	 * DEBERIA DE ESTAR EN PROC/MISC
	 */
	std::string OS = getOsName();
	if (OS == "Linux" || "FreeBSD" || "Mac OSX" || "Unix")
		system("ls *.jpg > prueba.txt");
	else
		system("dir *.jpg> prueba.txt");

}

void calculate(set<string> images, int contador) {

	CImg<int> contenedor_numobj(contador);
	CImg<int> contenedor_tiempo(contador);
	float totalTime;

	std::vector<std::vector<std::vector<int> > > contenedor_dorsales;

	std::vector<std::vector<int> > vector_nivel_medio;

	if (images.size()) {
		std::set<string>::iterator it;
		int contador2 = 0;
		int index = 1;
		for (it = images.begin(); it != images.end(); it++, contador2++, index++) {
			clock_t t0 = clock();
			vector_nivel_medio.clear();
			string imgname = *it;
			CImg<unsigned char> img(imgname.c_str());
			CImg<unsigned char> img_out_binarizacion = img;
			img.display("Imagen de entrada", false);
			std::cout << "Procesando imagen numero " << index << endl;

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
			std::cerr << "Entro a segmentar" << endl;
			segmentacion(img_out_binarizacion, seg, bbox, areas, cdg);
			std::cerr << "Salgo a segmentar" << endl;
			seg.display("Segmentada", false);
			std::vector<int> v_candidates;
			std::cerr << "Entro a candidates" << endl;
			candidates(v_candidates, seg, bbox);
			std::cerr << "Salgo de candidates" << endl;
			std::cerr << "Candidatos iniciales: " << v_candidates.size() << endl;

			std::vector<std::vector<int> > comienzos;

			/*busqueda_marcadores
			 *
			 * buscamos parejas de marcadores que cumplan varias condiciones:
			 * 	1-> Los centros esten juntos.
			 * 	2-> los centros de masas sean parecidos.
			 * 	3-> Que el marcador pequeño este dentro del grande.
			 */
			std::cerr << "Busco marcadores" << endl;
			busqueda_marcadores(bbox, comienzos, areas, cdg, v_candidates);
			std::cerr << "Salgo de buscar" << endl;
			std::cerr << "Candidatos de comienzos: " << comienzos.size() << endl;
			std::vector<std::vector<int> > comienzos_seleccionados;

			/*Seleccion marcadores
			 *
			 *Filtramos los comienzos anteriores con las siguientes caracteristicas:
			 *	1->Esten centrados
			 *	2->Que el area del bbox no sea 1,2 veces el area real del objeto.
			 */
			std::cerr << "Entro a seleccion" << endl;
			seleccion_marcadores(comienzos, comienzos_seleccionados, seg, bbox, areas);
			std::cerr << "Salgo de seleccion" << endl;
			std::cout << "Tam comienzos seleccionados: " << comienzos_seleccionados.size() << endl;

			std::vector<std::vector<std::vector<int> > > target_marks_index;

			/* Busca trios de marcadores QR respecto de los comienzos seleccionados.
			 *
			 */
			std::cerr << "Entro a target" << endl;
			target_marks(comienzos_seleccionados, target_marks_index, seg, bbox, areas);
			std::cerr << "Salgo de target" << endl;

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
			seg2.display("A", false);
			std::cerr << "Entro a qr_processing." << endl;
			std::vector<String> string_result;
			if (target_marks_index.size() != 0) {
				qr_processing(img, seg2, target_marks_index, bbox, string_result);
				if (string_result.size() == 0) {
					std::cerr << "No hemos encontrado ningun QR. \n";
				}
			} else {
				std::cerr << "No hay candidatos. \n";
			}
			std::cerr << "Salgo de qr_processing." << endl;

		}
		system("rm -rf temp.jpg");

	}
}
