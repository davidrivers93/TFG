/*
 * segmentutils.cpp
 *
 *  Created on: Jun 27, 2012
 *      Author: aalbiol
 */

// *** THIS FILE SHOULDN'T BE CHANGED IN THE COURSE
#include <math.h>
#include <set>
#include <list>
#include <iostream>
#include <CImg.h>
using namespace cimg_library;

#include "functions.h"

int otsu(CImg<float> & hist) {
	float total;
	CImg<float> prob;

	int nnonulos = 0;
	cimg_foroff(hist,i) {
		if(hist[i])
		nnonulos++;
	}
	if(nnonulos<2) //El histograma no tiene 2 bins !=0 -> No se puede segmentar
			{
				return 0;
			}

			total=hist.sum();
			prob=hist/total;
			CImg<float> q1( prob.size() );

			q1[0] = prob[0];
			for (int x=1; x < q1.size(); x++) {
				q1(x)=q1(x-1)+prob(x);
			}

			CImg<float> q2 = 1 - q1;

			CImg<float> mu1,
	mu2, xp;

	xp = prob;
	cimg_forX(xp,x)
	{
		xp(x) = x * prob(x);
	}
	float Sumxp = xp.sum();

	CImg<float> Sxp;
	Sxp = xp;
	for (int x = 1; x < q1.size(); x++) {
		Sxp(x) = Sxp(x - 1) + xp(x);
	}
	CImg<float> Sxp2;
	Sxp2 = Sumxp - Sxp;
	mu1 = xp;
	mu2 = xp;
	for (int x = 0; x < q1.size(); x++) {
		mu1(x) = Sxp(x) / q1(x);
		mu2(x) = Sxp2(x) / q2(x);
	}
	int posmax = -1;
	float maximo = -3;
	for (int x = 0; x < q1.size(); x++) {
		float v;
		float dm;
		dm = (mu1[x] - mu2[x]);
		v = q1[x] * q2[x] * dm * dm;
		if (v > maximo) {
			maximo = v;
			posmax = x;
		}
	}
	return posmax;
}

/**********************************************************+
 int SeleccionarEtiquetas_cimg(CImg<int>  & segment,CImg<int> tabla, int & numobj_out)

 Recibe una capa de segmentaci������n (el 0 no es fondo en la entrada necesariamente) y una tabla con tantos
 elementos como etiquetas tenga la imagen de entrada.

 Si para una etiqueta, tabla vale 0, dicha etiqueta se convierte a fondo (0). El resto de valores en tabla se consideran objetos v������lido. La numeraci������n de las etiquetas se compacta. En salida, tabla contiene la equivalencia de a qu������ se ha convertido cada etiqueta original.
 ************************************************************/

int SeleccionarEtiquetas_cimg(CImg<int> & segment, CImg<int> & tabla,
		int & numobj_out) {
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
} //Bounding Box2 Int cimg

/***********************************************
 *
 * int Oriented_BoundingBox_cimg(const CImg<int> & segment, CImg<float> & orientation, CImg<float> & sizes, CImg<float> & centers)
 * orientation (input): Matriz de 2x (nobjs+1) con las orientaciones en grados del eje mayor (grados)
 *
 * sizes(output): MAtriz de 2 x (nobjs+1) conteniendo en en sizes(0,etiq) la dimensi��n mayor del BB y en  conteniendo en en sizes(1,etiq)
 * la dimensi��n menor del BB.
 *
 *
 * centers(output) : MAtriz de 2 x (nobjs+1) con los centros de los BB
 *
 * ******************************************* */

int Oriented_BoundingBox_cimg(const CImg<int> & segment,
		const CImg<float> & orientation, CImg<float> & sizes,
		CImg<float> & centers) {
	int e;
	int maxetiq = orientation.size() - 1;

	sizes.assign(2, maxetiq + 1);
	centers.assign(2, maxetiq + 1);
	CImg<float> xmin(maxetiq + 1);
	CImg<float> xmax(maxetiq + 1);
	CImg<float> ymin(maxetiq + 1);
	CImg<float> ymax(maxetiq + 1);
	CImg<float> cosine(maxetiq + 1);
	CImg<float> sine(maxetiq + 1);
	CImg<char> initialized(maxetiq + 1);

	float grad2rad = atan(1.0) * 4.0 / 180.0;

	for (int e = 1; e < cosine.size(); e++) {
		cosine[e] = cos(orientation[e] * grad2rad);
		sine[e] = sin(orientation[e] * grad2rad);
	}

	initialized.fill(0);

	cimg_forXY(segment,x,y)
	{
		e = segment(x, y);
		float xx = x * cosine[e] + y * sine[e];
		float yy = -x * sine[e] + y * cosine[e];
		if (0 == initialized[e]) {
			xmin[e] = xmax[e] = xx;
			ymin[e] = ymax[e] = yy;
			initialized[e] = 1;
		} else {
			if (xx < xmin[e]) {
				xmin[e] = xx;
			} else if (xx > xmax[e]) {
				xmax[e] = xx;
			}

			if (yy < ymin[e]) {
				ymin[e] = yy;
			} else if (yy > ymax[e]) {
				ymax[e] = yy;
			}
		}

	}

	centers(0, 0) = segment.width() / 2;
	centers(1, 0) = segment.height() / 2;
	sizes(0, 0) = segment.width();
	sizes(1, 0) = segment.height();
	for (int e = 1; e < orientation.size(); e++) {
		float cx = (xmax[e] + xmin[e]) / 2;
		float cy = (ymax[e] + ymin[e]) / 2;
		centers(0, e) = cx * cosine[e] - cy * sine[e];
		centers(1, e) = cx * sine[e] + cy * cosine[e];

		sizes(0, e) = (xmax[e] - xmin[e] + 1);
		sizes(1, e) = (ymax[e] - ymin[e] + 1);

	}

	return 0;
} //Oriented_BoundingBox_cimg

