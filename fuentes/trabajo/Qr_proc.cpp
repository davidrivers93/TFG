/*
 * Qr_proc.cpp
 *
 *  Created on: 25/5/2015
 *      Author: David
 *
 * Macrofuncion que procesara y desencriptara codigos QR. Recibira del programa principal los objetos identificados como
 * posibles codigos QR y devolvera -1 si no es codigo o en el caso de que sea codigo delvolvera el string.
 */



#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define cimg_plugin1 "cimgcvMat.h"
#include "CImg.h"
#include <iostream>
#include <cmath>
#include <zbar.h>

#include "Qr_proc.h"

using namespace cv;
using namespace std;
using namespace zbar;
using namespace cimg_library;


//Constantes identificacion

const int CV_QR_NORTE = 0;
const int CV_QR_ESTE = 1;
const int CV_QR_SUR = 2;
const int CV_QR_OESTE = 3;



//recibe una imagen en cv
void qr_processing(CImg<unsigned char> img_qr) {
	//Esto sobra cuando pasemos la imagen directamente
	//VideoCapture capture(0);
	//For image only
	//Mat image = imread(argv[1]);

	Mat image = img_qr.get_MAT();


	//capture >> image;

	int A, B, C;

	int DBG = 1;
	float dist_AB, dist_CA, dist_BC;
	int outlier, median1, median2;

	int mark_abajo, mark_derecha;
	int orientacion;

	int mark = 0;

	int top;

	float dist, pendiente;
	int alineamiento;

	//Creamos imagenes intermedias
	Mat gray(image.size(), CV_MAKETYPE(image.depth(), 1));
	Mat edges(image.size(), CV_MAKETYPE(image.depth(), 1));
	//CV_8UC3->Unsigned int 8 bit long
	Mat traces(image.size(), CV_8UC3);
	Mat qr, qr_raw, qr_gray, qr_thres;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	int key = 0;

		//cv_8uc3 -> depth=3;
		//cv_8uc1 -> depth=1;
		traces = Scalar(0, 0, 0);
		qr_raw = Mat::zeros(100, 100, CV_8UC3);
		qr = qr_raw;
		qr_gray = Mat::zeros(100, 100, CV_8UC1);
		qr_thres = qr_gray;

		//capture >> image;

		//Pasamos a grises
		cvtColor(image, gray, CV_RGB2GRAY);
		//imshow("Qr", gray);
		//Busqueda de vertices mediante metodo de canny
		Canny(gray, edges, 100, 200, 3);

		findContours(edges, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_TC89_L1);
		//imshow("edges", edges);
		//Calculamos momentos y centros de masas
		vector<Moments> mu(contours.size());
		vector<Point2f> mc(contours.size());

		for (int i = 0; i < contours.size(); i++) {
			mu[i] = moments(contours[i], false);
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}



		std:cout <<"Paso x aqui" << endl;

		/* mark -> numero de "marcadores" de orientacion. Minimo 3.
		 *
		 */
		for (int i = 0; i < contours.size(); i++) {
			int k = i;
			int c = 0;

			while (hierarchy[k][2] != -1) {
				k = hierarchy[k][2];
				c = c + 1;
				std::cout << "hierarchi[k][2]" << hierarchy[k][2] << endl;
				std::cout << "k: " << k << endl;
				system("clear");
			}

			if (hierarchy[k][2] != -1)
				c = c + 1;

			if (c > 5) {
				if (mark == 0)
					A = i;
				else if (mark == 1)
					B = i;
				else if (mark == 2)
					C = i;
				mark = mark + 1;
			}

		}

		std::cout << "A :" << A << endl;
		std::cout << "B :" << B << endl;
		std::cout << "C :" << C << endl;

		imshow("Edges.", edges);
		std::cout << "Marcas detectadas: " << mark << endl;
		if (mark >= 2) {
			/* DETERMINAMOS LA DISTANCIA DE LOS LADOS DE LOS TRIANGULOS.
			 *
			 */

			dist_AB = calc_distancia(mc[A], mc[B]);
			dist_CA = calc_distancia(mc[C], mc[A]);
			dist_BC = calc_distancia(mc[B], mc[C]);

			//Tomamos como outlier el punto que no pertenece a la hipotenusa del triangulo que forman A,B,C.

			if (dist_AB > dist_BC && dist_AB > dist_CA) {
				outlier = C;
				median1 = A;
				median2 = B;
			} else if (dist_CA > dist_AB && dist_CA > dist_BC) {
				outlier = B;
				median1 = A;
				median2 = C;
			} else if (dist_BC > dist_CA && dist_BC > dist_CA) {
				outlier = A;
				median1 = B;
				median2 = C;
			}

			//Tomamos como top el outlier. luego ya rotaremos.
			top = outlier;

			//Calculamos la distancia perpendicular del outlier

			dist = perpendicular_dist(mc[median1], mc[median2], mc[top]);
			pendiente = calc_pendiente(mc[median1], mc[median2], alineamiento);

			if (alineamiento == 0) {
				mark_abajo = median1;
				mark_derecha = median2;
			} else if (pendiente < 0 && dist < 0) {
				mark_abajo = median1;
				mark_derecha = median2;
				orientacion = CV_QR_NORTE;
			} else if (pendiente > 0 && dist < 0) {
				mark_abajo = median2;
				mark_derecha = median1;
				orientacion = CV_QR_ESTE;
			} else if (pendiente < 0 && dist > 0) {
				mark_abajo = median2;
				mark_derecha = median1;
				orientacion = CV_QR_SUR;

			} else if (pendiente > 0 && dist > 0) {
				mark_abajo = median1;
				mark_derecha = median2;
				orientacion = CV_QR_OESTE;
			}

			float area_arriba, area_abajo, area_derecha;

			if (top < contours.size() && mark_derecha < contours.size() && mark_abajo < contours.size() && contourArea(contours[top]) > 10
					&& contourArea(contours[mark_derecha]) > 10 && contourArea(contours[mark_abajo]) > 10) {

				vector<Point2f> A, B, C, temp_A, temp_B, temp_C;
				Point2f D;

				vector<Point2f> source_points, destination_points;

				Mat warp_matrix;

				calc_vertices(contours, top, pendiente, temp_A);
				calc_vertices(contours, mark_derecha, pendiente, temp_B);
				calc_vertices(contours, mark_abajo, pendiente, temp_C);

				act_esquinaOr(orientacion, temp_A, A);
				act_esquinaOr(orientacion, temp_B, B);
				act_esquinaOr(orientacion, temp_C, C);

				int iflag = interseccion(B[1], B[2], C[3], C[2], D);

				source_points.push_back(A[0]);
				source_points.push_back(B[1]);
				source_points.push_back(D);
				source_points.push_back(C[3]);

				destination_points.push_back(Point2f(0, 0));
				destination_points.push_back(Point2f(qr.cols, 0));
				destination_points.push_back(Point2f(qr.cols, qr.rows));
				destination_points.push_back(Point2f(0, qr.rows));

				if (source_points.size() == 4 && destination_points.size() == 4)
				{

					warp_matrix = getPerspectiveTransform(source_points, destination_points);
					warpPerspective(image, qr_raw, warp_matrix, Size(qr.cols, qr.rows));
					copyMakeBorder(qr_raw, qr, 10, 10, 10, 10, BORDER_CONSTANT, Scalar(255, 255, 255));

					cvtColor(qr, qr_gray, CV_RGB2GRAY);
					threshold(qr_gray, qr_thres, 127, 255, CV_THRESH_BINARY);

				}
				drawContours(image, contours, top, Scalar(255, 200, 0), 2, 8, hierarchy, 0);
				drawContours(image, contours, mark_derecha, Scalar(0, 0, 255), 2, 8, hierarchy, 0);
				drawContours(image, contours, mark_abajo, Scalar(255, 0, 100), 2, 8, hierarchy, 0);

				if (DBG == 1) {
					// Debug Prints
					// Visualizations for ease of understanding
					if (pendiente > 5)
						circle(traces, Point(10, 20), 5, Scalar(0, 0, 255), -1, 8, 0);
					else if (pendiente < -5)
						circle(traces, Point(10, 20), 5, Scalar(255, 255, 255), -1, 8, 0);

					// Draw contours on Trace image for analysis
					drawContours(traces, contours, top, Scalar(255, 0, 100), 1, 8, hierarchy, 0);
					drawContours(traces, contours, mark_derecha, Scalar(255, 0, 100), 1, 8, hierarchy, 0);
					drawContours(traces, contours, mark_abajo, Scalar(255, 0, 100), 1, 8, hierarchy, 0);

					// Draw points (4 corners) on Trace image for each Identification marker
					circle(traces, A[0], 2, Scalar(255, 255, 0), -1, 8, 0);
					circle(traces, A[1], 2, Scalar(0, 255, 0), -1, 8, 0);
					circle(traces, A[2], 2, Scalar(0, 0, 255), -1, 8, 0);
					circle(traces, A[3], 2, Scalar(128, 128, 128), -1, 8, 0);

					circle(traces, B[0], 2, Scalar(255, 255, 0), -1, 8, 0);
					circle(traces, B[1], 2, Scalar(0, 255, 0), -1, 8, 0);
					circle(traces, B[2], 2, Scalar(0, 0, 255), -1, 8, 0);
					circle(traces, B[3], 2, Scalar(128, 128, 128), -1, 8, 0);

					circle(traces, C[0], 2, Scalar(255, 255, 0), -1, 8, 0);
					circle(traces, C[1], 2, Scalar(0, 255, 0), -1, 8, 0);
					circle(traces, C[2], 2, Scalar(0, 0, 255), -1, 8, 0);
					circle(traces, C[3], 2, Scalar(128, 128, 128), -1, 8, 0);

					// Draw point of the estimated 4th Corner of (entire) QR Code
					circle(traces, D, 2, Scalar(255, 255, 255), -1, 8, 0);

					// Draw the lines used for estimating the 4th Corner of QR Code
					line(traces, B[1], D, Scalar(0, 0, 255), 1, 8, 0);
					line(traces, C[3], D, Scalar(0, 0, 255), 1, 8, 0);

					// Show the Orientation of the QR Code wrt to 2D Image Space
					int fontFace = FONT_HERSHEY_PLAIN;

					if (orientacion == CV_QR_NORTE) {
						putText(traces, "NORTH", Point(20, 30), fontFace, 1, Scalar(0, 255, 0), 1, 8);
					} else if (orientacion == CV_QR_ESTE) {
						putText(traces, "EAST", Point(20, 30), fontFace, 1, Scalar(0, 255, 0), 1, 8);
					} else if (orientacion == CV_QR_SUR) {
						putText(traces, "SOUTH", Point(20, 30), fontFace, 1, Scalar(0, 255, 0), 1, 8);
					} else if (orientacion == CV_QR_OESTE) {
						putText(traces, "WEST", Point(20, 30), fontFace, 1, Scalar(0, 255, 0), 1, 8);
					}

					// Debug Prints
				}
			}

		}

		//imshow("Image", image);
		//imshow("Traces", traces);
		//imshow("QR code", qr_thres);

		int width_decod = qr_thres.cols;
		int height_decod = qr_thres.rows;

		ImageScanner scanner;
		scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

		uchar *image_conv = (uchar *) qr_thres.data;
		Image image_decod(width_decod, height_decod, "Y800", image_conv, width_decod * height_decod);

		int n = scanner.scan(image_decod);

		for (Image::SymbolIterator symbol = image_decod.symbol_begin(); symbol != image_decod.symbol_end(); ++symbol) {
			vector<Point> vp;

			cout << "Decodificado " << symbol->get_type_name() << " con simbolo " << symbol->get_data() << "\n";
		}



}
/*
float calc_distancia(Point2f P, Point2f Q) {
	return sqrt(pow(abs(P.x - Q.x), 2) + pow(abs(P.y - Q.y), 2));
}

float perpendicular_dist(Point2f A, Point2f B, Point2f C) {
	float a, b, c, distance;
	//ax+by+c -> ecuacion de la recta
	a = -((B.y - A.y) / (B.x - A.x));
	b = 1.0;
	c = (((B.y - A.y) / (B.x - A.x)) * A.x) - B.y;

	distance = (a * C.x + (b * C.y) + c) / sqrt((a * a) + (b * b));

	return distance;
}

float calc_pendiente(Point2f A, Point2f B, int& alineamiento) {
	float dx, dy;
	dx = B.x - A.x;
	dy = B.y - A.y;

	if (dy != 0) {
		alineamiento = 1;
		return (dy / dx);
	} else {
		alineamiento = 0;
		return 0.0;
	}
}

void calc_vertices(vector<vector<Point> > cont, int c_id, float pendiente, vector<Point2f>& quad) {

	Rect box;
	//Creamos bounding box
	box = boundingRect(cont[c_id]);

	Point2f M0, M1, M2, M3;
	Point2f p_A, p_B, p_C, p_D, weight, p_X, p_Y, p_Z;

	p_A = box.tl();
	p_B.x = box.br().x;
	p_B.y = box.tl().y;
	p_C = box.br();
	p_D.x = box.tl().x;
	p_D.y = box.br().y;

	weight.x = (p_A.x + p_B.x) / 2;
	weight.y = (p_A.y);

	p_X.x = p_B.x;
	p_X.y = (p_B.y + p_C.y) / 2;

	p_Y.x = (p_C.x + p_C.y) / 2;
	p_Y.y = (p_C.y);

	p_Z.x = p_D.x;
	p_Z.y = (p_D.x + p_A.y) / 2;

	float dmax[4];
	dmax[0] = 0.0;
	dmax[1] = 0.0;
	dmax[2] = 0.0;
	dmax[3] = 0.0;

	float pd1, pd2;
	pd1 = 0.0;
	pd2 = 0.0;

	if (pendiente > 5 || pendiente < -5) {

		for (int i = 0; i < cont[c_id].size(); i++) {

			pd1 = perpendicular_dist(p_C, p_A, cont[c_id][i]);
			pd2 = perpendicular_dist(p_B, p_D, cont[c_id][i]);

			if ((pd1 >= 0.0) && (pd2 > 0.0)) {
				act_esquina(cont[c_id][i], weight, dmax[1], M1);
			} else if ((pd1 > 0.0) && (pd2 <= 0.0)) {
				act_esquina(cont[c_id][i], p_X, dmax[2], M2);
			} else if ((pd1 <= 0.0) && (pd2 < 0.0)) {
				act_esquina(cont[c_id][i], p_Y, dmax[3], M1);
			} else if ((pd2 < 0.0) && (pd2 >= 0.0)) {
				act_esquina(cont[c_id][i], p_Z, dmax[0], M1);
			} else
				continue;
		}

	} else {

		int halfx = (p_A.x + p_B.x) / 2;
		int halfy = (p_A.y + p_D.y) / 2;

		for (int i = 0; i < cont[c_id].size(); i++) {

			if ((cont[c_id][i].x < halfx) && (cont[c_id][i].y <= halfy)) {
				act_esquina(cont[c_id][i], p_C, dmax[2], M0);
			} else if ((cont[c_id][i].x >= halfx) && (cont[c_id][i].y < halfy)) {
				act_esquina(cont[c_id][i], p_D, dmax[3], M1);
			} else if ((cont[c_id][i].x > halfx) && (cont[c_id][i].y >= halfy)) {
				act_esquina(cont[c_id][i], p_A, dmax[0], M2);
			} else if ((cont[c_id][i].x <= halfx) && (cont[c_id][i].y > halfy)) {
				act_esquina(cont[c_id][i], p_B, dmax[1], M3);
			} else
				continue;
		}

	}

	quad.push_back(M0);
	quad.push_back(M1);
	quad.push_back(M2);
	quad.push_back(M3);

}

void act_esquina(Point2f P, Point2f ref, float& baseline, Point2f& corner) {

	float temp_distance;
	temp_distance = calc_distancia(P, ref);

	if (temp_distance > baseline) {

		baseline = temp_distance;
		corner = P;

	}

}

void act_esquinaOr(int orientacion, vector<Point2f> input, vector<Point2f> &output) {

	Point2f M0, M1, M2, M3;

	if (orientacion == CV_QR_NORTE) {
		M0 = input[0];
		M1 = input[1];
		M2 = input[2];
		M3 = input[3];
	}

	else if (orientacion == CV_QR_ESTE) {
		M0 = input[1];
		M1 = input[2];
		M2 = input[3];
		M3 = input[0];
	}

	else if (orientacion == CV_QR_SUR) {
		M0 = input[2];
		M1 = input[3];
		M2 = input[0];
		M3 = input[1];
	}

	else if (orientacion == CV_QR_OESTE) {
		M0 = input[3];
		M1 = input[0];
		M2 = input[1];
		M3 = input[2];
	}

	output.push_back(M0);
	output.push_back(M1);
	output.push_back(M2);
	output.push_back(M3);

}

bool interseccion(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& interseccion) {

	Point2f p = a1;
	Point2f q = b1;
	Point2f r(a2 - a1);
	Point2f s(b2 - b1);

	if (cross(r, s) == 0) {
		return false;
	}

	float t = cross(q - p, s) / cross(r, s);
	interseccion = p + t * r;
	return true;

}

float cross(Point2f v1, Point2f v2) {
	return v1.x * v2.y - v1.y * v2.x;
}*/


