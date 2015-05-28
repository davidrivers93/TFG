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

void qr_processing(CImg<unsigned char> img_qr);
float calc_distancia(Point2f P, Point2f Q);
float perpendicular_dist(Point2f A, Point2f B, Point2f C);
float calc_pendiente(Point2f A, Point2f B, int& alineamiento);
void calc_vertices(vector<vector<Point> > cont, int c_id, float pendiente, vector<Point2f>& quad);
void act_esquina(Point2f P, Point2f ref, float& baseline, Point2f& corner);
void act_esquinaOr(int orientacion, vector<Point2f> input, vector<Point2f> &output);
bool interseccion(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& interseccion);
float cross(Point2f v1, Point2f v2);

#endif /* TRABAJO_QR_PROC_H_ */
