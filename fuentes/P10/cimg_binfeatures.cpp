#include <iostream>
#include <CImg.h>
#include "segmentutils.h"

using namespace cimg_library;
// THIS PROGRAM SHOULD NOT BE CHANGED


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


  int numobj;
  CImg<int> seg;
  //Binary segmentation.Segments foreground pixels into connected components. Pixels==0 ---> Label = 0
  int r = binarySegmentation(img, seg,numobj);
  if( r < 0)
	  exit(-1);
  std::cout <<"Numobjects = " << numobj << "\n";

  CImg<int> bbox;
  BoundingBox_cimg( seg, numobj, bbox);

  CImg<float> cdg;
  CImg<float> covarianzas;
  CImg<int> areas;
  Momentos_Areas_cimg( seg , cdg, covarianzas, areas , numobj);

  CImg<float> orientacion;
  CImg<float> autovalores;
  momentos2orientacion_autovalores( covarianzas, orientacion ,  autovalores);

  CImg<float> oriented_sizes;
  CImg<float> oriented_BBcenters;
  Oriented_BoundingBox_cimg( seg, orientacion, oriented_sizes, oriented_BBcenters);

  CImg<float> BBWidths(numobj + 1);
  CImg<float> BBHeights(numobj + 1);
  CImg<float> BBCenters(2,numobj + 1);
  CImg<float> Elipticidad(numobj + 1);
  for(int e=1; e <= numobj ; e++) {
	  BBWidths[e] = bbox(1,e) - bbox(0,e) +1;
	  BBHeights[e] = bbox(3,e) - bbox(2,e) +1;
	  BBCenters(0,e) = (bbox(1,e) + bbox(0,e)) / 2;
	  BBCenters(1,e) = (bbox(3,e) + bbox(2,e)) / 2;
	  Elipticidad[e] = autovalores(0,e) / autovalores(1,e);
  }

  FILE *fd = 0;
  fd =fopen("features.csv", "w");
  if( ! fd ) {
	  std::cerr<< "Can't open features.csv for writing\n";
	  exit(0);
  }
  fprintf(fd,"Label , " );
  	  fprintf(fd,"CDG_x , CDG_y ," );
  	  fprintf(fd,"BB_center_x , BB_center_y ,");
  	  fprintf(fd,"Area , ");
  	  fprintf(fd,"BB_width , BB_height ,");
  	  fprintf(fd,"Orientation  , Elipticicidad ," );
  	  fprintf(fd,"OrientBB_center_x , OrientBB_center_y ," );
  	  fprintf(fd,"OrientBB_majorSize , OrientBB_minorSize" );
  	  fprintf(fd,"\n");
  for(int e = 1; e <= numobj; e++) {
	  fprintf(fd,"%d , ", e ); // Label
	  fprintf(fd,"%.2f , %.2f ,", cdg(e,0), cdg(e,1) ); // Centre of gravity
	  fprintf(fd,"%.2f , %.2f ,", BBCenters(0,e), BBCenters(1,e) ); // Centre of BB
	  fprintf(fd,"%d , ", areas(e) ); // areas
	  fprintf(fd,"%.2f , %.2f ,", BBWidths(e), BBHeights(e) ); // Size of BB
	  fprintf(fd,"%.2f , %.2f ,", -orientacion(e), Elipticidad(e) ); // Major axis orientation, Elipticity 1: circle. \approx 0 very eliptic
	  //Orientacion tiene signo - para tener en cuenta que el eje 'y' va hacia abajo
	  fprintf(fd,"%.2f , %.2f ,", oriented_BBcenters(0,e), oriented_BBcenters(1,e) ); // Centre of oriented BB
	  fprintf(fd,"%.2f , %.2f", oriented_sizes(0,e), oriented_sizes(1,e) ); // Size of oriented BB
	  fprintf(fd,"\n");
  }
if(fd )
	fclose(fd);
  seg.display("Bin-Segmentation", false);
  CImg<unsigned char> colors;
  labels2colors(seg, colors);
  colors.display("Bin-Segment",false);

}