float calc_distancia(Point2f P, Point2f Q)
{
	return sqrt(pow(abs(P.x - Q.x),2) + pow(abs(P.y - Q.y),2)) ;
}


// Function: Perpendicular Distance of a Point J from line formed by Points L and M; Equation of the line ax+by+c=0
// Description: Given 3 points, the function derives the line quation of the first two points,
//	  calculates and returns the perpendicular distance of the the 3rd point from this line.

float perpendicular_dist(Point2f L, Point2f M, Point2f J)
{
	float a,b,c,pdist;

	a = -((M.y - L.y) / (M.x - L.x));
	b = 1.0;
	c = (((M.y - L.y) /(M.x - L.x)) * L.x) - L.y;

	// Now that we have a, b, c from the equation ax + by + c, time to substitute (x,y) by values from the Point J

	pdist = (a * J.x + (b * J.y) + c) / sqrt((a * a) + (b * b));
	return pdist;
}

// Function: Slope of a line by two Points L and M on it; Slope of line, S = (x1 -x2) / (y1- y2)
// Description: Function returns the slope of the line formed by given 2 points, the alignement flag
//	  indicates the line is vertical and the slope is infinity.

float calc_pendiente(Point2f L, Point2f M, int& alignement)
{
	float dx,dy;
	dx = M.x - L.x;
	dy = M.y - L.y;

	if ( dy != 0)
	{
		alignement = 1;
		return (dy / dx);
	}
	else				// Make sure we are not dividing by zero; so use 'alignement' flag
	{
		alignement = 0;
		return 0.0;
	}
}



