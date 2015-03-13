/*
 * otsu.cpp
 *
 *  Created on: Jun 28, 2012
 *      Author: aalbiol
 */


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

#include <iostream>
#include <CImg.h>
#include "segmentutils.h"

using namespace cimg_library;


int main(int argc, char **argv)
{

  if(argc < 4) {
    std::cerr << "Uso: cimg_umbral -u 57 imagen [resultado]\n";
    exit(0);
  }

  int th = cimg_option("-u",-1,"Valor del umbral");

  if (th < 0 ) {
	  std::cout << "Debe darse la opci—n -u\n";
	  std::cerr << "Uso: cimg_umbral -u 57 imagen [resultado]\n";
	  exit(0);
  }

  CImg<unsigned char> img( argv[3] );

  if( img.spectrum() > 1)
  {
	  std::cerr << "Image can not be colour\n";
	  exit(0);
  }


  CImg<unsigned char>  imth = 255 * img.get_threshold(th);
  img.get_append(imth,'x').display("Thresholded", false);
  if(argc >= 5) {
	  imth.save(argv[4]);
	  std::cout << "Saving result at " << argv[4] << "\n";
  }

}
