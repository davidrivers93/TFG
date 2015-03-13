
#include <iostream>
#include <CImg.h>

using namespace cimg_library;
#include "tgeometry.h"

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

int main(int argc, char **argv) 
{


	const char * filename = cimg_option("-i", (char *) 0, "Input image name");
	const char * outfilename = cimg_option("-o", (char *) 0, "Output image name");
	const char * Hfilename = cimg_option("-H", (char *) 0, "H matrix filename (dlm)");
	int outwidth = cimg_option ("-owidth", -1, "Output width");
	int outheight = cimg_option ("-oheight", -1, "Output height");
	int method = cimg_option("-method", 0, "0: NN.   1:Bil");


	cimg_usage("This program applies the Homography Transform to an image");

	if(filename == 0) {
		std::cerr << "One input image needed. Option -i \n";
		exit(0);
	}

	CImg<unsigned char> img( filename );

	CImg<float> H;
	H.load_dlm(Hfilename);


	if (outwidth < 0 || outheight < 0) {
		std::cerr << "Determinining automatically output image size \n";
		CImg<float> x(1,3);
		x.fill(1.0);
		x(0)=0;
		x(1)=0;
		CImg<float> xp00 = H*x;
		x(0)=img.width()-1;
		x(1)=0;
		CImg<float> xpW0 = H*x;
		x(0)=0;
		x(1)=img.height()-1;
		CImg<float> xp0H = H*x;
		x(0)=img.width()-1;
		x(1)=img.height()-1;
		CImg<float> xpWH = H*x;

		//Normalize by third component
		for(int k=0;k<2; k++) {
			xp00(k) /= xp00(2);
			xpW0(k) /= xpW0(2);
			xpWH(k) /= xpWH(2);
			xp0H(k) /= xp0H(2);
		}
		CImg<float> origin = xp00.get_min(xpW0).get_min(xp0H).get_min(xpWH);
		CImg<float> endp = xp00.get_max(xpW0).get_max(xp0H).get_max(xpWH);

		float xmin = origin(0);
		float ymin = origin(1);
		float xmax = endp(0);
		float ymax = endp(1);
		float w = xmax-xmin;
		float h = ymax-ymin;

		CImg<float> Htranslate(3,3);
		Htranslate.identity_matrix();
		Htranslate(2,0) = -xmin;
		Htranslate(2,1) = -ymin;

		H = Htranslate * H;
		outwidth = w;
		outheight = h;

	}

	CImg<unsigned char> output(outwidth, outheight, 1, img.spectrum() );

	int fillValue = 0;
	output.fill(0);



	int r = cimgPerspectiveTransform(img, H,  method, output);


	if (r < 0) //Error
		exit(0);

	output.display("Homography",false);
	if (outfilename)
	output.save(outfilename);

}
