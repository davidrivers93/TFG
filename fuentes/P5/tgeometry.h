#ifndef TGEOMETRY_H
#define TGEOMETRY_H

#include <vector>

#define METHOD_NN 0
#define METHOD_BILINEAR 1

cimg_library::CImg<float> ComputeHomography(cimg_library::CImg<float> &x1, cimg_library::CImg<float>  &x2);
cimg_library::CImg<float> ComputeHomography(std::vector<cimg_library::CImg<float> > &x1, std::vector<cimg_library::CImg<float> > &x2);

cimg_library::CImg<float> TransformPoint(const cimg_library::CImg<float> & H, const cimg_library::CImg<float> & src);
cimg_library::CImg<float> InvertPoint(const cimg_library::CImg<float> & H, const cimg_library::CImg<float> & src);


cimg_library::CImgList<float> InvertPoints(const cimg_library::CImg<float> & H, const cimg_library::CImgList<float> & points);
cimg_library::CImgList<float> TransformPoints(const cimg_library::CImg<float> & H, const cimg_library::CImgList<float> & points);

int cimgPerspectiveTransform(const cimg_library::CImg<unsigned char> & in,
		const cimg_library::CImg<float> & H, int method,
		cimg_library::CImg<unsigned char> & out);

#endif