/**********************************************************
 *
 *
 * centros(k,0): coord x del centro de masas del objeto kesimo
 * centros(k,1): coord y del centro de masas del objeto kesimo
 * covarianzas(0,0,k): sigma xx  del objeto k esimo
 * covarianzas(0,1,k): sigma xy  del objeto k esimo
 * covarianzas(1,0,k): sigma xy  del objeto k esimo
 * covarianzas(1,1,k): sigma yy  del objeto k esimo
 * Npuntos(k): Area del objeto k esimo
 *
 ****************************************************   */

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
} //int momentos_cimg

/*******************************************************
 int momentos2orientacion_autovalores(CImg<float> & covarianza, CImg<float> orientacion,CImg<float> & autovalores)

 covarianza (2x2x(numobjs+1))
 orientacion: (numbojs+1) direcci������n del eje mayor en grados
 autovalores:(2xnumobjs+1) El primero es el menor

 ******************************************************/

int momentos2orientacion_autovalores(CImg<float> & covarianza,
		CImg<float> & orientacion, CImg<float> & autovalores) {
	orientacion.assign(covarianza.depth());
	autovalores.assign(2, covarianza.depth());
	float f180_pi = 180.0 / 3.141592;
	cimg_forZ(covarianza,e)
	{
		if (e) {
			CImg<float> val, vec;
			CImg<float> M = covarianza.get_shared_slice(e);
			//	  M.print();
			M.eigen(val, vec);

			if (val(0) > val(1)) {
				autovalores(0, e) = val(1);
				autovalores(1, e) = val(0);
				if (vec(0, 0) > 0) //La componente x del autovector debe ser positiva
					orientacion[e] = atan2(vec(0, 1), vec(0, 0)) * f180_pi;
				else
					orientacion[e] = atan2(-vec(0, 1), -vec(0, 0)) * f180_pi;
			} else {
				autovalores(0, e) = val(0);
				autovalores(1, e) = val(1);
				if (vec(1, 0) > 0)
					orientacion[e] = atan2(vec(1, 1), vec(1, 0)) * 180.0
							/ 3.141592;
				else
					orientacion[e] = atan2(-vec(1, 1), -vec(1, 0)) * 180.0
							/ 3.141592;
			}
		} else {
			orientacion[e] = 0;
			autovalores(0, e) = -1;
			autovalores(1, e) = -1;
		}

	}
	return 0;
} //momentos2orientacion_autovalores

//Creates image of random color to visualize segmentation
void labels2colors(const CImg<int> & seg, CImg<unsigned char> & out) {
	out.assign(seg.width(), seg.height(), 1, 3, 0);

	CImg<unsigned char> table(1, seg.max() + 1, 1, 3);
	table.rand(0, 20);
	table *= 10;
	table += 50;
	cimg_forXY(seg,x,y)
	{
		int label = seg(x, y);
		if (label == 0)
			continue;
		for (int c = 0; c < 3; c++)
			out(x, y, 0, c) = table(0, label, 0, c);
	}
}

// Creates a small binary image (0/1) where pixels belonging to label 'label' are equal to 1 and the rest are set to
// 0.
// If margin > 0 it adds a black frame around the object.
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

void object_points(const cimg_library::CImg<int> & segment,
		cimg_library::CImg<int>& Npuntos,
		std::vector<cimg_library::CImg<int> > & puntos) {
	puntos.resize(Npuntos.size());
	for (int o = 1; o < Npuntos.size(); o++) {
		puntos[o].assign(2, Npuntos[o]);
	}
	CImg<int> counters(Npuntos.size());
	counters.fill(0);

	cimg_forXY(segment, x,y)
	{
		int label = segment(x, y);
		if (label == 0)
			continue;
		int pos = counters[label];
		puntos[label](0, pos) = x;
		puntos[label](1, pos) = y;
		counters[label]++;
	}

}

/**********************************************************************++
 int Segmentbin8_cimg(CImg<unsigned char> & binimg,CImg<int> & seg,int & numobj)

 ***********************************************************************/
