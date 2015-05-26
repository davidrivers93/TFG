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
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

//Constantes identificacion

const int CV_QR_NORTE=0;
const int CV_QR_ESTE=1;
const int CV_QR_SUR=2;
const int CV_QR_OESTE=3;

float calc_distancia(Point2f P, Point2f Q);
float perpendicular_dist(Point2f A, Point2f B,Point2f C);
float calc_pendiente(Point2f A, Point2f B, int& alineamiento);

//recibe una imagen en cv
int main(int argc, char **argv){
	//Esto sobra cuando pasemos la imagen directamente
	Mat image = imread(argv[1]);

	int A,B,C;


	//Creamos imagenes intermedias
	Mat gray(image.size(), CV_MAKETYPE(image.depth(),1));
	Mat edges(image.size(), CV_MAKETYPE(image.depth(),1));
	//CV_8UC3->Unsigned int 8 bit long
	Mat traces(image.size(), CV_8UC3);
	Mat qr,qr_raw,qr_gray,qr_thres;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//cv_8uc3 -> depth=3;
	//cv_8uc1 -> depth=1;
	traces = Scalar(0,0,0);
	qr_raw = Mat::zeros(100,100,CV_8UC3);
	qr = qr_raw;
	qr_gray = Mat::zeros(100,100,CV_8UC1);
	qr_thres = qr_gray;

	//Pasamos a grises
	cvtColor(image,gray,CV_RGB2GRAY);
	//Busqueda de vertices mediante metodo de canny
	Canny(gray, edges, 100 , 200, 3);


	findContours( edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//Calculamos momentos y centros de masas
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());

	for( int i = 0; i < contours.size(); i++ )
	{	mu[i] = moments( contours[i], false );
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}

	int mark=0;
	/* mark -> numero de "marcadores" de orientacion. Minimo 3.
	 *
	 */
	for (int i=0; i< contours.size(); i++){
		int k=i;
		int c=0;

		while(hierarchy[k][2] != -1){
				k = hierarchy[k][2];
				c = c+1;
		}

		if(hierarchy[k][2] != -1) c=c+1;

		if(c>5){
			if(mark==0) A = i;
			else if(mark ==1) B=i;
			else if(mark == 2) C=i;
			mark = mark + 1;
		}

	}

	if(mark>=3){
		/* DETERMINAMOS LA DISTANCIA DE LOS LADOS DE LOS TRIANGULOS.
		 *
		 */
		float dist_AB, dist_CA, dist_BC;
		dist_AB = calc_distancia(mc[A],mc[B]);
		dist_CA = calc_distancia(mc[C],mc[A]);
		dist_BC = calc_distancia(mc[B],mc[C]);

		int outlier,median1,median2;
		//Tomamos como outlier el punto que no pertenece a la hipotenusa del triangulo que forman A,B,C.

		if(dist_AB > dist_BC && dist_AB > dist_CA){
			outlier = A; median1=B; median2=B;
		}
		else if(dist_CA > dist_AB && dist_CA > dist_BC){
			outlier=B;median1=A;median2=C;
		}
		else if(dist_BC > dist_CA && dist_BC > dist_CA){
			outlier=C;median1=B;median2=A;
		}
		//Tomamos como top el outlier. luego ya rotaremos.
		int top=outlier;
		//Calculamos la distancia perpendicular del outlier
		float dist, pendiente;
		int alineamiento;
		dist = perpendicular_dist(mc[median1],mc[median2], mc[top]);
		pendiente = calc_pendiente(mc[median1], mc[median2], alineamiento);

		int mark_abajo, mark_derecha;
		int orientacion;

		if(alineamiento ==0){
			mark_abajo = median1;
			mark_derecha = median2;
		}
		else if(pendiente < 0 && dist < 0){
			mark_abajo = median1;
			mark_derecha=median2;
			orientacion = CV_QR_NORTE;
		}
		else if(pendiente > 0 && dist < 0){
			mark_abajo = median2;
			mark_derecha = median1;
			orientacion  = CV_QR_ESTE;
		}
		else if(pendiente < 0 &&  dist > 0){
			mark_abajo = median2;
			mark_derecha = median1;
			orientacion = CV_QR_SUR;

		}
		else if (pendiente > 0 && dist >0){
			mark_abajo = median1;
			mark_derecha = median2;
			orientacion = CV_QR_OESTE;
		}

		float area_arriba, area_abajo, area_derecha;

		if( top < contours.size() && right < contours.size() && mark_abajo < contours.size() && contourArea(contours[top]) > 10 && contourArea(contours[mark_derecha]) > 10 && contourArea(contours[mark_abajo]) > 10){

			vector<Point2f>A, B, C, temp_A, temp_B, temp_C;
			Point2f D;

			vector<Point2f> source_points, destination_points;

			Mat warp_matrix;




		}
	}

}

float calc_distancia(Point2f P, Point2f Q){
	return sqrt(pow(abs(P.x - Q.x),2) + pow(abs(P.y - Q.y),2));
}

float perpendicular_dist(Point2f A, Point2f B,Point2f C){
	float a,b,c, distance;
	//ax+by+c -> ecuacion de la recta
	a = -((B.y - A.y)/(B.x - A.y));
	b = 1.0;
	c = (((B.y - A.y) / (B.x - A.x)) * A.x) - B.y;

	distance = (a * C.x + (b * C.y) + c) / sqrt((a*a) +(b*b));

	return distance;
}

float calc_pendiente(Point2f A, Point2f B, int& alineamiento){
	float dx,dy;
	dx = A.x - B.x;
	dy = A.y - B.y;

	if( dy != 0){
			alineamiento = 1;
			return (dy/dx);
	}
	else{
			alineamiento = 0;
			return 0.0;
	}
}

float calc_vertices(vector<vector<Point>> cont, int c_id, float pendiente, vector<Point2f>& quad){

	Rect box;
	//Creamos bounding box
	box = boundingRect(cont[c_id]);

	Point2f p_A, p_B, p_C, p_D, weight, p_X, p_Y, p_Z;

	p_A = box.tl();
	p_B.x = box.br().x;
	p_B.y = box.tl().y;
	p_C = box.br();
	p_D.x = box.tl().x;
	p_D.y = box.br().y;

	weight.x = (p_A.x+ p_B.x)/2;
	weight.y = (p_A.y);

	p_X.x = p_B.x;
	p_X.y = (p_B.y + p_C.y)/2;

	p_Y.x = (p_C.x + p_C.y)/2;
	p_Y.y = (p_C.y);

	p_Z.x = p_D.x;
	p_Z.y =(p_D.x + p_A.y)/2;

	float dmax[4];
	dmax[0]=dmax[1]=dmax[2]=dmax[3]=0.0;

	float pd1,pd2;
	pd1=pd2=0.0;


	return 0;



}
