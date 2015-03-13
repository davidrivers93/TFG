// This programs compares two images



// *** THIS FILE MUST BE COMPLETED IN THE COURSE


#include <iostream>
#include <CImg.h>

using namespace cimg_library;

int main(int argc, char **argv) 
{

  if(argc < 3) {
    std::cerr << "Two arguments needed\n";
    exit(0);
  }

  CImg<unsigned char> img1( argv[1] );
  CImg<unsigned char> img2( argv[2] );

 // If the images have different dimensions or different number of color channels
 // print message and exit


  // If the images have the same dimensions and number of color channels
  // check to see if every pixel is identical in both images.
  // If every pixel is identical print message and exit


  // If the images have the same dimensions and number of color channels
  // If at least one pixel is different
  // Display the signed difference image between both images.


}