int update_equivalence_classes(std::list<std::set<int> > & equivalence_classes,
		std::set<int> & neighborLabels) {
	if (neighborLabels.size() == 1) { //No need to update equivalence classes
		return *(neighborLabels.begin());
	}
	if (neighborLabels.size() > 1) {
		int firstLabel = *(neighborLabels.begin());
		std::list<std::set<int> >::iterator it;
		std::list<std::set<int> >::iterator itfirstclass =
				equivalence_classes.end();

		for (it = equivalence_classes.begin(); it != equivalence_classes.end();
				it++) {
			std::set<int>::iterator ite = it->find(firstLabel);
			if (ite != it->end())
				itfirstclass = it;
		}

		if (itfirstclass == equivalence_classes.end()) {
			std::cerr
					<< "Equivalence class not found. Probably an error in code\n";
			return 0;
		}

		std::set<int>::iterator itl = neighborLabels.begin();
		itl++; //Point to the second label

		for (; itl != neighborLabels.end(); itl++) {
			int label = *itl;
			for (it = equivalence_classes.begin();
					it != equivalence_classes.end(); it++) {
				std::set<int>::iterator ite = it->find(label);
				if (ite == it->end()) //Not in this equivalence class
					continue;
				if (it != itfirstclass) { //Merge equivalence classes

					itfirstclass->insert(it->begin(), it->end()); //Insert in the first equivalence class
					equivalence_classes.erase(it); // Remove Class
					break;
				}
			}

		}

		return firstLabel;
	}

	else { //Equivalence classes empty probably an error
		std::cerr << "Empty neighbor labels . Probably an error in code\n";
		return 0;
	}
}

int Segmentbin8_cimg(const CImg<unsigned char> & binimg, CImg<int> & seg,
		int & numobj) {

	int anch, alt;

	unsigned char const *src;
	int *des;
	long maxobj;
	int k;
	int f, c;
	int next_etiq;

	int nvecinos;
	int etiq_vecinos[4], et[4];
	int anch4d, anch4s;

	int num_etiq;
	int tam;

	if (binimg.spectrum() > 1)
		return -1;

	anch = binimg.width();
	alt = binimg.height();

	seg.assign(anch, alt).fill(0);
	maxobj = anch * alt;

	//inicializamos la tabla de equivalencias haciendo que cada etiqueta equivalga a si misma
	//for(k=0; k<maxobj;k++)
	// equival[k]=k;

	next_etiq = 1;
	src = binimg.data();
	des = seg.data();
	anch4d = anch;
	anch4s = anch;
	// ------------ PRIMERA FILA ----------------------------------
	// printf("Primera fila\n");

	std::list<std::set<int> > equivalence_classes;
	if (src[0]) {
		des[0] = next_etiq;

		std::set<int> newclass;
		newclass.insert(next_etiq);
		equivalence_classes.push_back(newclass);
		next_etiq++;
	} else
		des[0] = 0;

	for (c = 1; c < anch; c++) {
		if (src[c]) {
			if (des[c - 1])
				des[c] = des[c - 1];
			else {
				des[c] = next_etiq;
				std::set<int> newclass;
				newclass.insert(next_etiq);
				equivalence_classes.push_back(newclass);
				next_etiq++;
			}
		}	//if (src[c])
		else
			des[c] = 0;
	}	//for c

	//Resto de filas -----------------------------------------

	for (f = 1; f < alt; f++) {
		// printf(" fila %d\n",f);
		src = binimg.data() + f * anch4s;
		des = seg.data() + f * anch4d;

		//La primera columna: No se pueden producir fusiones de etiquetas
		c = 0;
		if (src[c]) {

			if (des[c - anch4d]) {
				des[c] = des[c - anch4d];
			}

			else {	//Si no hay vecinos,
				des[c] = next_etiq;
				std::set<int> newclass;
				newclass.insert(next_etiq);
				equivalence_classes.push_back(newclass);
				next_etiq++;
			}
		}	//if(src[0])
		else
			des[c] = 0;

		//Resto de columnas menos la ultima
		for (c = 1; c < anch - 1; c++) {
			if (src[c]) {
				std::set<int> neighborLabels;
				nvecinos = 0;
				if (des[c - 1]) {
					//				{
					//					et[nvecinos]=des[c-1];
					//					etiq_vecinos[nvecinos]=equival[des[c-1]];
					//					nvecinos++;
					neighborLabels.insert(des[c - 1]);
				}
				if (des[c - anch4d]) {
					//					et[nvecinos]=des[c-anch4d];
					//					etiq_vecinos[nvecinos]=equival[des[c-anch4d]];
					//					nvecinos++;
					neighborLabels.insert(des[c - anch4d]);
				}

				if (des[c - anch4d - 1]) {
					//					et[nvecinos]=des[c-anch4d-1];
					//					etiq_vecinos[nvecinos]=equival[des[c-anch4d-1]];
					//					nvecinos++;
					neighborLabels.insert(des[c - anch4d - 1]);
				}
				if (des[c - anch4d + 1]) {
					//					et[nvecinos]=des[c-anch4d+1];
					//					etiq_vecinos[nvecinos]=equival[des[c-anch4d+1]];
					//					nvecinos++;
					neighborLabels.insert(des[c - anch4d + 1]);
				}

				nvecinos = neighborLabels.size();
				if (nvecinos) {					//Equivalencias
					int label = update_equivalence_classes(equivalence_classes,
							neighborLabels);
					des[c] = label;
				} else {					//Si no hay vecinos,
					des[c] = next_etiq;
					std::set<int> newclass;
					newclass.insert(next_etiq);
					equivalence_classes.push_back(newclass);
					next_etiq++;
				}
			} else
				des[c] = 0;
		}

		//Ultima columna
		if (src[c]) {
			std::set<int> neighborLabels;
			if (des[c - 1]) {
				neighborLabels.insert(des[c - 1]);
			}
			if (des[c - anch4d]) {
				neighborLabels.insert(des[c - anch4d]);
			}
			if (des[c - anch4d - 1]) {
				neighborLabels.insert(des[c - anch4d - 1]);
			}

			nvecinos = neighborLabels.size();
			if (nvecinos) {					//Equivalencias

				int label = update_equivalence_classes(equivalence_classes,
						neighborLabels);
				des[c] = label;

			} else {					//Si no hay vecinos,
				des[c] = next_etiq;
				std::set<int> newclass;
				newclass.insert(next_etiq);
				equivalence_classes.push_back(newclass);
				next_etiq++;
			}
		} else
			des[c] = 0;

	}					//for f

	//Hacer la sustitucion final de etiquetas y encontrar cuantas hay diferentes

	if (equivalence_classes.size() == 0) { // All black pixels
		numobj = 0;
		return 0;

	}

	// At least one object
	std::list<std::set<int> >::iterator fitstClassIterator =
			equivalence_classes.begin();

	CImg<int> compacta(next_etiq);
	compacta.fill(0);

	for (int k = 1; k < next_etiq; k++) {
		std::list<std::set<int> >::iterator it;

		int label = 0;
		int i = 1;
		for (it = equivalence_classes.begin(); it != equivalence_classes.end();
				it++, i++) {
			std::set<int>::iterator ite = it->find(k);
			if (ite != it->end())
				label = i;
		}
		if (label == 0) {
			std::cerr << "Probably an error in compacting segmentation table\n";
		} else
			compacta[k] = label;
	}

	tam = anch * alt;

	des = seg.data();
	int * end = des + tam;
	for (; des < end; des++) {
		*des = compacta[*des];
	}
	numobj = equivalence_classes.size();

	return 0;
}

