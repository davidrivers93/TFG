/*
 * img_functions.h
 *
 *  Created on: 18/6/2015
 *      Author: David
 */

#ifndef TFG_IMG_FUNCTIONS_H_
#define TFG_IMG_FUNCTIONS_H_

#include "database_mng.h"
#include "db_functions.h"
#include "img_functions.h"
#include "misc_functions.h"
#include <opencv2/opencv.hpp>
#include "functions.h"

#define cimg_plugin1 "cimgcvMat.h"
#include "CImg.h"
#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

struct dorsal{
	cv::Rect rectangle;
	std::vector<cv::Point> approx_points;
	float angle;
};

struct dorsal_final{
	cv::Rect rectangle;
	std::vector<cv::Point> approx_points;
	cv::Mat image_rect;
	cv::Mat image_bin;
	float angle;
};

void bin_to_RGB(const CImg<unsigned char> &input, CImg <unsigned char> & output);
void save_image(std::string imagename, database_mng & database, CImg <unsigned char> & image_save);
int numberOfSons(const std::vector<Vec4i> & hierarchy, int id);
cv::Point2f blobCenter(const std::vector<cv::Point> & points);
void computeContourDepth(const std::vector<Vec4i> & hierarchy, std::vector<int> & depth);
void calculate(set<string> images, int contador,database_mng & database, int resize, int out_option);
void search_rectangles(vector< dorsal > & dorsales ,vector<vector<Point> > & contours , vector<Vec4i> & hierarchy, Size & s,std::vector<int> depth);
void get_bibs(std::vector<Rect> & validBBs, Size & s, std::vector<std::vector<Rect> > & dorsales, std::vector<std::vector<int> > & num);
void no_repeat(std::vector<std::vector<Rect> > & dorsales_ordenados,std::vector<std::vector<int> > & num_ordenados,std::vector<std::vector<Rect> > & dorsales_finales, std::vector<std::vector<int> > & num_finales );
void OCR_proc(CImg <unsigned char> & dorsal_cimg, std::vector <int> &resultados);
void resize_own(const CImg <unsigned char> & input, CImg <unsigned char> & output, int size);
void binarizacion_adaptativa(const cimg_library::CImg<unsigned char> & input,cimg_library::CImg<unsigned char> & out_bin) ;

#endif /* TFG_IMG_FUNCTIONS_H_ */
