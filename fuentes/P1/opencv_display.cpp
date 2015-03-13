// This programs shows how to read an image file
// and display it using OPENCV


// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE


#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{
  Mat image;
  if( argc < 2 )
    {
      printf( "No image name given \n" );
      return -1;
    }
  image = imread( argv[1], CV_LOAD_IMAGE_COLOR );

  if( !image.data )
    {
      printf( "No image data \n" );
      return -1;
    }

  //First load it as color
  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  imshow( "Display Image", image );

  //Wait for a key press
  waitKey(0);

  //Now as grayscale
  image = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  imshow( "Display Image", image );
  
//Wait for a key press
  waitKey(0);

  return 0;
}
