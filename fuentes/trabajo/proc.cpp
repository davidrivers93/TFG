#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <time.h>

#include </opt/local/include/tesseract/baseapi.h>
#include </opt/local/include/leptonica/allheaders.h>

#if cimg_os==2 //Windows
#include "getopt.h"
#else
#include <unistd.h>
#include <stdlib.h>
#endif

#define cimg_use_opencv //To be able to use capture from camera in cimg
#define cimg_plugin "opencv.h"
#include "CImg.h"

/* OWN LIBRARIES
 *
 */

#include "utilities.h"
#include "opencv.h"
#include "misc.h"
#include "proc.h"

using namespace cimg_library;
using namespace std;
using namespace cv;

/*	function: router
 *
 * 	funcion que rutea todas las llamadas y centraliza todo.
 *
 */

void router(set<string> images, int modo){

//RECIBIMOS LA LISTA DE IMAGENES Y EL MODO!

	int proc_size = images.size();
	std::cout << "Comprobamos que el size es igual a antes: " << proc_size;

	CImg<int> contenedor_numobj(proc_size);
	CImg<int> contenedor_tiempo(proc_size);

	float totalTime;

	std::vector<std::vector<std::vector<int> > > contenedor_dorsales;

	std::set<string>::iterator it;

	int contador2=0;

	for (it = images.begin(); it != images.end(); it++, contador2++) {

		std::cout << "Imagen a procesar: " << *it << "\n";
		string imgname = *it;
		CImg<unsigned char> img_loaded(imgname.c_str());

		proc_general(img_loaded);



	}

}

/* function: thread_creator
 *
 * Crea los hilos para procesar las imagenes.
 * 		hilo 1->principal
 * 		hilo 2->OCR
 * 		hilo 3->tesseract
 */

void thread_creator(){



}

/*	function: proc_general
 *
 * 	funcion que realiza el procesado general de la imagen
 *
 */

void proc_general(CImg<unsigned char> img_loaded){

	CImg<unsigned char> img_out_binarizacion(img_loaded);
	binarizacion_adaptativa(img_loaded, img_out_binarizacion);

	CImg<int> bbox;
	CImg<int> areas;
	CImg<int> seg;

	segmentacion(img_out_binarizacion, seg, bbox, areas);

	std::vector<std::vector<int> > comienzos;
	busqueda(bbox, comienzos);

	show_parejas(comienzos);

	std::vector<std::vector<int> > comienzos_seleccionados;
	seleccion_comienzos(comienzos, comienzos_seleccionados, seg, bbox, areas);
	show_parejas(comienzos_seleccionados);

	busqueda_tercera_cifra(comienzos_seleccionados, bbox);

	std::cout << "Comienzos seleccionados con tercera cifra.\n";
	int numobj = bbox.height();
	CImg<int> tabla(numobj);
	tabla.fill(0);

	for (int h = 0; h < comienzos_seleccionados.size(); h++) {
		for (int h2 = 0; h2 < comienzos_seleccionados[h].size(); h2++) {
			int indice_objeto = comienzos_seleccionados[h][h2];
			tabla[indice_objeto] = 1;
		}
		std::cout << "\n";
	}


	CImg<int> seg2(seg);
	SeleccionarEtiquetas_cimg(seg2, tabla, numobj);

	seg2.display("A", false);
	seg2.save("temp.jpg");
	std::cout << "Imagen guardada temporalmente.";

}

void show_parejas(std::vector<std::vector<int> > comienzos){

	std::cout << "Comienzos.\n";

	for (int h = 0; h < comienzos.size(); h++) {
		for (int h2 = 0; h2 < comienzos[h].size(); h2++) {
			if (h2 == 0)
				std::cout << "Pareja: " << comienzos[h][h2];
			if (h2 != 0)
				std::cout << " " << comienzos[h][h2];
		}
		std::cout << "\n";
	}

}



/*	function: proc_OCR
 *
 * 	funcion que realiza el procesado OCR en hilo 2
 *
 */

void proc_OCR(){



}

/*	function: proc_OCR
 *
 * 	funcion que realiza el procesado tesseract en hilo 3
 *
 */

void proc_Tesseract(){



}

void load_dlm(){

}


/* function: binarizacion adaptativa
 *
 * funcion que toma una imagen en color y devuelve una imagen
 * binaria resuelta mediante el metodo de binarizacion adaptativa
 */

