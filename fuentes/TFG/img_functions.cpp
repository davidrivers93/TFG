/*
 * img_functions.cpp
 *
 *  Created on: 18/6/2015
 *      Author: David
 */


#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <fstream>
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

#include "database_mng.h"
#include "db_functions.h"
#include "img_functions.h"
#include "misc_functions.h"
#include "functions.h"


using namespace cimg_library;
using namespace std;
using namespace cv;

void save_image(std::string imagename, database_mng & database, CImg <unsigned char> & image_save){

	std::string path_xampp = "/opt/lampp/htdocs";

	std::string path_image = path_xampp + "/" + database.race_data_query.tablen_data + "/" + imagename ;

	std::cout << "Ruta de la imagen " << path_image << endl;

	//if(!check_file(path_image.c_str()))
	if(fileExists(path_image.c_str()))
		std::cout << "Imagen guardada anteriormente" << endl;

	else{
		image_save.save(path_image.c_str());
		std::cout << "Imagen " << path_image << " guardada" << endl;
	}


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

void search_rectangles(vector< dorsal > & dorsales ,vector<vector<Point> > & contours , vector<Vec4i> & hierarchy, Size & s,std::vector<int> depth){
	for (int n = 0; n < contours.size(); n++) {

		dorsal dorsal_i;

		//Solo tomamos los que tengan depth impar.
		if (depth[n] % 2) // Exteriores;
			continue;

		Rect bb = boundingRect(contours[n]);

		//Descartamos bb que sean mas altos que anchos, que sean muy pequeños.
		if (bb.height > bb.width)
			continue;
		if (bb.width < 50 || bb.width > s.width / 4)
			continue;
		float ratio_wh_obj = float(bb.width)/float(bb.height);
		//if(ratio_wh_obj < 1.15 || ratio_wh_obj > 1.25) continue;
		if (bb.height > 0.6*bb.width)
			continue;
		std::vector<cv::Point> approx;
		double area = contourArea(contours[n]);

		float ratio_wh = float(23/19.3);
		float height = sqrt(area/ratio_wh);
		float tolerance = 0.15 * height;

		approxPolyDP(contours[n], approx, tolerance, true);


		std::cout << " ** NVertices " << approx.size() << " \n";
		if (approx.size() < 4 || approx.size() > 4)
			continue;
		int sons = numberOfSons(hierarchy,n);
		std::cout << "Numero hijos " << sons  << endl;
		if(sons < 5)
			continue;

		int y1=0;
		int y2=0;
		int x1=0;
		int x2=0;
		//PERSPECTIVA
		int point1=0;
		int point2=0;

		for(int i=0; i<4; i++){
			if(i==0){
				y1=approx[0].y;
				point1=i;
			}
			else{
				if(approx[i].y>y1) {
					y1=approx[i].y;
					point1=i;
				}
			}

		}

		for(int i=0; i<4; i++){
			if(i==0){
				y2=approx[0].y;
				point2=i;
			}
			else{
				if(approx[i].y<y2 && approx[i].y>=y2) {
					y2=approx[i].y;
					point2=i;
				}
			}

		}

		x1=approx[point1].x;
		x2=approx[point2].x;

		float pendiente = float(y2-y1)/float(x2-x1);
		float degrees = atan(pendiente);
		std::cout << "** ANGULO ** " << degrees << endl;

		//validBBs.push_back(bb);
		dorsal_i.rectangle = bb;
		dorsal_i.approx_points = approx;
		dorsal_i.angle = degrees;
		dorsales.push_back(dorsal_i);

	}
}

void get_bibs(std::vector<Rect> & validBBs, Size & s, std::vector<std::vector<Rect> > & dorsales, std::vector<std::vector<int> > & num){
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

			float dist_x = bb_temp_2.x - bb_temp.x;
			float dist_y = bb_temp_2.x - bb_temp.x;
			float width_5 = 0.1 * s.width;

			if(dist_y > width_5) continue;


			//std::cout << "\tRatio 1 " << ratio_width << endl;
			//std::cout << "\tRatio masasa 1" << ratio_mass_x << endl;

			if (ratio_mass_x > 1.2 || ratio_mass_x < 0.95)
				continue;

			if (ratio_width > 1.2 || ratio_width < 0.95)
				continue;

			for (int i3 = 0; i3 < validBBs.size(); i3++) {

				if (i3 == i || i3 == i2)
					continue;


				Rect bb_temp_3 = validBBs[i3];
				float dist_3_y = bb_temp_3.x - bb_temp_2.x;
				float width_7 = 0.1 * s.width;

				if(dist_3_y > width_7) continue;

				Point2f third_object_mass;

				third_object_mass.x = (bb_temp_3.x + bb_temp_3.width) / 2;
				third_object_mass.y = (bb_temp_3.y + bb_temp_3.height) / 2;

				float radio_mass_x = float(third_object_mass.x) / float(first_object_mass.x);
				float ratio_width =float(bb_temp_3.width) / float(bb_temp.width);
				float ratio_first_third = float(bb_temp_3.height) / float(bb_temp.height);

				//std::cout << "\tRatio 2 " << ratio_width << endl;
				//std::cout << "\tRatio masasa 2" << ratio_mass_x << endl;

				if (ratio_mass_x > 1.2 || ratio_mass_x < 0.95)
					continue;
				if (ratio_width > 1.2 || ratio_width < 0.95)
					continue;
				if(ratio_first_third > 1.3 || ratio_first_third <0.7) continue;


				//std::cout << "\tRatio 2 pasado " << ratio_width << endl;
				//std::cout << "\tRatio masasa 2 pasado " << ratio_mass_x << endl;

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
}

void ordenar(std::vector<std::vector<Rect> > & dorsales, std::vector<std::vector<int> > & num, std::vector<std::vector<Rect> > & dorsales_ordenados,std::vector<std::vector<int> > & num_ordenados){
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

}

void no_repeat(std::vector<std::vector<Rect> > & dorsales_ordenados,std::vector<std::vector<int> > & num_ordenados,std::vector<std::vector<Rect> > & dorsales_finales, std::vector<std::vector<int> > & num_finales ){
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
}


void calculate(set<string> images, int contador,database_mng & database) {

	std::set<string>::iterator it;
	for (it = images.begin(); it != images.end(); it++) {
		string imgname = *it;
		CImg<unsigned char> img(imgname.c_str());

		//img.display("Entrada",false);

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
		findContours(img_opencv_contours, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_TC89_KCOS); // Find contours with hierarchy

		//imshow("A", img_opencv);
		std::cout << "Contours " << contours.size() << endl;

		std::vector<Rect> validBBs;

		std::vector<int> depth;
		computeContourDepth(hierarchy, depth);

		std::vector <int> valid_contours;




		Size s = img_opencv.size();
		std::vector <dorsal> dorsales;
		search_rectangles(dorsales, contours, hierarchy, s, depth);



		//std::cout << "**Tamaño de BBs:  " << validBBs.size() << endl;
		Mat img_orig = img.get_MAT();
		Mat img_norect = img_orig.clone();
		Mat img_orig_cut = img_orig.clone();
		Mat image_opencv_rect = img_opencv;
		Scalar color = Scalar(0, 0, 0);
		for (int i = 0; i < validBBs.size(); i++) {
			//Pintamos rectangulos
			rectangle(img_orig, dorsales[i].rectangle.tl(), dorsales[i].rectangle.br(), color, 10, 10, 0);
			//std::cout << "Valor " << i << endl;

		}

		CImg<unsigned char> img_salida;

		img_salida.assign(img_orig);

		img_salida.display("Salida", false);

		//std::vector<std::vector<Rect> > dorsales;

		//std::cout << "ValidBBs tamaño " << validBBs.size() << endl;

		//std::vector<std::vector<int> > num;


		/*get_bibs(validBBs,s,dorsales,num);


		std::cout << "Numero dorsales " << dorsales.size() << endl;

		for (int i = 0; i < num.size(); i++) {
				std::cout << "Dorsales " << num[i][0] << " " << num[i][1] << " " << num[i][2] << endl;

			}

		std::vector<std::vector<Rect> > dorsales_ordenados;
		std::vector<std::vector<int> > num_ordenados;

		//Ordeno los dorsales
		ordenar(dorsales,num,dorsales_ordenados, num_ordenados);


		std::cout << "Tamaño final " << dorsales_ordenados.size() << endl;

		std::vector<std::vector<Rect> > dorsales_finales;
		std::vector<std::vector<int> > num_finales;

		no_repeat(dorsales_ordenados, num_ordenados, dorsales_finales, num_finales);


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
			cv::Mat copy_img_1 = img_norect.clone();
			cv::Mat obj1 = copy_img_1(myROI);
			cv::Mat copy_img1_bin = img_opencv.clone();
			cv::Mat obj1_bin = copy_img1_bin(myROI);
			CImg<unsigned char> ob1_cimg;
			ob1_cimg.assign(obj1);
			CImg<unsigned char> ob1_cimg_bin;
			ob1_cimg_bin.assign(obj1_bin);

			cv::Rect myROI2(bb_2.x, bb_2.y, bb_2.width, bb_2.height);

			cv::Mat copy_img_2 = img_norect.clone();
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
			cv::Mat copy_img3_bin = img_norect.clone();
			cv::Mat obj3_bin = copy_img3_bin(myROI3);
			CImg<unsigned char> ob3_cimg;
			ob3_cimg.assign(obj3);
			CImg<unsigned char> ob3_cimg_bin;
			ob3_cimg_bin.assign(obj3_bin);

			//ob1_cimg.get_append(ob2_cimg, 'x').get_append(ob3_cimg,'x').display("Objetos", false);
			//ob1_cimg_bin.get_append(ob2_cimg_bin, 'x').get_append(ob3_cimg_bin,'x').display("Objetos binarizados", false);

			//CImg<unsigned char> dorsal_cimg(ob3_cimg_bin);

			//OCR_proc(dorsal_cimg,resultados);

			std::vector <cv::Mat> temp(3);
			temp[0] = obj1_bin;
			temp[1] = obj2_bin;
			temp[2] = obj3_bin;

			dorsal_objects.push_back(temp);

		}*/

		std::vector < dorsal_final > dorsales_finales;
		for(int i=0; i<dorsales.size(); i++){

			/*cv::Rect myROI(bb_1.x, bb_1.y, bb_1.width, bb_1.height);
			cv::Mat copy_img_1 = img_norect.clone();
			cv::Mat obj1 = copy_img_1(myROI);
			cv::Mat copy_img1_bin = img_opencv.clone();
			cv::Mat obj1_bin = copy_img1_bin(myROI);
			CImg<unsigned char> ob1_cimg;
			ob1_cimg.assign(obj1);
			CImg<unsigned char> ob1_cimg_bin;
			ob1_cimg_bin.assign(obj1_bin);*/

			dorsal_final dorsali;

			cv::Rect rectangle_i = dorsales[i].rectangle;
			cv::Mat copy_original = img_norect.clone();
			cv::Rect rectangle(rectangle_i.x,rectangle_i.y,rectangle_i.width,rectangle_i.height);
			cv::Mat copy_img = copy_original(rectangle);

			cv::Mat copy_orbin= img_opencv.clone();
			cv::Mat copy_bin = copy_orbin(rectangle);

			std::cout << "Dorsal numero " << i << endl;
			std::cout << "\t Coordenadas" << rectangle_i.x << " "<< rectangle_i.y << " " << rectangle_i.width << " " << rectangle_i.height << endl;

			CImg < unsigned char > roi_cimg;
			roi_cimg.assign(copy_img);
			roi_cimg.display("A", false);

			dorsali.angle = dorsales[i].angle;
			dorsali.approx_points = dorsales[i].approx_points;
			dorsali.rectangle = dorsales[i].rectangle;
			dorsali.image_rect = copy_img;
			dorsali.image_bin = copy_bin;

			dorsales_finales.push_back(dorsali);

		}
		std::cout << "TAMAÑO "  << dorsales_finales.size() << endl;
		std::vector < dorsal > dorsales_rt;
		/*for(int i=0; i<dorsales.size(); i++){

			int size = std::max(img_norect.cols, img_norect.rows);
			cv::Mat copy = img_norect.clone();
			cv::Point2f pt((dorsales[i].rectangle.x+dorsales[i].rectangle.width)/2, (dorsales[i].rectangle.y+dorsales[i].rectangle.height)/2);
			cv::Mat r = cv::getRotationMatrix2D(pt,dorsales[i].angle, 1.0);
			cv::warpAffine(img_norect, copy, r,cv::Size(size,size));

			CImg < unsigned char > roi_cimg;
						roi_cimg.assign(copy);
						roi_cimg.display("A", false);


		}*/

		tesseract::TessBaseAPI api;
		api.Init(NULL, "eng");
		std::vector <int> resultados;
		for(int i=0; i<dorsales_finales.size(); i++){

			CImg<unsigned char> dorsal_cimg;
			dorsal_cimg.assign(dorsales_finales[i].image_bin);


			CImg<int> bbox;
			CImg<int> areas;
			CImg<int> seg;
			CImg<float> cdg;

			segmentacion(dorsal_cimg, seg, bbox, areas, cdg);

			int numobj = bbox.size();

			//seg.display("PRUEBA", false);
			std::vector <int> comienzos;
			float area_img = dorsal_cimg.width() * dorsal_cimg.height();

			std::cout << "Coordenadas: " <<endl;
				std::cout << "\t xmin " << bbox(0,i) << endl;
				std::cout << "\t xmax " << bbox(1,i) << endl;
				std::cout << "\t ymin " << bbox(2,i) << endl;
				std::cout << "\t ymax " << bbox(3,i) << endl;

			for(int i=0; i<bbox.size(); i++){
				float width = bbox(1,i) - bbox(0,i);
				float height = bbox(3,i) - bbox(2,i);
				if(bbox(1,i) < 0.2 * seg.width()) continue;
				if(bbox(0,i) > 0.8 * seg.width()) continue;
				if(bbox(3,i) > 0.6 * seg.height()) continue;
				if(areas(i) < 0.01 * area_img) continue;
				if(float(width)/float(height) > 2) continue;
				comienzos.push_back(i);
			}

			//std::cout << "Tamaño " << comienzos.size() << endl;

			CImg<int> tabla(numobj);

			tabla.fill(0);
			for (int h = 0; h < comienzos.size(); h++) {
				int indice_objeto = comienzos[h];
				tabla[indice_objeto] = 1;
			}
			CImg<int> seg2(seg);
			SeleccionarEtiquetas_cimg(seg2, tabla, numobj);
			//seg2.display();
			CImg<int> salida(seg2);
			cimg_forXY(seg2,x,y){
				if(seg2(x,y) == 0) salida(x,y) = 1;
				else salida(x,y)=0;
			}

			salida.display("PRUEBA", false);
			cv::Mat image_tess =255*( salida.get_MAT());
			imwrite("temp.jpg", image_tess);

			/*//Inicializamos el tesseract
			tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
				 //Cargamos la API. El idioma nos da igual. ENG por defecto.
				 if(api->Init(NULL, "eng")){
					 fprintf(stderr, "Could not initizalize tesseract. \n");
					 exit(1);
				 }*/

			STRING text;

			if(!api.ProcessPages("temp.jpg", NULL,0, &text)){
				std::cout << "ERROR";

			}
			else{


				std::string textres= text.string();
				if(textres.length() > 0){
					std::cout << "Resultado " << text.string() << endl;
					char *cstr = new char[textres.length()-1];
					strcpy(cstr, textres.c_str());
					bool flag=false;
					for(int i=0; i<textres.length()-2;i++){
						std::cout << "char " << cstr[i] << endl;
						if(!isdigit(cstr[i])){
							flag=true;
							break;
						}

					}
					//std::cout << "flag " << flag << endl;
					if(flag==false){
						int res = std::stoi (text.string(),0);
						resultados.push_back(res);
					}
				}
			}

		}
		std::cout << "He salido del bucle " << endl;
		std::vector <int >resultados_norepeat;
		//process_results(resultados,resultados_norepeat);
		std::cout << "Tamaño resultados finales " << resultados_norepeat.size() << endl;
		for(int pointer=0; pointer < resultados_norepeat.size(); pointer++){
			string out= std::to_string(resultados_norepeat[pointer]);
			std::cout << "\n" << endl;
			std::cout << "Resultado final " << out << endl;
			std::cout << endl;
			//add_result_db(out, database,imgname);

		}

		img_salida.display("Rectangle search", false);
		//save_image(imgname, database, img);
		//save_thumbail(imgname, database, img);
		system("rm -rf temp.jpg");

	}

}
