#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include<vector>

#define HEIGHT_RATIO_TOLERANCE 1.5
#define MAX_HEIGHT 200
#define MIN_HEIGHT 12
#define RATIO_TOLERANCE 1.8
#define TYPICAL_RATIO 1.2
#define WIDTH_RATIO_TOLERANCE 1.7
#define MIN_RATIO_MARK 0.95
#define MAX_RATIO_MARK 1.05

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

void load_dlm(cimg_library::CImg<float> & vectores);
void busqueda(const cimg_library::CImg<int> & bbox, std::vector<std::vector<int> > & comienzos);
bool busqueda_izquierda(int o1, int o2, const cimg_library::CImg<int> & bbox);
bool busqueda_derecha(int o1, int o2, const cimg_library::CImg<int> & bbox);
void binarizacion_adaptativa(const cimg_library::CImg<unsigned char> & input,cimg_library::CImg<unsigned char> & out_bin);
void segmentacion(const cimg_library::CImg<unsigned char> & img,cimg_library::CImg<int> & seg, cimg_library::CImg<int> & bbox,cimg_library::CImg<int> & areas, cimg_library::CImg<float> & cdg);
void seleccion_comienzos(std::vector<std::vector<int> > & comienzos,std::vector<std::vector<int> > & comienzos_seleccionados,cimg_library::CImg<int> & seg, cimg_library::CImg<int> & bbox,cimg_library::CImg<int> & areas  );
void busqueda_tercera_cifra(std::vector<std::vector<int> > & comienzos_seleccionados,cimg_library::CImg<int> & bbox);
int OCR(cimg_library::CImg<float> & vectores, cimg_library::CImg<float> & lowres);
void calc_centro_masas(cimg_library::CImg<int> bbox, int &center_x, int &center_y);
void calc_ancho(cimg_library::CImg<int> bbox, int center_x, int center_y, int &anch_x, int &anch_y);
void busqueda_marcadores(const cimg_library::CImg<int> & bbox, std::vector<std::vector<int> > & comienzos_marcadores, const cimg_library::CImg<int> & areas, const cimg_library::CImg <float> cdg, const std::vector < int > & v_candidates);
void target_marks(const std::vector<std::vector<int> > & comienzos_seleccionados,std::vector<std::vector<std::vector < int > > > & target_marks,const cimg_library::CImg<int> & seg, const cimg_library::CImg<int> & bbox, const cimg_library::CImg<int> & areas);
void seleccion_marcadores(const std::vector<std::vector<int> > & comienzos,std::vector<std::vector<int> > & comienzos_seleccionados,cimg_library::CImg<int> & seg, const cimg_library::CImg<int> & bbox,const cimg_library::CImg<int> & areas  );
bool search_targets(const std::vector<std::vector<std::vector < int > > > & target_marks, int index);
void get_coordinates_qr(const std::vector<std::vector < int > > & target_marks, const cimg_library::CImg<int> & bbox, std::vector <int> & coordinates_qr);
void candidates(std::vector< int> & candidates, const cimg_library::CImg <int> & seg, const cimg_library::CImg<int> & bbox );

#endif /* SEGMENTUTILS_H_ */
