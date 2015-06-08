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
#include "functions.h"

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
<<<<<<< Updated upstream
void qr_processing(const CImg<unsigned char> & img, const CImg <unsigned char> & seg , std::vector<std::vector<std::vector <int > > > & target_marks_index, const CImg<int> & bbox, std::vector < std::string > &string_result) {

=======
void qr_processing(const CImg<unsigned char> & img, vector<vector<vector <int > > > & target_marks_index, const CImg<int> & bbox, vector < string > & string_result){
>>>>>>> Stashed changes
	std::cout << "Tamaño: " << target_marks_index.size() << "\n";

	std::vector <int> coordinates_qr(4);
	for (int i = 0; i < target_marks_index.size(); i++) {

		CImg<unsigned char> img_circles(img);
		CImg<unsigned char> seg2(seg);

		get_coordinates_qr(target_marks_index[i], bbox, coordinates_qr);
		CImg<unsigned char> image_crop(img);
		image_crop.crop(coordinates_qr[0], coordinates_qr[2], coordinates_qr[1], coordinates_qr[3]);
		//image_crop.display("A", false);


		const unsigned char color[] = { 255,128,64 };

		img_circles.draw_circle(coordinates_qr[0], coordinates_qr[2],30,color, 0.5);
		img_circles.draw_circle(coordinates_qr[1], coordinates_qr[2],30,color, 0.5);
		img_circles.draw_circle(coordinates_qr[0], coordinates_qr[3],30,color, 0.5);
		img_circles.draw_circle(coordinates_qr[1], coordinates_qr[3],30,color, 0.5);

		//img_circles.display("Circulos", false);
		/*seg.display("a",false);
		CImg<unsigned char> img_crop_object(seg);*/
		//seg2.get_append(img_circles,'x').display("false",false);

		Mat image2 = image_crop.get_MAT();

		Mat gray(image2.size(), CV_MAKETYPE(image2.depth(), 1));
		ImageScanner scanner;
		scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
		// obtain image date
		cvtColor(image2, gray, CV_RGB2GRAY);
		int width = image2.cols;
		int height = image2.rows;
		uchar *raw = (uchar *) gray.data;
		// wrap image data
		Image image(width, height, "Y800", raw, width * height);
		// scan the image for barcodes
		int n = scanner.scan(image);
		// extract results
		int i2=0;
		for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol, i2++) {
			vector<Point> vp;
			// do something useful with results
			cout << "decoded " << symbol->get_type_name() << " symbol \"" << symbol->get_data() << '"' << " " << endl;
			int n = symbol->get_location_size();
			for (int i = 0; i < n; i++) {
				vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
			}
			RotatedRect r = minAreaRect(vp);
			Point2f pts[4];
			r.points(pts);
			for (int i = 0; i < 4; i++) {
				line(gray, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 3);
			}
			cout << "Angle: " << r.angle << endl;
			std::string temp = symbol->get_data();
			std::cerr << "Simbolo: " << temp << "\n";
			string_result.push_back(temp);
			std:cerr << "He llegado a aqui" << "\n";
			std::cerr << i << "\n";

		}



	}


}


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
