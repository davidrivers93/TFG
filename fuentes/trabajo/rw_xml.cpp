#include <iostream>
#include <string>
#include "pugixml.cpp"
#include "pugixml.hpp"
#include "xmlwriter.h"
#include "CImg.h"

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace cimg_library;

struct now{


};


int xml_read() {

	pugi::xml_document doc_load;

	if (!doc_load.load_file("mapa.tmx")) {
		std::cerr << "Error al cargar el documento XML." << std::endl;
		return -1;
	}

	pugi::xml_node root_node;

	if (!(root_node = doc_load.child("dorsales"))) {
		std::cerr << "El documento no es un mapa valido." << std::endl;
		return -2;
	}

	float version_firmware;
	std::string filename_txt, filename_img;
	int numero, width, height, result_OCR, result_tesseract;

	//Obtenemos los datos del nodo dorsales
	version_firmware = root_node.attribute("version_firmware").as_float();
	filename_txt = root_node.attribute("filename").value();

	//mostramos los datos obtenidos

	std::cout << "Dorsales\n------\n";
	std::cout << "Version del firmware: " << version_firmware << std::endl;
	std::cout << "Fichero de text fuente: " << filename_txt << std::endl;

	//Recorremos los nodos hijo

	for (pugi::xml_node node = root_node.first_child(); node;
			node = node.next_sibling()) {
		std::string node_name = node.name();
		// Actuamos en consecuencia segun el tipo de nodo
		if (node_name == "image") {
			unsigned int firstgid;
			firstgid = node.attribute("firstgid").as_uint();
			numero = node.attribute("number").as_uint();
			filename_img = node.attribute("filename").value();
			width = node.attribute("width").as_uint();
			height= node.attribute("height").as_uint();
			// Obtenemos el nodo hijo image si existe
			if (pugi::xml_node node_image = node.child("OCR")) {
				result_OCR = node.attribute("result").as_uint();
			}
			if (pugi::xml_node node_image = node.child("tesseract")) {
				result_tesseract = node.attribute("result").as_uint();
			}

			// Una vez extraida la información la mostramos en pantalla
			std::cout << "--------\nDorsal\n--------\n";
			std::cout << "Numero imagen : " << numero << std::endl;
			std::cout << "Fichero imagen : " << filename_img << std::endl;
			std::cout << "Ancho imagen :  " << width << std::endl;
			std::cout << "Alto imagen : " << height << std::endl;
			std::cout << "Resultado OCR : " << result_OCR << std::endl;
			std::cout << "Resultado Tesseract : " << result_tesseract << std::endl;
			if(result_OCR == result_tesseract)
				std::cout << "Coinciden? : Si";
			else
				std::cout << "Coinciden? : No";

		}
	}

	std::cin.get();
	return 0;

}

void xml_write(	std::vector<std::vector<std::vector<int> > > *contenedor_dorsales, CImg<int> *contenedor_tiempo, CImg<int> *contenedor_numobj) {


    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    cout << (now->tm_year + 1900) << '-'
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << endl;


	FILE *fd = 0;
	fd = fopen("results_" + tm_mday + "_" + tm_mon+1 + "_" + tm_year_1900 + ".xml", "w");
	string xml_version = "1.0";
	string encoding = "UTF-8";

	string first_data = "<?xml version = " + xml_version +" enconding= " + encoding + "?> \n";
	string open_dorsales_tag = "<dorsales \n";
	string version_firmware = "\t version_firmware = 1.0";
	string filename = "prueba.txt";

	for (int pointer = 0; pointer > contenedor_dorsales.size(); pointer ++ ){

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

