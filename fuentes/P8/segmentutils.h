/*
 * segmentutils.h
 *
 *  Created on: Jun 27, 2012
 *      Author: aalbiol
 */

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE

#ifndef SEGMENTUTILS_H_
#define SEGMENTUTILS_H_

#include<vector>
int  otsu(cimg_library::CImg<float> & hist);
int SeleccionarEtiquetas_cimg(cimg_library::CImg<int>  & segment, cimg_library::CImg<int> & tabla, int & numobj_out);
int binarySegmentation(const cimg_library::CImg<unsigned char> & im, cimg_library::CImg<int> & seg, int & numobj);

int BoundingBox_cimg(const cimg_library::CImg<int> & segment,int maxetiq,cimg_library::CImg<int> &bbox);
int Momentos_Areas_cimg(const cimg_library::CImg<int> &segment,
		cimg_library::CImg<float> & centros, cimg_library::CImg<float> & covarianzas,
		cimg_library::CImg<int>& Npuntos , int nobjetos);
int momentos2orientacion_autovalores(cimg_library::CImg<float> & covarianza, cimg_library::CImg<float> & orientacion,cimg_library::CImg<float> & autovalores);
void labels2colors (const cimg_library::CImg<int> & seg, cimg_library::CImg<unsigned char> & out) ;
int Oriented_BoundingBox_cimg(const cimg_library::CImg<int> & segment, const cimg_library::CImg<float> & orientation,
		cimg_library::CImg<float> & sizes, cimg_library::CImg<float> & centers);
void extractObject(const cimg_library::CImg<int> & segment, const cimg_library::CImg<int> & bbox, int label, int margin,
		cimg_library::CImg<unsigned char> &output);

void object_points(const cimg_library::CImg<int> & segment, cimg_library::CImg<int>& Npuntos, std::vector < cimg_library::CImg<int> > & puntos) ;

int Segmentbin4_cimg(const cimg_library::CImg<unsigned char> & binimg,cimg_library::CImg<int> & seg,int & numobj);

int Segmentbin8_cimg(const cimg_library::CImg<unsigned char> & binimg,cimg_library::CImg<int> & seg,int & numobj);
int MeanColor(const cimg_library::CImg<int> & segment, const cimg_library::CImg<unsigned char> & colorimg, int maxetiq,cimg_library::CImg<float> &colores_medios);
void oriented_draw_rectangle(cimg_library::CImg<unsigned char> & im, const cimg_library::CImg<float> & oriented_BBcenters,
		const cimg_library::CImg<float> & oriented_sizes,
		const cimg_library::CImg<float> & orientacion ) ;

#endif /* SEGMENTUTILS_H_ */