// Function: Routine to calculate 4 Corners of the Marker in Image Space using Region partitioning
// Theory: OpenCV Contours stores all points that describe it and these points lie the perimeter of the polygon.
//	The below function chooses the farthest points of the polygon since they form the vertices of that polygon,
//	exactly the points we are looking for. To choose the farthest point, the polygon is divided/partitioned into
//	4 regions equal regions using bounding box. Distance algorithm is applied between the centre of bounding box
//	every contour point in that region, the farthest point is deemed as the vertex of that region. Calculating
//	for all 4 regions we obtain the 4 corners of the polygon ( - quadrilateral).
void calc_vertices(vector<vector<Point> > contours, int c_id, float slope, vector<Point2f>& quad)
{
	Rect box;
	box = boundingRect( contours[c_id]);

	Point2f M0,M1,M2,M3;
	Point2f A, B, C, D, W, X, Y, Z;

	A =  box.tl();
	B.x = box.br().x;
	B.y = box.tl().y;
	C = box.br();
	D.x = box.tl().x;
	D.y = box.br().y;


	W.x = (A.x + B.x) / 2;
	W.y = A.y;

	X.x = B.x;
	X.y = (B.y + C.y) / 2;

	Y.x = (C.x + D.x) / 2;
	Y.y = C.y;

	Z.x = D.x;
	Z.y = (D.y + A.y) / 2;

	float dmax[4];
	dmax[0]=0.0;
	dmax[1]=0.0;
	dmax[2]=0.0;
	dmax[3]=0.0;

	float pd1 = 0.0;
	float pd2 = 0.0;

	if (slope > 5 || slope < -5 )
	{

	    for( int i = 0; i < contours[c_id].size(); i++ )
	    {
		pd1 = perpendicular_dist(C,A,contours[c_id][i]);	// Position of point w.r.t the diagonal AC
		pd2 = perpendicular_dist(B,D,contours[c_id][i]);	// Position of point w.r.t the diagonal BD

		if((pd1 >= 0.0) && (pd2 > 0.0))
		{
		    act_esquina(contours[c_id][i],W,dmax[1],M1);
		}
		else if((pd1 > 0.0) && (pd2 <= 0.0))
		{
			act_esquina(contours[c_id][i],X,dmax[2],M2);
		}
		else if((pd1 <= 0.0) && (pd2 < 0.0))
		{
		    act_esquina(contours[c_id][i],Y,dmax[3],M3);
		}
		else if((pd1 < 0.0) && (pd2 >= 0.0))
		{
			act_esquina(contours[c_id][i],Z,dmax[0],M0);
		}
		else
		    continue;
             }
	}
	else
	{
		int halfx = (A.x + B.x) / 2;
		int halfy = (A.y + D.y) / 2;

		for( int i = 0; i < contours[c_id].size(); i++ )
		{
			if((contours[c_id][i].x < halfx) && (contours[c_id][i].y <= halfy))
			{
				act_esquina(contours[c_id][i],C,dmax[2],M0);
			}
			else if((contours[c_id][i].x >= halfx) && (contours[c_id][i].y < halfy))
			{
				act_esquina(contours[c_id][i],D,dmax[3],M1);
			}
			else if((contours[c_id][i].x > halfx) && (contours[c_id][i].y >= halfy))
			{
				act_esquina(contours[c_id][i],A,dmax[0],M2);
			}
			else if((contours[c_id][i].x <= halfx) && (contours[c_id][i].y > halfy))
			{
				act_esquina(contours[c_id][i],B,dmax[1],M3);
			}
	    	}
	}

	quad.push_back(M0);
	quad.push_back(M1);
	quad.push_back(M2);
	quad.push_back(M3);

}

