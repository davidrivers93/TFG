/*
 * bib_generator.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: david
 */

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

#include <cairo/cairo.h>
#include <string>

#include <zbar.h>
using namespace zbar;

#define CARACTER_SEPARADOR_CSV ";"	// Carácter para separar campos en archivo csv. España = ";" pero en el resto es ",".

#include "database_mng.h"
//#include "contours_utils.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

void putTextCairo(cv::Mat &targetImage,
				std::string const& text,
				cv::Point2d centerPoint,
				std::string const& fontFace,
				double fontSize,
				cv::Scalar textColor,
				bool fontItalic,
				bool fontBold);

int main(int argc, char **argv) {


	Mat image;
	image = imread("plantilla_dorsa.jpg", 1);

	database_mng database;

	database.connect();
	string db_races = "prueba";
	database.switchDb(db_races);
	std::cout << "BBDD conectada" << endl;

	//Hacemos la llamada a la bbdd

	database.execute("SELECT * FROM races");


	while (database.fetch()) {
		std::cerr << "*****************************************************************" << endl;
		std::cerr << " Race " << database.print(1)<< endl;
		std::cerr << "\t Nombre carrera: " << database.print(2) <<endl;
		std::cerr << "\t Fecha carrera: " << database.print(3) <<endl;
		std::cerr << "\t Nombre bbdd dorsales: " << database.print(4) <<endl;
		std::cerr << "\t Distancia: " << database.print(5) <<endl;
		std::cerr << "\t Nombre BBDD inscritos: " << database.print(6) <<endl;
		std::cerr << "*****************************************************************" << endl;
	}

	std::cerr << "Elige la carrera: \n";
	string index_race;
	std::cin >> index_race;

	//Actualizamos struct con carrera seleccionada
	string query_race = "Select * FROM races WHERE idraces= \'" + index_race + "\'";
	database.execute(query_race);
	database.fetch();
	database.race_data_query.race_data = database.print(2);
	database.race_data_query.date_data = database.print(3);
	database.race_data_query.tablen_data = database.print(4);

	string temp = database.print(5);
	int distance;
	istringstream ( temp ) >> distance;
	database.race_data_query.distance = distance;
	database.race_data_query.ins_table = database.print(6);
	std::cerr << "Elige la carrera: \n";
	database.execute("SELECT * FROM " + database.race_data_query.ins_table);

	while (database.fetch()) {

		std::cout << " Dorsal numero " << database.print(1) << endl;
		string dorsals = database.print(1);
		int dorsal;
		istringstream ( dorsals ) >> dorsal;
		std::cout << "\t Nombre " << database.print(2) << endl;
		std::cout << "\t Apellido " << database.print(3) << endl;
		std::cout << "\t Marca " << database.print(4) << endl;
		std::cout << "\t Apodo " << database.print(5) << endl;

		Mat template_image = image.clone();
		std::string dorsal_text = database.print(1);
		cv::Point2d center;
		center.x = (template_image.cols)/2;
		center.y = (template_image.rows)*0.42;
		std::string font = "Rockwell Extra Bold";
		cv::Scalar color = Scalar(0,0,0);
		double fontSize = 450;

		putTextCairo(template_image, dorsal_text, center, font, fontSize, color, false, true);

		std::string dorsal_text2 = database.print(5);
		cv::Point2d center2;
		center2.x = (template_image.cols)/2;
		center2.y = (template_image.rows)*(1-0.40);

		double fontSize2 = 250;

		putTextCairo(template_image, dorsal_text2, center2, font, fontSize2, color, false, true);

		std::string image_dorsal_file = "prueba" + dorsal_text;
		std::cout << "Nombre archivo" << image_dorsal_file << endl;
		CImg <unsigned char> orig, cambiada;
		orig.assign(image);
		cambiada.assign(template_image);
		orig.get_append(cambiada, 'x').display("Slaida", false);
		//imwrite(image_dorsal_file, template_image);

		std::cout << "Imagen guardada" << endl;

	}


}



void putTextCairo(cv::Mat &targetImage,
				std::string const& text,
				cv::Point2d centerPoint,
				std::string const& fontFace,
				double fontSize,
				cv::Scalar textColor,
				bool fontItalic,
				bool fontBold)
		{
	// Create Cairo
	cairo_surface_t* surface =
			cairo_image_surface_create(
					CAIRO_FORMAT_ARGB32,
					targetImage.cols,
					targetImage.rows);

	cairo_t* cairo = cairo_create(surface);

	// Wrap Cairo with a Mat
	cv::Mat cairoTarget(
			cairo_image_surface_get_height(surface),
			cairo_image_surface_get_width(surface),
			CV_8UC4,
			cairo_image_surface_get_data(surface),
			cairo_image_surface_get_stride(surface));

	// Put image onto Cairo
	cv::cvtColor(targetImage, cairoTarget, cv::COLOR_BGR2BGRA);

	// Set font and write text
	cairo_select_font_face (
			cairo,
			fontFace.c_str(),
			fontItalic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
					fontBold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);

	cairo_set_font_size(cairo, fontSize);
	cairo_set_source_rgb(cairo, textColor[2], textColor[1], textColor[0]);

	cairo_text_extents_t extents;
	cairo_text_extents(cairo, text.c_str(), &extents);

	cairo_move_to(
			cairo,
			centerPoint.x - extents.width/2 - extents.x_bearing,
			centerPoint.y - extents.height/2- extents.y_bearing);
	cairo_show_text(cairo, text.c_str());

	// Copy the data to the output image
	cv::cvtColor(cairoTarget, targetImage, cv::COLOR_BGRA2BGR);

	cairo_destroy(cairo);
	cairo_surface_destroy(surface);
}