/**********************************************************************++
 int Segmentbin4_cimg(CImg<unsigned char> & binimg,CImg<int> & seg,int & numobj)

 ***********************************************************************/

int Segmentbin4_cimg(const CImg<unsigned char> & binimg, CImg<int> & seg,
		int & numobj) {

	int anch, alt;

	unsigned char const *src;
	int *des;
	long maxobj;
	int k;
	int f, c;
	int next_etiq;

	int nvecinos;
	int etiq_vecinos[4], et[4];
	int anch4d, anch4s;

	int num_etiq;
	int tam;

	if (binimg.spectrum() > 1)
		return -1;

	anch = binimg.width();
	alt = binimg.height();

	seg.assign(anch, alt).fill(0);
	maxobj = anch * alt;

	//inicializamos la tabla de equivalencias haciendo que cada etiqueta equivalga a si misma
	//for(k=0; k<maxobj;k++)
	// equival[k]=k;

	next_etiq = 1;
	src = binimg.data();
	des = seg.data();
	anch4d = anch;
	anch4s = anch;
	// ------------ PRIMERA FILA ----------------------------------
	// printf("Primera fila\n");

	std::list<std::set<int> > equivalence_classes;
	if (src[0]) {
		des[0] = next_etiq;

		std::set<int> newclass;
		newclass.insert(next_etiq);
		equivalence_classes.push_back(newclass);
		next_etiq++;
	} else
		des[0] = 0;

	for (c = 1; c < anch; c++) {
		if (src[c]) {
			if (des[c - 1])
				des[c] = des[c - 1];
			else {
				des[c] = next_etiq;
				std::set<int> newclass;
				newclass.insert(next_etiq);
				equivalence_classes.push_back(newclass);
				next_etiq++;
			}
		}	//if (src[c])
		else
			des[c] = 0;
	}	//for c

	//Resto de filas -----------------------------------------

	for (f = 1; f < alt; f++) {
		// printf(" fila %d\n",f);
		src = binimg.data() + f * anch4s;
		des = seg.data() + f * anch4d;

		//La primera columna: No se pueden producir fusiones de etiquetas
		c = 0;
		if (src[c]) {

			if (des[c - anch4d]) {
				des[c] = des[c - anch4d];
			}

			else {	//Si no hay vecinos,
				des[c] = next_etiq;
				std::set<int> newclass;
				newclass.insert(next_etiq);
				equivalence_classes.push_back(newclass);
				next_etiq++;
			}
		}	//if(src[0])
		else
			des[c] = 0;

		//Resto de columnas menos la ultima
		for (c = 1; c < anch - 1; c++) {
			if (src[c]) {
				std::set<int> neighborLabels;
				nvecinos = 0;
				if (des[c - 1]) {
					//				{
					//					et[nvecinos]=des[c-1];
					//					etiq_vecinos[nvecinos]=equival[des[c-1]];
					//					nvecinos++;
					neighborLabels.insert(des[c - 1]);
				}
				if (des[c - anch4d]) {
					//					et[nvecinos]=des[c-anch4d];
					//					etiq_vecinos[nvecinos]=equival[des[c-anch4d]];
					//					nvecinos++;
					neighborLabels.insert(des[c - anch4d]);
				}

				nvecinos = neighborLabels.size();
				if (nvecinos) {					//Equivalencias
					int label = update_equivalence_classes(equivalence_classes,
							neighborLabels);
					des[c] = label;
				} else {					//Si no hay vecinos,
					des[c] = next_etiq;
					std::set<int> newclass;
					newclass.insert(next_etiq);
					equivalence_classes.push_back(newclass);
					next_etiq++;
				}
			} else
				des[c] = 0;
		}

		//Ultima columna
		if (src[c]) {
			std::set<int> neighborLabels;
			if (des[c - 1]) {
				neighborLabels.insert(des[c - 1]);
			}
			if (des[c - anch4d]) {
				neighborLabels.insert(des[c - anch4d]);
			}

			nvecinos = neighborLabels.size();
			if (nvecinos) {					//Equivalencias

				int label = update_equivalence_classes(equivalence_classes,
						neighborLabels);
				des[c] = label;

			} else {					//Si no hay vecinos,
				des[c] = next_etiq;
				std::set<int> newclass;
				newclass.insert(next_etiq);
				equivalence_classes.push_back(newclass);
				next_etiq++;
			}
		} else
			des[c] = 0;

	}					//for f

	//Hacer la sustitucion final de etiquetas y encontrar cuantas hay diferentes

	if (equivalence_classes.size() == 0) { // All black pixels
		numobj = 0;
		return 0;

	}

	// At least one object
	std::list<std::set<int> >::iterator fitstClassIterator =
			equivalence_classes.begin();

	CImg<int> compacta(next_etiq);
	compacta.fill(0);

	for (int k = 1; k < next_etiq; k++) {
		std::list<std::set<int> >::iterator it;

		int label = 0;
		int i = 1;
		for (it = equivalence_classes.begin(); it != equivalence_classes.end();
				it++, i++) {
			std::set<int>::iterator ite = it->find(k);
			if (ite != it->end())
				label = i;
		}
		if (label == 0) {
			std::cerr << "Probably an error in compacting segmentation table\n";
		} else
			compacta[k] = label;
	}

	tam = anch * alt;

	des = seg.data();
	int * end = des + tam;
	for (; des < end; des++) {
		*des = compacta[*des];
	}
	numobj = equivalence_classes.size();

	return 0;
}