void binarizacion_adaptativa(const cimg_library::CImg<unsigned char> & input, cimg_library::CImg<unsigned char> & out_bin) {

	CImg<unsigned char> img_blur;

	float sigma = 15;
	float epsilon = 2;
	img_blur.assign(input);
	img_blur = input.get_blur(sigma);
	out_bin.assign(input.get_shared_channel(0));
	cimg_foroff(out_bin,off)
	{
		if (input(off) < img_blur(off) - epsilon) {
			out_bin(off) = 1;
		} else {
			out_bin(off) = 0;
		}
	}
}

bool busqueda_derecha(int o1, int o2, const cimg_library::CImg<int> & bbox) {

	/*BUSQUEDA DERECHA
	 *
	 * FUNCION QUE DADOS DOS INDICES DE OBJETO Y LOS BBOX
	 * DETERMINA SI EL OBJETO O2 SE ENCUENTRA A LA DERECHA DE 01
	 * CUMPLIENDO UNAS CARACTERISTICAS.
	 * EN CASO AFIRMATIVO DEVOLVEMOS TRUE
	 *
	 */

	int distancia_x = bbox(1, o2) - bbox(1, o1);
	int distancia_y = abs(bbox(3, o2) - bbox(3, o1));
	float h1 = (bbox(3, o1) - bbox(2, o1) + 1);
	float h2 = (bbox(3, o2) - bbox(2, o2) + 1);
	float w1 = (bbox(1, o1) - bbox(0, o1) + 1);
	float w2 = (bbox(1, o2) - bbox(0, o2) + 1);
	float ratioh = h2 / h1;
	float ratiow = w2 / w1;
	float ratio_xmax_xmin = float((bbox(1, o1) - bbox(0, o1))) / float(1.5);
	float distance_xmax_xmin = bbox(0, o2) - bbox(1, o1);
	if (distance_xmax_xmin > ratio_xmax_xmin) {
		return false;
	}
	if (ratioh > HEIGHT_RATIO_TOLERANCE || ratioh < 1.0 / HEIGHT_RATIO_TOLERANCE) {
		return false;
	}
	if (ratiow > WIDTH_RATIO_TOLERANCE || ratiow < 1.0 / WIDTH_RATIO_TOLERANCE) {
		return false;
	}
	if (distancia_y > h1 / 2.5) {
		return false;
	}
	if (distancia_x < w1 / 2.0) {

		return false;
	}
	if (distancia_x > cimg::max(w1, w2) * 2.0) {

		return false;
	}
	return true;
}

bool busqueda_izquierda(int o1, int o2, const cimg_library::CImg<int> & bbox) {

	/*BUSQUEDA IZQUIERDA
	 *
	 * FUNCION QUE DADOS DOS INDICES DE OBJETO Y LOS BBOX
	 * DETERMINA SI EL OBJETO O2 SE ENCUENTRA A LA IZQUIERDA DE 01
	 * CUMPLIENDO UNAS CARACTERISTICAS.
	 * EN CASO AFIRMATIVO DEVOLVEMOS TRUE
	 *
	 */

	int distancia_x = bbox(1, o2) - bbox(1, o1); //Si esta a la izquierda esta distancia sera negativa
	int distancia_y = abs(bbox(3, o2) - bbox(3, o1)); //Los condicionales de y seran iguales.
	float h1 = (bbox(3, o1) - bbox(2, o1) + 1);
	float h2 = (bbox(3, o2) - bbox(2, o2) + 1);
	float w1 = (bbox(1, o1) - bbox(0, o1) + 1);
	float w2 = (bbox(1, o2) - bbox(0, o2) + 1);
	float ratioh = h2 / h1;
	float ratiow = w2 / w1;
	float ratio_xmax_xmin = float((bbox(1, o1) - bbox(0, o1))) / float(2.0); //Positivo siempre
	float distance_xmax_xmin = bbox(0, o1) - bbox(1, o2); //Positivo siempre

	if (distance_xmax_xmin > ratio_xmax_xmin) { //Si el objeto esta muy lejos no es candidato a ser objeto a la izquierda
		return false;
	}
	if (ratioh > HEIGHT_RATIO_TOLERANCE || ratioh < 1.0 / HEIGHT_RATIO_TOLERANCE) //Si el cociente entre alturas es mayor que 1.5 o menor que 1.5 no es candidato
		return false;
	if (ratiow > WIDTH_RATIO_TOLERANCE || ratiow < 1.0 / WIDTH_RATIO_TOLERANCE) //Lo mismo que arriba pero con anchos
		return false;
	if (distancia_y > h1 / 2.5) // Si la distancia en y es mayor que el height1/2.5 no es candidato
		return false;

	if (distancia_x > w1 / 1.7) {
		return false;
	}

	if (-distancia_x > cimg::max(w1, w2) * 1.7) {
		return false;
	}
	return true;
}

