// This program segments an image and considers each connected component
// as a number and tries to classify it.

// Antonio Albiol, jan. 2013


// THIS PROGRAM SHOULD NOT BE CHANGED

#include <vector>
#include <iostream>
#include <CImg.h>
#include "P8/segmentutils.h"
using namespace cimg_library;

int main(int argc, char **argv) 
{

	if( argc < 2 ){
		std::cout << "At least one argument needed\n";
		exit(0);
	}
	//Read input image
	CImg<unsigned char> input(argv[1]);


// Load pattern vectors
	CImg<float> vectores;
	vectores.load_dlm("train2013b.dlm");


	//Make sure it's binary
	CImg<unsigned char> inputBin = input.get_threshold(128);

	//Segment
	CImg<int> seg;
	int numobj;
	binarySegmentation( inputBin, seg, numobj);


	//Compute BB
	CImg<int> bbox;
	BoundingBox_cimg( seg , numobj, bbox);


	char txt[100];
	int interpolation_method=2; //Moving average

	for (int o = 1; o <= numobj; o ++) {
		int h = bbox(3,o) -bbox(2,o)+1;
		if(h < 6) //Very small objects are ignored
			continue;

		CImg<unsigned char> object_img;
		extractObject( seg, bbox, o, 0, object_img);
		sprintf(txt,"Object %d",o);
		object_img.display("object",false);

		//To prevent finite precision effects, first we convert binary image to float...
		CImg<float> lowres = object_img;

		//... and then compute the low resolution image
		lowres.resize(3,3,-100,-100, interpolation_method);

		lowres.display("lowres",false);
		//This is because pattern vectors are stored columnwise and CImg orders pixel rowwise
		lowres.transpose();

		// Search NN
		int nmin = 0;
		float d = 0.0;
		for(int x=0; x< vectores.width(); x++)
			d+= (vectores(x,nmin)-lowres[x])*(vectores(x,nmin)-lowres[x]);
		float dmin = d;
		for(int n= 1; n< vectores.height(); n++) {
			d= 0.0;
			for(int x=0; x< vectores.width(); x++)
				d += (vectores(x,n)-lowres[x])*(vectores(x,n)-lowres[x]);
			if( d < dmin ){
				dmin = d;
				nmin = n;
			}
		}

		int digit =nmin%10;
		std::cout << "Best match: "<< nmin <<". Number is: " << digit << "\n";
		object_img.display("object",false);

	}

}
