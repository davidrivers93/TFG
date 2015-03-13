// Antonio Albiol 2012
// Universidad Politecnica Valencia, SPAIN

#include <iostream>
#include <CImg.h>

using namespace cimg_library;
#include "linfilters.h"

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

int main(int argc, char **argv) 
{

	const float angle = cimg_option("-a", 0, "Angle (degrees) of partial derivative");
	const int scheme = cimg_option("-s", 3, "Gradient scheme");
	const char * filename = cimg_option("-i", (char *) 0, "Input image name");
	cimg_usage("This computes gradients of an image");
	if(filename == 0) {
		std::cerr << "One argument needed. Option -i \n";
		exit(0);
	}

	CImg<unsigned char> img( filename );

	
	/****** gradient scheme //from cimg documentation *******
	-1 = Backward finite differences
	0 = Centered finite differences :fastest
	1 = Forward finite differences
	2 = Using Sobel masks
	3 = Using rotation invariant masks: default
	4 = Using Deriche recusrsive filter.
	
	*************************************************/
	int deltaSize = 9;
	CImg<float> delta(deltaSize,deltaSize,1,1,0);
	delta(deltaSize/2, deltaSize/2) = 1.0;
	CImgList<float> resp_imp = delta.get_gradient("xy",scheme);
	resp_imp[0].append(resp_imp[1],'x').display("Impulse Response",false);
	
	CImgList<float> grad = img.get_gradient("xy",scheme);
	

	float ang_rad = angle * 3.1416/180.0;
	CImg<float> g1 = grad[0]* cos(ang_rad) + grad[1] * sin(ang_rad);
	CImg<float> g2 = grad[0]* -sin(ang_rad) + grad[1] * cos(ang_rad);
	CImg<float> g = g1.get_append(g2,'x');
	g.display("Gradient",false);
	

}