void busqueda(const cimg_library::CImg<int> & bbox, std::vector<std::vector<int> > & comienzos) {

	/* FUNCION: Busqueda
	 * Recibe los bounding boxes asi como el vector de comienzos.
	 *	TYPICAL_RATIO = 1.2 && RATIO_TOLERANCE = 1.8
	 *	TYPICAL_RATIO / RATIO_TOLERANCE = 0.67
	 *	TYPICAL_RATIO * RATIO_TOLERANCE = 2.16
	 *	MIN_HEIGHT =12 && MAX_HEIGHT = 30
	 */

	comienzos.clear();
	float min_ratio = TYPICAL_RATIO / RATIO_TOLERANCE;
	float max_ratio = TYPICAL_RATIO * RATIO_TOLERANCE;
	int numobj = bbox.height() - 1;
	for (int o1 = 1; o1 <= numobj; o1++) {
		int numero_objeto = o1;
		int xmin1 = bbox(0, o1);
		int xmax1 = bbox(1, o1);
		int ymin1 = bbox(2, o1);
		int ymax1 = bbox(3, o1);
		int width1 = (xmax1 - xmin1);
		int height1 = (ymax1 - ymin1);

		if (height1 < MIN_HEIGHT || height1 > MAX_HEIGHT) {
			continue;
		}

		float ratio = float(height1) / float(width1);

		if (ratio < min_ratio || ratio > max_ratio) {
			continue;
		}

		int rf = 0;
		int lf = 0;

		int right_object_id = -1;

		for (int o2 = 1; o2 <= numobj; o2++) {
			/* Buscamos a la derecha. Para ello creamos un bucle for que recorra todos los objetos.
			 * Si ambos objetos son los mismos no se hace nada, si no se pasa a la funcion busqueda derecha
			 *
			 */
			if (o1 == o2) {
				continue;
			}
			rf = busqueda_derecha(o1, o2, bbox);

			if (rf) {
				right_object_id = o2;
				break;
			}
		}
		for (int o2 = 1; o2 <= numobj; o2++) {
			if (o1 == o2) {
				continue;
			}
			lf = busqueda_izquierda(o1, o2, bbox);
			if (lf) { //Hemos encontrado un objeto a la izquierda. Paramos el bucle y o1 no es candidato a comienzo dorsal.
				break;
			}
		}
		if (rf == true && lf == false) {
			std::vector<int> v(2);
			v[0] = o1;
			v[1] = right_object_id;
			comienzos.push_back(v);
		}
	}
}

void seleccion_comienzos(std::vector<std::vector<int> > & comienzos, std::vector<std::vector<int> > & comienzos_seleccionados, cimg_library::CImg<int> & seg,
		cimg_library::CImg<int> & bbox, cimg_library::CImg<int> & areas) {

	/* SELECCION_COMIENZOS
	 * FUNCION QUE DADOS EL VECTOR DE COMIENZOS, LA CAPA DE SEGMENTACION, LOS BBOX Y LAS AREAS
	 * CALCULA EL VECTOR DE COMIENZOS_SELECCIONADOS QUE CUMPLEN CIERTAS CARACTERISTICAS.
	 * ESTE VECTOR DE SALIDA SERA DE TAMA�O IGUAL O MENOS QUE EL DE ENTRADA.
	 */

	int height = seg.height();
	int height30 = 0.30 * height;
	int height70 = 0.65 * height;
	int width = seg.width();
	int width20 = 0.2 * width;
	int width80 = 0.9 * width;

	for (int o1 = 0; o1 < comienzos.size(); o1++) {

		int index = comienzos[o1][0];
		int area_bbox = (bbox(3, index) - bbox(2, index)) * (bbox(1, index) - bbox(0, index));
		float area_objeto = float(area_bbox) / float(areas(index));
		float area_imagen = seg.width() * seg.height();

		if (bbox(3, index) < height30) {
			continue;
		}
		if (bbox(2, index) > height70) {
			continue;
		}
		/*	if( areas(o1) < area_objeto/1000.0 )
		 continue;*/
		if (bbox(1, index) < width20)
			continue;
		if (bbox(0, index) > width80)
			continue;
		if (area_bbox > areas(index) * 2.0) {
			continue;
		}

		std::vector<int> v(2);
		v[0] = comienzos[o1][0];
		v[1] = comienzos[o1][1];
		comienzos_seleccionados.push_back(v);
	}

}

