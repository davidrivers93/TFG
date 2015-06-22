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

/* Search_rectangles
 *
 * Busca rectangulos que cumplan el ratio establecido y ademas que
 * tengan al menos 5 hijos.
 */

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

		//std::cout << " ** NVertices " << approx.size() << " \n";
		if (approx.size() < 4 || approx.size() > 4)
			continue;
		int sons = numberOfSons(hierarchy,n);
		//std::cout << "Numero hijos " << sons  << endl;
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
		//std::cout << "** ANGULO RADIANES ** " << degrees << endl;

		//validBBs.push_back(bb);
		dorsal_i.rectangle = bb;
		dorsal_i.approx_points = approx;
		dorsal_i.angle = degrees;
		dorsales.push_back(dorsal_i);

	}
}


/* no_repeat
 *
 * Chequeamos que no hayan 2 resultados de dorsales repetidos.
 */
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


void calculate(set<string> images, int contador,database_mng & database, int resize) {
	std::vector <float> time;
	std::set<string>::iterator it;
	std::vector<std::vector <int > > cont_dorsales;
	std::vector <string> names;
	int cont = 1;
	for (it = images.begin(); it != images.end(); it++, cont++) {
		std::cout << "Procesando imagen numero " << cont << endl;
		clock_t t0 = clock();
		string imgname = *it;
		names.push_back(imgname);
		CImg<unsigned char> img2(imgname.c_str());
		CImg<unsigned char> img;


		int orientacion = orientation(imgname);
		std::cout << "Orientacion " << orientacion << endl;
		if(orientacion==8)
			img2.rotate(-90);
		if(orientacion==7)
			img2.rotate(90);

		if(resize!=1){
			CImg <unsigned char> img_temp(img2);
			resize_own(img_temp,img2,resize);
		}

		img=img2;
		//img.display("Entrada",false);

		CImg<unsigned char> img_out_binarizacion = img;

		binarizacion_adaptativa(img, img_out_binarizacion);

		//img_out_binarizacion.display("a", false);

		Mat img_opencv = img_out_binarizacion.get_MAT();

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		Mat img_opencv_contours = img_opencv.clone();
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
			rectangle(img_orig, dorsales[i].rectangle.tl(), dorsales[i].rectangle.br(), color, 50, 10, 0);

		}

		CImg<unsigned char> img_salida;

		img_salida.assign(img_orig);


		//img_salida.display("Salida", false);
		std::vector < dorsal_final > dorsales_finales;
		for(int i=0; i<dorsales.size(); i++){
			dorsal_final dorsali;

			cv::Rect rectangle_i = dorsales[i].rectangle;
			cv::Mat copy_original = img_norect.clone();
			cv::Rect rectangle(rectangle_i.x,rectangle_i.y,rectangle_i.width,rectangle_i.height);
			cv::Mat copy_img = copy_original(rectangle);

			cv::Mat copy_orbin= img_opencv.clone();
			cv::Mat copy_bin = copy_orbin(rectangle);

			//std::cout << "Dorsal numero " << i << endl;
			//std::cout << "\t Coordenadas" << rectangle_i.x << " "<< rectangle_i.y << " " << rectangle_i.width << " " << rectangle_i.height << endl;

			dorsali.angle = dorsales[i].angle;
			dorsali.approx_points = dorsales[i].approx_points;
			dorsali.rectangle = dorsales[i].rectangle;
			dorsali.image_rect = copy_img;
			dorsali.image_bin = copy_bin;

			/*CImg < unsigned char > roi_cimg;
			roi_cimg.assign(copy_img);
			roi_cimg.display("A", false);


			int size = std::max(img_norect.cols, img_norect.rows);

			cv::Mat copy = img_norect.clone();
			cv::Mat copy_bin = img_opencv.clone();

			cv::Point2f pt((dorsales[i].rectangle.x+dorsales[i].rectangle.width)/2, (dorsales[i].rectangle.y+dorsales[i].rectangle.height)/2);
			float angulo = dorsales[i].angle * 2 * 3.1416 /360;
			std::cout << "Angulo de rotacion " << angulo << endl;
			cv::Mat r = cv::getRotationMatrix2D(pt,angulo, 1.0);
			cv::warpAffine(img_norect, copy, r,cv::Size(size,size));
			cv::warpAffine(img_opencv, copy_bin, r,cv::Size(size,size));


			std::vector <cv::Point> temp;
			for(int i2=0; i2<4; i2++){
				cv::Point2f punto;
				float x_orig=dorsales[i].approx_points[i2].x;
				float y_orig=dorsales[i].approx_points[i2].y;
				float new_x = x_orig * cos(dorsales[i].angle) + y_orig * sin(dorsales[i].angle);
				float new_y = -x_orig * sin(dorsales[i].angle) + y_orig * cos(dorsales[i].angle);;
				punto.x = new_x;
				punto.y = new_y;
				temp.push_back(punto);
			}

			cv::Point2f pt((dorsales[i].rectangle.x+dorsales[i].rectangle.width)/2, (dorsales[i].rectangle.y+dorsales[i].rectangle.height)/2);
			float angulo = dorsales[i].angle * PI/180;
			float angulorad = dorsales[i].angle;
			std::cout << "Angulo de rotacion " << angulo << endl;
			cv::Mat r = cv::getRotationMatrix2D(pt,angulo, 1.0);
			cv::warpAffine(img_norect, copy, r,cv::Size(size,size));
			cv::warpAffine(img_opencv, copy_bin, r,cv::Size(size,size));

			cv::Rect temp;
			float x_orig=dorsales[i].rectangle.x;
			float y_orig=dorsales[i].rectangle.y;
			float new_x = float(x_orig) * float(cos(angulo)) + float(y_orig) * float(sin(angulo));
			float new_y = float(-x_orig) * float(sin(angulo)) + float(y_orig) * float(cos(angulo));

			temp.x = new_x;
			temp.y = new_y;
			temp.width = dorsales[i].rectangle.width;
			temp.height = dorsales[i].rectangle.height;
			dorsali.new_points_rect = temp;

			dorsali.angle = dorsales[i].angle;
			dorsali.approx_points = dorsales[i].approx_points;
			dorsali.rectangle = dorsales[i].rectangle;
			dorsali.new_points_rect = temp;

			cv::Rect rectangle_i = dorsali.new_points_rect;
			cv::Mat copy_original = copy.clone();
			cv::Rect rectangle(rectangle_i.x,rectangle_i.y,rectangle_i.width,rectangle_i.height);
			cv::Mat copy_img = copy_original(rectangle);

			cv::Mat copy_orbin= copy_bin.clone();
			cv::Mat copy_bin2 = copy_orbin(rectangle);

			CImg < unsigned char > roi_cimg;
			roi_cimg.assign(copy_img);
			roi_cimg.display("Recortada orig", false);

			CImg < unsigned char > roi_cimg2;
			roi_cimg2.assign(copy_bin2);
			roi_cimg2.display("Recortada bin", false);
			dorsali.angle = dorsales[i].angle;
			dorsali.approx_points = dorsales[i].approx_points;
			dorsali.rectangle = dorsales[i].rectangle;
			//dorsali.new_points = temp;

			cv::Mat copy_original = img_norect.clone();
			float width = temp[1].x - temp[0].x;
			float height = temp[2].y - temp[0].y;

			cv::Rect rectangle(temp[0].x,temp[0].y,width,height);
			cv::Mat copy_img = copy(rectangle);
			cv::Mat copy_orbin= img_opencv.clone();
			cv::Mat copy_bin_1 = copybin(rectangle);

			CImg < unsigned char > roi_cimg;
			roi_cimg.assign(copy);
			roi_cimg.display("A", false);
			dorsali.image_rect = copy_img;
			dorsali.image_bin = copy_bin;*/

			dorsales_finales.push_back(dorsali);

		}

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

			//salida.display("PRUEBA", false);
			cv::Mat image_tess =255*( salida.get_MAT());
			imwrite("temp.jpg", image_tess);

			STRING text;

			if(!api.ProcessPages("temp.jpg", NULL,0, &text)){
				std::cout << "ERROR";

			}
			else{


				std::string textres= text.string();
				if(textres.length() > 0){
					char *cstr = new char[textres.length()-1];
					strcpy(cstr, textres.c_str());
					bool flag=false;
					for(int i=0; i<textres.length()-2;i++){
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
		std::vector <int >resultados_norepeat;
		process_results(resultados,resultados_norepeat);
		for(int pointer=0; pointer < resultados_norepeat.size(); pointer++){
			string out= std::to_string(resultados_norepeat[pointer]);
			std::cout << "Resultado final " << out << endl;
			//add_result_db(out, database,imgname);
			CImg <unsigned char> thumb;
			resize_own(img,thumb,5);
			string thumb_nm = "thumb_" + imgname;
			thumb.save(thumb_nm.c_str());
			std::cout << "Imagen guardada" << endl;
		}
		cont_dorsales.push_back(resultados_norepeat);
		//img_salida.display("Rectangle search", false);

		system("rm -rf temp.jpg");
		system("rm -rf thumb*");
		clock_t t1 = clock();
		float totaltime= t1-t0;
		time.push_back(totaltime);
		std::cout << "**************************" << endl;

	}

	std::cout << "Archivo de resultados guardado. \n";
	FILE *fd = 0;
	fd = fopen("resultado.csv", "w");
	if (!fd) {
		std::cerr << "Can't open resultado.csv for writing\n";
		exit(0);
	}
	float sum=0;
	fprintf(fd, "Indice");
	fprintf(fd, CARACTER_SEPARADOR_CSV);
	fprintf(fd, "Nombre archivo");
	fprintf(fd, CARACTER_SEPARADOR_CSV);
	fprintf(fd, "Tiempo empleado");
	fprintf(fd, CARACTER_SEPARADOR_CSV);
	fprintf(fd, "Dorsales detectados");
	fprintf(fd, "\n");
	for(int i=0; i< time.size(); i++){
		fprintf(fd, "%d ", i); // Indice
		sum+=time[i]/CLOCKS_PER_SEC;
		fprintf(fd, CARACTER_SEPARADOR_CSV);
		fprintf(fd, "%s ", names[i].c_str()); // Names
		fprintf(fd, CARACTER_SEPARADOR_CSV);
		fprintf(fd, "%f ", time[i]/CLOCKS_PER_SEC); // Tiempo
		fprintf(fd, CARACTER_SEPARADOR_CSV);
		for(int i2=0;i2<cont_dorsales[i].size();i2++){
			fprintf(fd, "%i ", cont_dorsales[i][i2]); // Dorsal
			fprintf(fd, CARACTER_SEPARADOR_CSV);
		}
		fprintf(fd, "\n");
	}
	float meant = float(sum)/time.size();
	std::cout << "Media de tiempo " <<meant << endl;
	fprintf(fd, "Media de tiempo");
	fprintf(fd, CARACTER_SEPARADOR_CSV);
	fprintf(fd, "%f ", meant); // Tiempo
	if (fd)
		fclose(fd);
}

void resize_own(const CImg <unsigned char> & input, CImg <unsigned char> & output, int size){

	CImg <unsigned char> copy(input);
	output = copy.resize(input.width()/size, input.height()/size,1,3);

}

void binarizacion_adaptativa(const cimg_library::CImg<unsigned char> & input,
		cimg_library::CImg<unsigned char> & out_bin) {

	/*BINARIZACION_ADAPTATIVA
	 *
	 * FUNCION QUE DADA UNA IMAGEN CREA UNA IMAGEN BINARIA
	 * CREADA MEDIANTE EL METODO DE UMBRALIZACION ADAPTATIVA.
	 */

	CImg<unsigned char> img_blur;

	float sigma = 15;
	float epsilon = 2;
	img_blur.assign(input);
	img_blur = input.get_blur(sigma);
	out_bin.assign(input.get_shared_channel(0));
	cimg_foroff(out_bin,off)
	{
		if (input(off) < img_blur(off) - epsilon) {
			out_bin(off) = 1;
		} else {
			out_bin(off) = 0;
		}
	}
	//Salida binaria
}

