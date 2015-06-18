/*
 * misc_functions.h
 *
 *  Created on: 18/6/2015
 *      Author: David
 */

#ifndef TFG_MISC_FUNCTIONS_H_
#define TFG_MISC_FUNCTIONS_H_

#include "database_mng.h"
#include "db_functions.h"
#include "img_functions.h"
#include "misc_functions.h"

#include "CImg.h"
#include <opencv2/opencv.hpp>

using namespace cimg_library;
using namespace std;
using namespace cv;

bool fileExists ( const char * filename);
std::string search_number(std::string result);
void create_txt_file();
std::string getOsName();
int showfiles(std::set<std::string> images);
void process_results(std::vector <int> resultados, std::vector <int> & resultados_norepeat);


#endif /* TFG_MISC_FUNCTIONS_H_ */
