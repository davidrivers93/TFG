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

#include <zbar.h>
using namespace zbar;

#define CARACTER_SEPARADOR_CSV ";"	// Carácter para separar campos en archivo csv. España = ";" pero en el resto es ",".

#include "functions.h"
#include "utilities.h"
#include "Qr_proc.h"
#include "database_mng.h"
//#include "contours_utils.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

void computeContourDepth(const std::vector<Vec4i> & hierarchy, std::vector<int> & depth);
void calculate(set<string> images, int contador,database_mng & database );
void create_txt_file();
std::string getOsName();
int showfiles(set<string> images);
cv::Point2f blobCenter(const std::vector<cv::Point> & points);
int numberOfSons(const std::vector<Vec4i> & hierarchy, int id);
void list_races(std::vector<string> & list_races, database_mng & database);
void add_result_db(std::string result, database_mng & database, std::string imagename);
std::string search_number(std::string result);

int main(int argc, char **argv) {

	int opt;

	CImgDisplay disp_cimg;

	const bool help = cimg_option("-h", false, 0);

	database_mng database;


	std::string input = "prueba.txt"; //argv[optind];

	//Si optind==0 salta ayuda
	if (help) {
		//ayuda();
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
	std::cerr << "LLEGO" << endl;
	calculate(images, contador,database);

	if (contador == 0) {
		std::cout << "No hay ninguna imagen a procesar. \n";
		exit(0);
	}

	return 0;

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
		std::cerr << "Introduce la distancia de la carrera (en metros) : ";
		string distance_string;
		std::cin >> distance_string;
		int distance;
		istringstream ( distance_string ) >> distance;
		database.race_data_query.distance = distance;
		index_bbdd_races++;

		stringstream new_race_table;
		new_race_table << new_race_date.at(0) << new_race_date.at(1) << new_race_name.at(0) << new_race_name.at(1) << new_race_date.at(6) << new_race_date.at(7);
		string new_race_tables = new_race_table.str();
		string race_ins = "ins" + new_race_tables;
		database.prepare("INSERT INTO prueba.races (idraces, races_name, races_date, race_dbname, race_distance, race_ins) VALUES (?, ?, ?, ?, ?, ?)");
		database.setInt(1,index_bbdd_races);
		database.setString(2,new_race_name);
		database.setString(3,new_race_date);
		database.setString(4,new_race_tables);
		database.setInt(5,distance);
		database.setString(6,race_ins);
		database.execute();
		std::cerr << "He llegado" << endl;
		//actualizo struct
		database.race_data_query.race_data = new_race_name;
		database.race_data_query.date_data =  new_race_date;
		database.race_data_query.tablen_data = new_race_tables;
		database.race_data_query.distance = distance;
		database.race_data_query.ins_table = race_ins;
		std::cerr << "He llegado" << endl;



	}
	else{

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


	}

	//Compruebo si existe la tabla de la carrera
	std::cerr << "He llegado" << endl;
	database.prepare("SELECT * FROM ?");
	database.setString(1, database.race_data_query.tablen_data);
	database.execute();

	string prueba_create = "CREATE TABLE " + database.race_data_query.tablen_data +  "(`dorsal` INT NOT NULL,`path_img` VARCHAR(45) NOT NULL)";
	database.execute(prueba_create);
	std::cout << "He creado la tabla. \n";


	//Compruebo si existe tabla de inscritos
	database.prepare("SELECT * FROM ?");
	database.setString(1, database.race_data_query.tablen_data);
	database.execute();


	string prueba_create_ins = "CREATE TABLE " + database.race_data_query.ins_table +  "(`dorsal` INT NOT NULL,`Nombre` VARCHAR(45) NULL,`Apellidos` VARCHAR(45) NULL,`Marca` VARCHAR(45) NULL)";
	database.execute(prueba_create_ins);
	std::cout << "He creado la tabla. \n";


}

