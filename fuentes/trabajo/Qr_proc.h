/*
 * Qr_proc.h
 *
 *  Created on: 28/5/2015
 *      Author: David
 */

#ifndef TRABAJO_QR_PROC_H_
#define TRABAJO_QR_PROC_H_

#include <CImg.h>
#include <opencv2/opencv.hpp>


using namespace cimg_library;
using namespace cv;

void qr_processing(const CImg<unsigned char> & img, const CImg <unsigned char> & seg , std::vector<std::vector<std::vector <int > > >&  target_marks_index, const CImg<int> & bbox, std::vector < std::string > & string_result);
float calc_distancia(Point2f P, Point2f Q);
float perpendicular_dist(Point2f A, Point2f B, Point2f C);
float calc_pendiente(Point2f A, Point2f B, int& alineamiento);
void calc_vertices(std::vector<std::vector<Point> > cont, int c_id, float pendiente, std::vector<Point2f>& quad);
void act_esquina(Point2f P, Point2f ref, float& baseline, Point2f& corner);
void act_esquinaOr(int orientacion, std::vector<Point2f> input, std::vector<Point2f> &output);
bool interseccion(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& interseccion);
float cross(Point2f v1, Point2f v2);

#endif /* TRABAJO_QR_PROC_H_ */
