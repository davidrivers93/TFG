/*
 * utilities.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: aalbiol
 */
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <algorithm>
#include <set>

#include "utilities.h"
using namespace std;


static inline std::string &rtrim(std::string &s) {
	//s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	std::size_t found = s.find_last_not_of(" \n\r\t");
	if(found != std::string::npos)
		s.erase(found + 1);
	return s;
}

// trim from end
static inline std::string &ltrim(std::string &s) {
//	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	std::size_t found = s.find_first_not_of(" \n\r\t");
	if(found != std::string::npos)
		s.erase(0,found);
	return s;
}
// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

int isCamera(std::string &input) {
	int length = input.size();
	if( length != 1 )
		return -1;

	if (! isdigit( input[0] ) )
		return -1;

	return  atoi(input.c_str() );
}


std::string get_extension(std::string & filename) {
	std::string extension;
	int position = filename.find_last_of(".");

	if(position ==  string::npos )
		return std::string(); //No extension(position + 1);
	extension = filename.substr(position+1);
	return extension;
}

std::string remove_extension(std::string & filename) {
	std::string extension;
	int position = filename.find_last_of(".");

	if(position ==  string::npos )
		return filename;
	extension = filename.substr(0,position);
	return extension;
}

std::string get_path(std::string & filename) {

	std::string path;
	int found = filename.find_last_of("/\\");
	if (found == string::npos)
		return path;
	path = filename.substr(0,found);
	return path;
}


std::string get_basename(std::string & filename) {

	std::string basename;
	int found = filename.find_last_of("/\\");
	if (found == string::npos)
		return filename;
	basename = filename.substr(found+1);
	return basename;
}


bool isVideo(std::string & filename) {
	string extension = get_extension(filename);
	locale loc;

	// Convert to lowercase
	for (size_t i=0; i<extension.size(); ++i)
		extension[i] = tolower(extension[i],loc);

	set<string> extensions;
	//Add as many extensions as wanted
	extensions.insert("avi");
	extensions.insert("mpg");
	extensions.insert("mpeg");
	extensions.insert("mov");
	extensions.insert("mp4");
	extensions.insert("m4v");
	extensions.insert("mkv");

	return extensions.count(extension);
}


set< string>  isImages(std::string & filename) {
	string extension = get_extension(filename);
	locale loc;

	// Convert to lowercase
	for (size_t i=0; i<extension.size(); ++i)
		extension[i] = tolower(extension[i],loc);

	set<string> extensions;
	//Add as many extensions as wanted
	extensions.insert("jpg");
	extensions.insert("jpeg");
	extensions.insert("tiff");
	extensions.insert("tif");
	extensions.insert("pgm");
	extensions.insert("pbm");
	extensions.insert("bmp");


	bool is_Image = extensions.count(extension);
	std::set <string> imagenames;

	if( is_Image ) {
		imagenames.insert(filename);
		return imagenames;
	}

//Here it is a list file.
	 fstream filestr;

	 try {
	 filestr.open (filename.c_str(),fstream::in);
	 }
	 catch(...) {
		 std::cerr << "Could not open " << filename << "\n";
	 }

	  // >> i/o operations here <<



	if(! filestr.is_open() ) {
		std::cerr << "Could not open " << filename << "\n";
		return imagenames; //Empty
	}
	std::string name;
	while( ! filestr.eof() ) {
		filestr >> name ;
		name = trim( name ); // Remove white space from the beginning and end
		extension = get_extension(name);
		// Convert to lowercase
		for (size_t i=0; i<extension.size(); ++i)
			extension[i] = tolower(extension[i],loc);

		if( extensions.count(extension) ) {
			imagenames.insert(name);
		}

	}
	filestr.close();
	return imagenames;
}
