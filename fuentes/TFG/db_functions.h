/*
 * db_functions.h
 *
 *  Created on: 18/6/2015
 *      Author: David
 */

#ifndef TFG_DB_FUNCTIONS_H_
#define TFG_DB_FUNCTIONS_H_

#include "database_mng.h"
#include "db_functions.h"
#include "img_functions.h"
#include "misc_functions.h"

#include "CImg.h"
#include <opencv2/opencv.hpp>

using namespace cimg_library;
using namespace std;
using namespace cv;

void list_races(std::vector<string> & list_races, database_mng & database) ;
void add_result_db(std::string result, database_mng & database, std::string imagename);



#endif /* TFG_DB_FUNCTIONS_H_ */