void add_result_db(std::string result, database_mng & database, std::string imagename){

	std::cout << "Antes de formatear " << result << endl;

	std::string resultado = search_number(result);

	std::cout << "Resultado a enviar " << resultado << endl;

	std::string query = "INSERT INTO " + database.race_data_query.tablen_data + "(dorsal, path_img) VALUES (" + resultado + ",\'/" + database.race_data_query.tablen_data + "/" + imagename + "\')";

	std::cerr << "QUERY: " << query << endl;

	/*database.prepare("INSERT INTO ? (dorsal,path_img) VALUES (?,?)");
	database.setString(1,database.race_data_query.tablen_data);
	int dorsal = std::stoi(resultado,0);
	database.setInt(2,dorsal);
	string path = "/" + database.race_data_query.tablen_data + "/" + imagename;
	database.setString(1,path);*/

	database.execute(query);


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
		system("ls *.jpg > prueba.txt");
	else
		system("dir *.jpg> prueba.txt");

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

void calculate(set<string> images, int contador,database_mng & database ) {

	if (images.size()) {

		std::set<string>::iterator it;
		for (it = images.begin(); it != images.end(); it++) {
			string imgname = *it;
			CImg<unsigned char> img(imgname.c_str());

			CImg<unsigned char> img_out_binarizacion = img;

			binarizacion_adaptativa(img, img_out_binarizacion);

			img_out_binarizacion.display("a", false);

			//img.blur(0.8,0.8);

			Mat img_opencv = img_out_binarizacion.get_MAT();

			Mat edges(img_opencv.size(), CV_MAKETYPE(img_opencv.depth(), 1));			// To hold Grayscale Image
			Mat traces(img_opencv.size(), CV_8UC3);

			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			Mat img_opencv_contours = img_opencv.clone();
			//Canny(img_opencv, edges, 100 , 200, 3);
			findContours(img_opencv_contours, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Find contours with hierarchy

			//imshow("A", img_opencv);
			std::cout << "Contours " << contours.size() << endl;

			std::vector<Rect> validBBs;

			std::vector<int> depth;
			computeContourDepth(hierarchy, depth);

			std::vector <int> valid_contours;

			Size s = img_opencv.size();
			for (int n = 0; n < contours.size(); n++) {
				if (depth[n] % 2) // Exteriores;
					continue;
				Rect bb = boundingRect(contours[n]);

				if (bb.height > bb.width)
					continue;
				if (bb.width < 50 || bb.width > s.width / 4)
					continue;
				std::vector<cv::Point> approx;
				approxPolyDP(contours[n], approx, 10, true);
				std::cout << " ** NVertices " << approx.size() << " \n";
				if (approx.size() < 4 || approx.size() > 4)
					continue;
				int sons = numberOfSons(hierarchy,n);
				//if(sons < 5)
				//	continue;
				validBBs.push_back(bb);

			}

			std::cout << "**Tamaño de BBs:  " << validBBs.size() << endl;
			Mat img_orig = img.get_MAT();
			Mat image_opencv_rect = img_opencv;
			Scalar color = Scalar(0, 0, 0);
			for (int i = 0; i < validBBs.size(); i++) {

				rectangle(img_orig, validBBs[i].tl(), validBBs[i].br(), color, 5, 10, 0);
				std::cout << "Valor " << i << endl;

			}
			CImg<unsigned char> img_salida;

			img_salida.assign(img_orig);

			img_salida.display("Salida", false);

			std::vector<std::vector<Rect> > dorsales;

			std::cout << "ValidBBs tamaño " << validBBs.size() << endl;

			std::vector<std::vector<int> > num;
			std::vector <Rect> valid_BBs_son;



			std::cout << "Tamaño valid sons" <<  valid_BBs_son.size() << endl;

			for (int i = 0; i < validBBs.size(); i++) {
				Rect bb_temp = validBBs[i];
				Point2f first_object_mass;
				first_object_mass.x = (bb_temp.x + bb_temp.width) / 2;
				first_object_mass.y = (bb_temp.y + bb_temp.height) / 2;
				for (int i2 = 0; i2 < validBBs.size(); i2++) {
					if (i == i2)
						continue;
					Rect bb_temp_2 = validBBs[i2];
					Point2f second_object_mass;
					second_object_mass.x = (bb_temp_2.x + bb_temp_2.width) / 2;
					second_object_mass.y = (bb_temp_2.y + bb_temp_2.height) / 2;
					float ratio_mass_x = float(second_object_mass.x) / float(first_object_mass.x);
					float ratio_width =float(bb_temp_2.width) / float(bb_temp.width);

					std::cout << "\tRatio 1 " << ratio_width << endl;
					std::cout << "\tRatio masasa 1" << ratio_mass_x << endl;

					if (ratio_mass_x > 1.01 || ratio_mass_x < 0.99)
						continue;

					if (ratio_width > 1.01 || ratio_width < 0.99)
						continue;

					for (int i3 = 0; i3 < validBBs.size(); i3++) {

						if (i3 == i || i3 == i2)
							continue;
						Rect bb_temp_3 = validBBs[i3];
						Point2f third_object_mass;
						third_object_mass.x = (bb_temp_3.x + bb_temp_3.width) / 2;
						third_object_mass.y = (bb_temp_3.y + bb_temp_3.height) / 2;
						float radio_mass_x = float(third_object_mass.x) / float(first_object_mass.x);
						float ratio_width =float(bb_temp_3.width) / float(bb_temp.width);
						std::cout << "\tRatio 2 " << ratio_width << endl;
						std::cout << "\tRatio masasa 2" << ratio_mass_x << endl;
						if (ratio_mass_x > 1.01 || ratio_mass_x < 0.99)
							continue;
						if (ratio_width > 1.01 || ratio_width < 0.99)
							continue;

						std::cout << "\tRatio 2 pasado " << ratio_width << endl;
						std::cout << "\tRatio masasa 2 pasado " << ratio_mass_x << endl;

						std::vector<Rect> temp(3);
						temp[0] = bb_temp;
						temp[1] = bb_temp_2;
						temp[2] = bb_temp_3;
						dorsales.push_back(temp);
						std::vector<int> temp2(3);
						temp2[0] = i;
						temp2[1] = i2;
						temp2[2] = i3;
						num.push_back(temp2);

					}

				}

			}

			std::cout << "Numero dorsales " << dorsales.size() << endl;

			for (int i = 0; i < num.size(); i++) {
				std::cout << "Dorsales " << num[i][0] << " " << num[i][1] << " " << num[i][2] << endl;

			}

			std::vector<std::vector<Rect> > dorsales_ordenados;
			std::vector<std::vector<int> > num_ordenados;

			//Ordeno los dorsales
			for (int i = 0; i < num.size(); i++) {

				Rect y1 = dorsales[i][0];
				Rect y2 = dorsales[i][1];
				Rect y3 = dorsales[i][2];

				std::vector<Rect> temp(3);
				std::vector<int> temp_int(3);
				if (y1.y < y2.y && y1.y < y3.y) {

					temp[0] = y1;
					temp_int[0] = num[i][0];
					if (y2.y < y3.y) {
						temp[1] = y2;
						temp_int[1] = num[i][1];
						temp[2] = y3;
						temp_int[2] = num[i][2];
					}
					else {
						temp[1] = y3;
						temp_int[1] = num[i][1];
						temp[2] = y2;
						temp_int[2] = num[i][2];
					}

					dorsales_ordenados.push_back(temp);
					num_ordenados.push_back(temp_int);
					continue;
				}

				if (y2.y < y1.y && y2.y < y3.y) {

					temp[0] = dorsales[i][1];
					temp_int[0] = num[i][1];

					if (y1.y < y3.y) {
						temp[1] = dorsales[i][0];
						temp_int[1] = num[i][0];
						temp[2] = dorsales[i][2];
						temp_int[2] = num[i][2];
					}
					else {
						temp[1] = dorsales[i][2];
						temp_int[1] = num[i][2];
						temp[2] = dorsales[i][0];
						temp_int[2] = num[i][0];
					}
					dorsales_ordenados.push_back(temp);
					num_ordenados.push_back(temp_int);
					continue;

				}

				if (y3.y < y2.y && y3.y < y1.y) {

					temp[0] = dorsales[i][2];
					temp_int[0] = num[i][2];

					if (y2.y < y1.y) {
						temp[1] = dorsales[i][1];
						temp_int[1] = num[i][1];
						temp[2] = dorsales[i][0];
						temp_int[2] = num[i][0];
					} else {
						temp[1] = dorsales[i][0];
						temp_int[1] = num[i][0];
						temp[2] = dorsales[i][1];
						temp_int[2] = num[i][1];
					}
					dorsales_ordenados.push_back(temp);
					num_ordenados.push_back(temp_int);
					continue;

				}

			}


			std::cout << "Tamaño final " << dorsales_ordenados.size() << endl;

			std::vector<std::vector<Rect> > dorsales_finales;
			std::vector<std::vector<int> > num_finales;

			for (int i = 0; i < num_ordenados.size(); i++) {
				if (i == 0) {

					std::vector<Rect> temp_rect(3);
					std::vector<int> temp_num(3);
					temp_rect[0] = dorsales_ordenados[i][0];
					temp_rect[1] = dorsales_ordenados[i][1];
					temp_rect[2] = dorsales_ordenados[i][2];
					dorsales_finales.push_back(temp_rect);
					temp_num[0] = num_ordenados[i][0];
					temp_num[1] = num_ordenados[i][1];
					temp_num[2] = num_ordenados[i][2];
					num_finales.push_back(temp_num);
					continue;

				}

				for (int i2 = 0; i2 < num_finales.size(); i2++) {

					if (num_ordenados[i][0] == num_finales[i2][0] && num_ordenados[i][1] == num_finales[i2][1] && num_ordenados[i][2] == num_finales[i2][2]) {
						break;
					}
					if (i2 == num_finales.size() - 1) {
						std::vector<Rect> temp_rect(3);
						std::vector<int> temp_num(3);
						temp_rect[0] = dorsales_ordenados[i][0];
						temp_rect[1] = dorsales_ordenados[i][1];
						temp_rect[2] = dorsales_ordenados[i][2];
						dorsales_finales.push_back(temp_rect);
						temp_num[0] = num_ordenados[i][0];
						temp_num[1] = num_ordenados[i][1];
						temp_num[2] = num_ordenados[i][2];
						num_finales.push_back(temp_num);
					}
				}

			}
			/*
			POR IMPLEMENTAR NUMERO DE HIJOS
			std::vector<std::vector<Rect> > temp_dorsales_finales;
			std::vector<std::vector<int> > temp_num_finales;

			for (int i = 0; i < num_ordenados.size(); i++) {



			}*/

			std::cerr << "Tamaño final " << num_finales.size() << endl;

			std::vector<std::vector<cv::Mat> > dorsal_objects;
			for (int i = 0; i < num_finales.size(); i++) {
				Rect bb_1 = dorsales_ordenados[i][0];
				Rect bb_2 = dorsales_ordenados[i][1];
				Rect bb_3 = dorsales_ordenados[i][2];

				std::cout << "Conjunto numero " << i << endl;
				std::cout << "Indices" << num_finales[i][0] << " " << num_finales[i][1] << " " << num_finales[i][2] << endl;
				std::cout << "\t Coordenadas obj1" << bb_1.x << " "<< bb_1.y << " " << bb_1.width << " " << bb_1.height << endl;
				std::cout << "\t Coordenadas obj1" << bb_2.x << " "<< bb_2.y << " " << bb_2.width << " " << bb_2.height << endl;
				std::cout << "\t Coordenadas obj1" << bb_3.x << " "<< bb_3.y << " " << bb_3.width << " " << bb_3.height << endl;
				//Mat img_opencv = img_out_binarizacion.get_MAT();


				cv::Rect myROI(bb_1.x, bb_1.y, bb_1.width, bb_1.height);
				cv::Mat copy_img_1 = img_orig.clone();
				cv::Mat obj1 = copy_img_1(myROI);
				cv::Mat copy_img1_bin = img_opencv.clone();
				cv::Mat obj1_bin = copy_img1_bin(myROI);
				CImg<unsigned char> ob1_cimg;
				ob1_cimg.assign(obj1);
				CImg<unsigned char> ob1_cimg_bin;
				ob1_cimg_bin.assign(obj1_bin);

				cv::Rect myROI2(bb_2.x, bb_2.y, bb_2.width, bb_2.height);

				cv::Mat copy_img_2 = img_orig.clone();
				cv::Mat obj2 = copy_img_2(myROI2);
				cv::Mat copy_img2_bin = img_opencv.clone();
				cv::Mat obj2_bin = copy_img2_bin(myROI2);
				CImg<unsigned char> ob2_cimg;
				ob2_cimg.assign(obj2);
				CImg<unsigned char> ob2_cimg_bin;
				ob2_cimg_bin.assign(obj2_bin);

				cv::Rect myROI3(bb_3.x, bb_3.y, bb_3.width, bb_3.height);
				cv::Mat copy_img_3 = img_orig.clone();

				cv::Mat obj3 = copy_img_3(myROI3);
				cv::Mat copy_img3_bin = img_opencv.clone();
				cv::Mat obj3_bin = copy_img3_bin(myROI3);
				CImg<unsigned char> ob3_cimg;
				ob3_cimg.assign(obj3);
				CImg<unsigned char> ob3_cimg_bin;
				ob3_cimg_bin.assign(obj3_bin);

				ob1_cimg.get_append(ob2_cimg, 'x').get_append(ob3_cimg,'x').display("Objetos", false);
				ob1_cimg_bin.get_append(ob2_cimg_bin, 'x').get_append(ob3_cimg_bin,'x').display("Objetos binarizados", false);

				std::vector <cv::Mat> temp(3);
				temp[0] = obj1_bin;
				temp[1] = obj2_bin;
				temp[2] = obj3_bin;

				dorsal_objects.push_back(temp);

			}

			for(int i=0; i<num_finales.size(); i++){
				imwrite("temp.jpg", dorsal_objects[i][1]);
				char *outtext;
				//Inicializamos el tesseract
				 tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
				 //Cargamos la API. El idioma nos da igual. ENG por defecto.
				 if(api->Init(NULL, "eng")){
				 fprintf(stderr, "Could not initizalize tesseract. \n");
				 exit(1);
				 }

				 Pix *image = pixRead("temp.jpg");
				 pixDisplay(image,100,100);
				 api->SetImage(image);
				 outtext = api->GetUTF8Text();

				 if(outtext == NULL){
				 std::cout << "No hemos detectado nada. \n";
				 }

				 printf("Resultado OCR: \n%s", outtext);

				 api->End();
				 pixDestroy(&image);

				 add_result_db(outtext, database,imgname);
			}

		}
	}

	//Tesseract
	system("rm -rf temp.jpg");


}

void computeContourDepth(const std::vector<Vec4i> & hierarchy, std::vector<int> & depth) {
	bool continuar = true;

	depth.resize(hierarchy.size());
	while (continuar) {
		continuar = false;
		for (int i = 0; i < hierarchy.size(); i++) {
			int next = hierarchy[i][0];
			int prev = hierarchy[i][1];
			int first_child = hierarchy[i][2];
			int parent = hierarchy[i][3];
			if (depth[i] > 0)
				continue;
			if (parent < 0) {
				depth[i] = 1;
				continuar = true;
			} else if (depth[parent] > 0) {
				depth[i] = depth[parent] + 1;
				continuar = true;
			}
		}
	}
}

cv::Point2f blobCenter(const std::vector<cv::Point> & points) {
	cv::Point2f media(0, 0);
	for (int n = 0; n < points.size(); n++) {
		media.x = media.x + points[n].x;
		media.y = media.y + points[n].y;
	}
	media.x = media.x / points.size();
	media.y = media.y / points.size();
	return media;
}

int numberOfSons(const std::vector<Vec4i> & hierarchy, int id) {

	int nsons = 0;
	int son = hierarchy[id][2];
	while ( son >= 0) {
		nsons ++;
		son = hierarchy[son][0];
	}
	return nsons;
}