void busqueda_tercera_cifra(std::vector<std::vector<int> > & comienzos_seleccionados, cimg_library::CImg<int> & bbox) {

	/* BUSQUEDA_TERCERA_CIFRA
	 * FUNCION QUE DADO UN VECTOR DE COMIENZOS_SELECCIONADOS Y LOS BBOX
	 * BUSCA UNA POSIBLE TERCERA CIFRA PARA CADA PAREJA.
	 */

	int numobj = bbox.height() - 1;

	for (int o1 = 0; o1 < comienzos_seleccionados.size(); o1++) {
		int objeto_central = comienzos_seleccionados[o1][1];
		bool rf = false;
		int right_object_id;
		for (int o3 = 1; o3 <= numobj; o3++) {
			if (objeto_central == o3) {
				continue;
			}
			rf = busqueda_derecha(objeto_central, o3, bbox);
			if (rf) {
				right_object_id = o3;
				comienzos_seleccionados[o1].push_back(right_object_id);
				break;
			}
		}

	}
}

int SeleccionarEtiquetas_cimg(CImg<int> & segment, CImg<int> & tabla, int & numobj_out) {
	int k;
	int etiq;
	int numobj_in;

	numobj_in = tabla.size();
	etiq = 0;
	for (k = 0; k < numobj_in; k++) {
		if (tabla[k]) {
			etiq++;
			tabla[k] = etiq;
		}
	}

	numobj_out = etiq;
	cimg_foroff(segment,o)
	{
		if (segment[o] < numobj_in)
			segment[o] = tabla[segment[o]];
		else
			return -1; //He hencontrado una etiqueta demasiado grande en la capa de segmentaci������n.
	}

	return 0;
}

void segmentacion(const cimg_library::CImg<unsigned char> & img,
		cimg_library::CImg<int> & seg, cimg_library::CImg<int> & bbox,
		cimg_library::CImg<int> & areas) {

	/* FUNCION: Segmentacion
	 * recibe la imagen asi como los bbox, seg y areas.
	 */
	//aaaa
	int numobj;
	int r = binarySegmentation(img, seg, numobj);

	BoundingBox_cimg(seg, numobj, bbox);

	CImg<float> cdg;
	CImg<float> covarianzas;
	Momentos_Areas_cimg(seg, cdg, covarianzas, areas, numobj);
}

int binarySegmentation(const CImg<unsigned char> & im, CImg<int> & seg,
		int & numobj) {
	if (im.spectrum() > 1) {
		std::cerr << "Error at binarySegmentation. Image spectrum > 1\n";
		return -1;
	}
	seg = im.get_label(false);
	int maxLabel = seg.max();
	CImg<int> table(maxLabel + 1);
	table.fill(0);
	int npix = im.size();
	unsigned char const *p = im.data();
	for (int o = 0; o < npix; o++) {
		if (p[o]) {
			table[seg[o]] = 1;
		}
	}
	int r = SeleccionarEtiquetas_cimg(seg, table, numobj);
	return r;
}

int BoundingBox_cimg(const CImg<int> & segment, int numobjs, CImg<int> &bbox) {
	int e;
	int maxetiq;
	if (numobjs <= 0)
		maxetiq = segment.max();
	else
		maxetiq = numobjs;

	bbox.assign(4, maxetiq + 1);

	cimg_forY(bbox,k) //Inicializacion
	{
		bbox(1, k) = bbox(3, k) = -1;
		bbox(0, k) = segment.width() + 1;
		bbox(2, k) = segment.height() + 1;
	}

	cimg_forXY(segment,x,y)
	{
		e = segment(x, y);
		if (y < bbox(2, e)) //ymin
			bbox(2, e) = y;

		if (x < bbox(0, e)) //xmin
			bbox(0, e) = x;

		if (y > bbox(3, e)) //ymax
			bbox(3, e) = y;
		if (x > bbox(1, e)) //xmax
			bbox(1, e) = x;

	}

	return 0;
}


