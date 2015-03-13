// This programs shows how to read an image file
// and display it using CIMG



// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE


#include <iostream>
#include <CImg.h>

using namespace cimg_library;

int main(int argc, char **argv) 
{

  if(argc < 2) {
    std::cerr << "One argument needed\n";
    exit(0);
  }

  CImg<unsigned char> img( argv[1] );

  img.display("CIMG-DISPLAY");
}
