/*
 * rw_xml.h
 *
 *  Created on: 12/2/2015
 *      Author: David
 */

#ifndef TRABAJO_RW_XML_H_
#define TRABAJO_RW_XML_H_

using namespace cimg_library;
using namespace std;

int xml_read();

void xml_write(	std::vector<std::vector<std::vector<int> > > *contenedor_dorsales, CImg<int> *contenedor_tiempo, CImg<int> *contenedor_numobj);


#endif /* TRABAJO_RW_XML_H_ */
