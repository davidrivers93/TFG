/*
 * utilities.h
 *
 *  Created on: Apr 17, 2013
 *      Author: aalbiol
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

std::string get_extension(std::string & filename);
std::string remove_extension(std::string & filename);
std::string get_path(std::string & filename);
std::string get_basename(std::string & filename);

int isCamera(std::string &input);
bool isVideo(std::string & filename);
std::set< std::string>  isImages(std::string & filename);




#endif /* UTILITIES_H_ */