// Function: Compare a point if it more far than previously recorded farthest distance
// Description: Farthest Point detection using reference point and baseline distance
void act_esquina(Point2f P, Point2f ref , float& baseline,  Point2f& corner)
{
    float temp_dist;
    temp_dist = calc_distancia(P,ref);

    if(temp_dist > baseline)
    {
        baseline = temp_dist;			// The farthest distance is the new baseline
        corner = P;						// P is now the farthest point
    }

}

// Function: Sequence the Corners wrt to the orientation of the QR Code
void act_esquinaOr(int orientation, vector<Point2f> IN,vector<Point2f> &OUT)
{
	Point2f M0,M1,M2,M3;
    	if(orientation == CV_QR_NORTE)
	{
		M0 = IN[0];
		M1 = IN[1];
	 	M2 = IN[2];
		M3 = IN[3];
	}
	else if (orientation == CV_QR_ESTE)
	{
		M0 = IN[1];
		M1 = IN[2];
	 	M2 = IN[3];
		M3 = IN[0];
	}
	else if (orientation == CV_QR_SUR)
	{
		M0 = IN[2];
		M1 = IN[3];
	 	M2 = IN[0];
		M3 = IN[1];
	}
	else if (orientation == CV_QR_OESTE)
	{
		M0 = IN[3];
		M1 = IN[0];
	 	M2 = IN[1];
		M3 = IN[2];
	}

	OUT.push_back(M0);
	OUT.push_back(M1);
	OUT.push_back(M2);
	OUT.push_back(M3);
}

// Function: Get the Intersection Point of the lines formed by sets of two points
bool interseccion(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& intersection)
{
    Point2f p = a1;
    Point2f q = b1;
    Point2f r(a2-a1);
    Point2f s(b2-b1);

    if(cross(r,s) == 0) {return false;}

    float t = cross(q-p,s)/cross(r,s);

    intersection = p + t*r;
    return true;
}

float cross(Point2f v1,Point2f v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}