/*********************************************
 *
 * colores_medios es una imagen de (nobj+1)x3
 * colormedio (5,0): componente roja del color medio del objeto 5
 * colormedio (5,1): componente verde del color medio del objeto 5
 * colormedio (5,2): componente azul del color medio del objeto 5
 *
 */

int MeanColor(const cimg_library::CImg<int> & segment,
		const CImg<unsigned char> & colorimg, int maxetiq,
		cimg_library::CImg<float> &colores_medios) {
	colores_medios.assign(maxetiq + 1, colorimg.spectrum());
	CImg<float> count(maxetiq + 1);
	count.fill(0.0);
	colores_medios.fill(0.0);
	cimg_forXY(segment,x,y)
	{
		int label = segment(x, y);
		count[label]++;
		cimg_forC(colorimg, c)
		{
			colores_medios(label, c) += colorimg(x, y, 0, c);
		}
	}

	cimg_foroff(count,o)
	{
		if (count[o] == 0) {
			cimg_forY(colores_medios,y)
			{
				colores_medios(o, y) = -1.0;
			}
		} else {
			cimg_forY(colores_medios,y)
			{
				colores_medios(o, y) /= count[o];
			}
		}

	}

	return 0;
}

void oriented_draw_rectangle(cimg_library::CImg<unsigned char> & im,
		const cimg_library::CImg<float> & oriented_BBcenters,
		const cimg_library::CImg<float> & oriented_sizes,
		const cimg_library::CImg<float> & orientacion) {

	//Creada por: Ismael Mera - 2013. tdi
	// e = numero de objeto
	cimg_forY(oriented_BBcenters,e)
	{
		float PI = 3.14159265;
		float OBBx = oriented_BBcenters(0, e);
		float OBBy = oriented_BBcenters(1, e);
		float W = oriented_sizes(0, e);
		float H = oriented_sizes(1, e);
		float ang = orientacion[e] * PI / 180;
		float angc = (PI / 2) - ang;

		//Buscar los puntos medios del bounding box orientado
		//*****************************************************************************
		int Cxm1 = OBBx + W / 2 * cos(ang);
		int Cym1 = OBBy + W / 2 * sin(ang);

		int Cxm2 = OBBx + H / 2 * cos(angc);
		int Cym2 = OBBy - H / 2 * sin(angc);

		int Cxm3 = OBBx - W / 2 * cos(ang);
		int Cym3 = OBBy - W / 2 * sin(ang);

		int Cxm4 = OBBx - H / 2 * cos(angc);
		int Cym4 = OBBy + H / 2 * sin(angc);
		//****************************************************************************

		//Buscar los puntos extremos del bounding box orientado para dibujar lineas
		//****************************************************************************
		int Px1 = Cxm1 + H / 2 * cos(angc);
		int Py1 = Cym1 - H / 2 * sin(angc);

		int Px2 = Cxm1 - H / 2 * cos(angc);
		int Py2 = Cym1 + H / 2 * sin(angc);

		int Px3 = Cxm3 + H / 2 * cos(angc);
		int Py3 = Cym3 - H / 2 * sin(angc);

		int Px4 = Cxm3 - H / 2 * cos(angc);
		int Py4 = Cym3 + H / 2 * sin(angc);
		//****************************************************************************

		//Dibujar lineas entre los puntos extremos para formar rectangulo
		//****************************************************************************
		unsigned char yellow[] = { 255, 255, 0 };
		float opacity = 1.0;
		unsigned int pattern = ~0L;

		im.draw_line(Px1, Py1, Px2, Py2, yellow, opacity, pattern);
		im.draw_line(Px2, Py2, Px4, Py4, yellow, opacity, pattern);
		im.draw_line(Px4, Py4, Px3, Py3, yellow, opacity, pattern);
		im.draw_line(Px3, Py3, Px1, Py1, yellow, opacity, pattern);
		//****************************************************************************
	}
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

void binarizacion_adaptativa(const cimg_library::CImg<unsigned char> & input,
		cimg_library::CImg<unsigned char> & out_bin) {

	/*BINARIZACION_ADAPTATIVA
	 *
	 * FUNCION QUE DADA UNA IMAGEN CREA UNA IMAGEN BINARIA
	 * CREADA MEDIANTE EL METODO DE UMBRALIZACION ADAPTATIVA.
	 */

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
	//Salida binaria
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
	if (distance_xmax_xmin > ratio_xmax_xmin){
		return false;
		}
	if (ratioh > HEIGHT_RATIO_TOLERANCE || ratioh < 1.0 / HEIGHT_RATIO_TOLERANCE){
		return false;
	}
	if (ratiow > WIDTH_RATIO_TOLERANCE || ratiow < 1.0 / WIDTH_RATIO_TOLERANCE){
		return false;
	}
	if (distancia_y > h1 / 2.5){
		return false;
	}
	if (distancia_x < w1 / 2.0){

		return false;
	}
	if (distancia_x > cimg::max(w1, w2) * 2.0){

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


	if (distance_xmax_xmin > ratio_xmax_xmin){ //Si el objeto esta muy lejos no es candidato a ser objeto a la izquierda
		return false;
	}
	if (ratioh > HEIGHT_RATIO_TOLERANCE || ratioh < 1.0 / HEIGHT_RATIO_TOLERANCE)//Si el cociente entre alturas es mayor que 1.5 o menor que 1.5 no es candidato
		return false;
	if (ratiow > WIDTH_RATIO_TOLERANCE || ratiow < 1.0 / WIDTH_RATIO_TOLERANCE)//Lo mismo que arriba pero con anchos
		return false;
	if (distancia_y > h1 / 2.5) // Si la distancia en y es mayor que el height1/2.5 no es candidato
		return false;


	if (distancia_x > w1 / 1.7){
		return false;
	}

	if (-distancia_x > cimg::max(w1, w2) * 1.7){
		return false;
	}
	return true;
}

void busqueda(const cimg_library::CImg<int> & bbox,
		std::vector<std::vector<int> > & comienzos) {

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

		if (height1 < MIN_HEIGHT || height1 > MAX_HEIGHT){
			continue;
		}

		float ratio = float(height1) / float(width1);

		if (ratio < min_ratio || ratio > max_ratio){
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
			if (lf){ //Hemos encontrado un objeto a la izquierda. Paramos el bucle y o1 no es candidato a comienzo dorsal.
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

void segmentacion(const cimg_library::CImg<unsigned char> & img,
		cimg_library::CImg<int> & seg, cimg_library::CImg<int> & bbox,
		cimg_library::CImg<int> & areas, cimg_library::CImg<float> & cdg) {

	/* FUNCION: Segmentacion
	 * recibe la imagen asi como los bbox, seg y areas.
	 */
	//aaaa
	int numobj;
	int r = binarySegmentation(img, seg, numobj);

	BoundingBox_cimg(seg, numobj, bbox);

	CImg<float> covarianzas;
	Momentos_Areas_cimg(seg, cdg, covarianzas, areas, numobj);
}

void seleccion_comienzos(std::vector<std::vector<int> > & comienzos,
		std::vector<std::vector<int> > & comienzos_seleccionados,
		cimg_library::CImg<int> & seg, cimg_library::CImg<int> & bbox,
		cimg_library::CImg<int> & areas) {

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
		int area_bbox = (bbox(3, index) - bbox(2, index))
				* (bbox(1, index) - bbox(0, index));
		float area_objeto = float(area_bbox) / float(areas(index));
		float area_imagen = seg.width()*seg.height();

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

void busqueda_tercera_cifra(
		std::vector<std::vector<int> > & comienzos_seleccionados,
		cimg_library::CImg<int> & bbox) {

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

void calc_centro_masas(cimg_library::CImg<int> bbox, int &center_x, int &center_y){

	int xmin = bbox(0,0);
	int xmax = bbox(1,0);
	int ymin = bbox(2,0);
	int ymax = bbox(3,0);

	center_x = (xmin + xmax)/2;
	center_y = (ymin + ymax)/2;

}

void calc_ancho(cimg_library::CImg<int> bbox, int center_x, int center_y, int &anch_x, int &anch_y){

	int anch_x_2 = center_x - bbox(0,0);
	int anch_y_2 = center_y - bbox(2,0);
	anch_x = 25 * anch_x_2;
	anch_y = 20 * anch_y_2;

}

void candidates(std::vector< int> & candidates, const cimg_library::CImg <int> & seg, const cimg_library::CImg<int> & bbox ){

	int height = seg.height();
	for (int o1 = 1; o1< bbox.height(); o1++){

		int height30 = 0.30 * height;
		int height70 = 0.65 * height;
		int width = seg.width();
		int width20 = 0.2 * width;
		int width80 = 0.9 * width;

		if (bbox(3, o1) < height30) {
			continue;
		}

		if (bbox(2, o1) > height70) {
			continue;
		}

		if (bbox(1, o1) < width20)
			continue;

		if (bbox(0, o1) > width80)
			continue;

		candidates.push_back(o1);
	}
	std::cout << "Tamaño candidates " << candidates.size();

}

void busqueda_marcadores(const cimg_library::CImg<int> & bbox, std::vector<std::vector<int> > & comienzos_marcadores, const cimg_library::CImg<int> & areas, const cimg_library::CImg <float> cdg, const std::vector < int > & v_candidates){

	comienzos_marcadores.clear();
	int numobj = bbox.height() - 1;

	for (int o1 = 1; o1 <= v_candidates.size(); o1++) {

		int numero_objeto = v_candidates[o1];
		int xmin1 = bbox(0, numero_objeto);
		int xmax1 = bbox(1, numero_objeto);
		int ymin1 = bbox(2, numero_objeto);
		int ymax1 = bbox(3, numero_objeto);

		int width1 = (xmax1 - xmin1);
		int height1 = (ymax1 - ymin1);
		float ratio_hw = float(width1) / float(height1);

		if (ratio_hw < MIN_RATIO_MARK || ratio_hw > MAX_RATIO_MARK) {
			continue;
		}

		float centro1_x = (float(xmin1) + float(xmax1)) / 2;
		float centro1_y = (float(ymin1) + float(ymax1)) / 2;
		float dis_x_centro = 0.05 * centro1_x;
		float dis_y_centro = 0.05 * centro1_y;
		int inner_object_id = -1;

		float centro_masas_x_1 = cdg( numero_objeto,0);
		float centro_masas_y_1 = cdg( numero_objeto,1);

		float dis_x_centro_masas = 0.05 * centro_masas_x_1 ;
		float dis_y_centro_masas = 0.05 * centro_masas_y_1 ;

		for (int o2 = 1; o2 <= v_candidates.size(); o2++) {

			int numero_objeto_2 = v_candidates[o2];

			if (numero_objeto == numero_objeto_2)
				continue;

			int xmin2 = bbox(0,  numero_objeto_2);
			int xmax2 = bbox(1,  numero_objeto_2);
			int ymin2 = bbox(2,  numero_objeto_2);
			int ymax2 = bbox(3,  numero_objeto_2);

			int width2 = (xmax2 - xmin2);
			int height2 = (ymax2 - ymin2);

			float ratio_hw2 = float(width2) / float(height2);
			float centro2_x = (float(xmin2) + float(xmax2)) / 2;
			float centro2_y = (float(ymin2) + float(ymax2)) / 2;

			float centro_masas_x_2 = cdg(numero_objeto_2,0);
			float centro_masas_y_2 = cdg(numero_objeto_2,1);

			float area_bbox = float(width2 * height2);

			float ratio_area = float(area_bbox) / float(areas(o2));

			if (centro_masas_x_2  < centro_masas_x_1 - dis_x_centro_masas || centro_masas_x_2 > centro_masas_x_1 + dis_x_centro_masas){
				continue;
			}

			if (centro_masas_y_2  < centro_masas_y_1 - dis_y_centro_masas || centro_masas_y_2 > centro_masas_y_1 + dis_y_centro_masas){
				continue;
			}

			if (centro2_x < centro1_x - dis_x_centro || centro2_x > centro2_x + dis_x_centro){
				continue;
			}

			if (centro2_y < centro1_y - dis_y_centro || centro2_y > centro2_y + dis_y_centro){
				continue;
			}

			bool flag=false;

			if(xmin1 < xmax2 && xmax1 > xmax2 && ymin1 < ymin2 && ymax1 > ymax2){

				flag=true;
				std::vector<int> v(2);
				v[0] = numero_objeto;
				v[1] = numero_objeto_2;
				comienzos_marcadores.push_back(v);
				continue;

			}

		}

	}

}

void seleccion_marcadores(const std::vector<std::vector<int> > & comienzos,std::vector<std::vector<int> > & comienzos_seleccionados,cimg_library::CImg<int> & seg, const cimg_library::CImg<int> & bbox,const cimg_library::CImg<int> & areas  ){

	for (int o1 = 1; o1< comienzos.size(); o1++){

		int height = seg.height();
		int height30 = 0.30 * height;
		int height70 = 0.65 * height;
		int width = seg.width();
		int width20 = 0.2 * width;
		int width80 = 0.9 * width;

		int index_first = comienzos[o1][1];
		int area_bbox = (bbox(3, index_first) - bbox(2, index_first)) * (bbox(1, index_first) - bbox(0, index_first));

		if (bbox(3, index_first) < height30) {
			continue;
		}

		if (bbox(2, index_first) > height70) {
			continue;
		}

		if (bbox(1, index_first) < width20)
			continue;

		if (bbox(0, index_first) > width80)
			continue;

		if(area_bbox > areas(index_first) * 1.2) continue;

		std::vector <int> v(2);
		v[0] = comienzos[o1][0];
		v[1] = comienzos[o1][1];

		comienzos_seleccionados.push_back(v);

	}

}

void target_marks(const std::vector<std::vector<int> > & comienzos_seleccionados,std::vector<std::vector<std::vector < int > > > & target_marks,const cimg_library::CImg<int> & seg, const cimg_library::CImg<int> & bbox, const cimg_library::CImg<int> & areas){
	for(int o1 = 1 ; o1< comienzos_seleccionados.size(); o1 ++){

		int index_first_mark = comienzos_seleccionados[o1][1];
		int xmin1 = bbox(0, index_first_mark);
		int ymin1 = bbox(2, index_first_mark);
		float desviation_x = 0.02 * xmin1;
		float desviation_y = 0.02 * ymin1;
		if(search_targets(target_marks, index_first_mark)) continue;

		for(int o2 = 1; o2 < comienzos_seleccionados.size(); o2++){

			if(o1 == o2) continue;

			int index_second_mark = comienzos_seleccionados[o2][1];
			int xmin2 = bbox(0, index_second_mark);
			int ymin2 = bbox(2, index_second_mark);

			float ratio_areas = float(areas(index_first_mark))/float(areas(index_second_mark));

			if(ratio_areas < 0.98 || ratio_areas > 1.02) continue;

			for(int o3 = 1; o3 < comienzos_seleccionados.size(); o3++){

				if(o1 == o3) continue;
				if(o2 == o3) continue;

				int index_third_mark = comienzos_seleccionados[o3][1];
				int xmin3 = bbox(0, index_third_mark);
				int ymin3 = bbox(2, index_third_mark);
				float ratio_areas_2 = float(areas(index_first_mark))/float(areas(index_third_mark));
				if(ratio_areas_2 < 0.95|| ratio_areas_2 > 1.05) continue;

				std::vector <std::vector < int > > v(3);
				std::vector <int> temp1(2);
				std::vector <int> temp2(2);
				std::vector <int> temp3(2);
				temp1[0] = comienzos_seleccionados[o1][0];
				temp1[1] = comienzos_seleccionados[o1][1];
				v.push_back(temp1);
				temp2[0] = comienzos_seleccionados[o2][0];
				temp2[1] = comienzos_seleccionados[o2][1];
				v.push_back(temp2);
				temp3[0] = comienzos_seleccionados[o3][0];
				temp3[1] = comienzos_seleccionados[o3][1];
				v.push_back(temp3);
				target_marks.push_back(v);

			}
		}
	}

}

bool search_targets(const std::vector<std::vector<std::vector < int > > > & target_marks, int index){
	for (int h = 0; h < target_marks.size(); h++) {
		for (int h2 = 3; h2 < target_marks[h].size(); h2++) {
			for (int h3 = 0; h3 < target_marks[h][h2].size(); h3++) {
				int index_target = target_marks[h][h2][h3];
				if(index_target == index) return true;
			}
		}
	}

	return false;
}

void get_coordinates_qr(const std::vector<std::vector < int > > & target_marks, const cimg_library::CImg<int> & bbox, std::vector <int> & coordinates_qr){
	for (int h2 = 3; h2 < target_marks.size(); h2++) {
		for (int h3 = 0; h3 < target_marks[h2].size(); h3++) {
			int index = target_marks[h2][h3];
			if(h3 == 0 && h2 == 3) {

				coordinates_qr[0] = bbox(0,index);
				coordinates_qr[1] = bbox(1,index);
				coordinates_qr[2] = bbox(2,index);
				coordinates_qr[3] = bbox(3,index);

			}
			else{
				if(bbox(0,index) < coordinates_qr[0]) coordinates_qr[0] = bbox(0,index);
				if(bbox(1,index) > coordinates_qr[1]) coordinates_qr[1] = bbox(1,index);
				if(bbox(2,index) < coordinates_qr[2]) coordinates_qr[2] = bbox(2,index);
				if(bbox(3,index) > coordinates_qr[3]) coordinates_qr[3] = bbox(3,index);
			}

		}
	}

}
