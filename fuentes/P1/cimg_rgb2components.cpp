// This programs shows how obtain
// The color components and luminance of a color image


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE


#include <iostream>
#include <string>
#include <CImg.h>

using namespace cimg_library;

int main(int argc, char **argv) 
{

  if(argc < 2) {
    std::cerr << "One argument needed\n";
    exit(0);
  }

  CImg<unsigned char> img( argv[1] );


  // Check if it is color
  if (img.spectrum() < 3) {
	  std::cerr << "Sorry, Not a color image.\n";
  }


  //Save components
  img.get_shared_channel(0).display("Red Channel",false);
  img.get_shared_channel(1).display("Green Channel",false);
  img.get_shared_channel(2).display("Blue Channel",false);

  CImg<unsigned char> grey = 0.30 * img.get_shared_channel(0) + 0.59 * img.get_shared_channel(1) + 0.11 * img.get_shared_channel(2);
  grey.display("Grey (Luminance)",false);

}
