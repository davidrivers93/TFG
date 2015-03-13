#include <iostream>
#include <CImg.h>
#include "segmentutils.h"

using namespace cimg_library;


int main(int argc, char **argv) 
{

  if(argc < 2) {
    std::cerr << "One argument needed\n";
    exit(0);
  }

  CImg<unsigned char> img( argv[1] );

  if( img.spectrum() > 1)
  {
	  std::cerr << "Image can not be colour\n";
	  exit(0);
  }

 //CImg standard segmentation into flat connected components
  CImg<int> seg = img.get_label(false);

  seg.display("CIMG-Segmentation",false);


  int numobj;
  //Binary segmentation.Segments foreground pixels into connected components. Pixels==0 ---> Label = 0
  int r = binarySegmentation(img, seg,numobj);
  if( r < 0)
	  exit(-1);
  std::cout <<"Numobjects = " << numobj << "\n";
  seg.display("Bin-Segmentation", false);
  CImg<unsigned char> colors;
  labels2colors(seg, colors);
  colors.display("Bin-Segment",false);

}