int Momentos_Areas_cimg(const CImg<int> &segment, CImg<float> & centros,
		CImg<float> & covarianzas, CImg<int>& Npuntos, int nobjetos) {
	if (-1 == nobjetos)
		nobjetos = segment.max();

	int e = 0;
	centros.assign(nobjetos + 1, 2);
	centros.fill(0.0);
	covarianzas.assign(2, 2, nobjetos + 1);
	covarianzas.fill(0.0);
	Npuntos.assign(nobjetos + 1);
	Npuntos.fill(0);

	//Calculamos la suma de las coordenadas y sus cuadrados
	cimg_forXY(segment,x,y)
	{
		e = segment(x, y);
		if (e) {
			Npuntos[e]++;
			centros(e, 0) += x;
			centros(e, 1) += y;
			covarianzas(0, 0, e) += (x * x);
			covarianzas(0, 1, e) += (x * y);
			covarianzas(1, 1, e) += (y * y);
		}
	}
	//Calculamos los centros de gravedad
	cimg_forX(centros,ee)
	{
		if (Npuntos(ee)) {
			centros(ee, 0) = centros(ee, 0) / Npuntos[ee];
			centros(ee, 1) = centros(ee, 1) / Npuntos[ee];
		} else
			centros(ee, 0) = centros(ee, 1) = -1;
	}

	cimg_forZ(covarianzas,ee)
	{
		if (Npuntos(ee)) {
			covarianzas(0, 0, ee) = covarianzas(0, 0, ee) / Npuntos[ee]
					- centros(ee, 0) * centros(ee, 0);
			covarianzas(1, 1, ee) = covarianzas(1, 1, ee) / Npuntos[ee]
					- centros(ee, 1) * centros(ee, 1);
			covarianzas(0, 1, ee) = covarianzas(0, 1, ee) / Npuntos[ee]
					- centros(ee, 0) * centros(ee, 1);
			covarianzas(1, 0, ee) = covarianzas(0, 1, ee);
		} else {
			covarianzas(0, 0, ee) = covarianzas(1, 1, ee) = -1;
			covarianzas(0, 1, ee) = covarianzas(1, 0, ee) = 0;
		}
	}

	return 0;
}

void extractObject(const cimg_library::CImg<int> & segment,
		const cimg_library::CImg<int> & bbox, int label, int margin,
		cimg_library::CImg<unsigned char> &output) {
	output.assign();
	int nobjects = bbox.height() - 1;
	if (label > nobjects)
		return;
	int xmin = bbox(0, label);
	int xmax = bbox(1, label);
	int ymin = bbox(2, label);
	int ymax = bbox(3, label);

	int w = xmax - xmin + 1;
	int h = ymax - ymin + 1;
	int xoff = -xmin + margin;
	int yoff = -ymin + margin;
	output.assign(w + 2 * margin, h + 2 * margin).fill(0);
	for (int x = xmin; x <= xmax; x++) {
		for (int y = ymin; y <= ymax; y++) {
			if (segment(x, y) != label)
				continue;
			output(x + xoff, y + yoff) = 1;
		}
	}
}

int OCR(cimg_library::CImg<float> & vectores,
		cimg_library::CImg<float> & lowres) {
	/*OCR
	 * FUNCION QUE DADA LA MATRIZ DE OCR Y UN OBJETO DE 3X3 CALCULA
	 * EL DIGITO QUE MAS SE PARECE AL OBJETO MEDIANTE EL METODO
	 * DE VECINO MAS PROXIMO.
	 */
	// Search NN
	int nmin = 0;
	float d = 0.0;
	for (int x = 0; x < vectores.width(); x++)
		d += (vectores(x, nmin) - lowres[x]) * (vectores(x, nmin) - lowres[x]);
	float dmin = d;
	for (int n = 1; n < vectores.height(); n++) {
		d = 0.0;
		for (int x = 0; x < vectores.width(); x++)
			d += (vectores(x, n) - lowres[x]) * (vectores(x, n) - lowres[x]);
		if (d < dmin) {
			dmin = d;
			nmin = n;
		}
	}
	int digit =nmin%10;

	return digit;

}

void load_dlm(cimg_library::CImg<float> & vectores) {
	std::vector<CImg<float> > vectores_dlm(10);
	CImg<float>intermedia(9,10);

	/* LOAD_DLM
	 *
	 * FUNCION QUE DADOS 10 VECTORES_DLM DE CADA UNA DE LAS CIFRAS
	 * CREA UNA MATRIZ CON ESOS 10 VECTORES DE TAMA�O 9 COLUMNAS
	 * 10 FILAS.
	 */

	vectores_dlm[0].load_dlm("trainer0.dlm");
	vectores_dlm[1].load_dlm("trainer1.dlm");
	vectores_dlm[2].load_dlm("trainer2.dlm");
	vectores_dlm[3].load_dlm("trainer3.dlm");
	vectores_dlm[4].load_dlm("trainer4.dlm");
	vectores_dlm[5].load_dlm("trainer5.dlm");
	vectores_dlm[6].load_dlm("trainer6.dlm");
	vectores_dlm[7].load_dlm("trainer7.dlm");
	vectores_dlm[8].load_dlm("trainer8.dlm");
	vectores_dlm[9].load_dlm("trainer9.dlm");

	int fila=0;
	int puntero1=0;

	for(int o1=0;o1 < 89; o1++){
		vectores(o1)=vectores_dlm[fila][puntero1];
		if(puntero1==8){
			puntero1=0;
			fila++;
			continue;
		}
		puntero1++;

	}
}

