

void router(set<string> images, int modo);
void proc_general(CImg<unsigned char> img_loaded);
bool busqueda_derecha(int o1, int o2, const cimg_library::CImg<int> & bbox);
bool busqueda_izquierda(int o1, int o2, const cimg_library::CImg<int> & bbox);
void busqueda(const cimg_library::CImg<int> & bbox, std::vector<std::vector<int> > & comienzos) ;
void show_parejas(std::vector<std::vector<int> > comienzos);
void seleccion_comienzos(std::vector<std::vector<int> > & comienzos, std::vector<std::vector<int> > & comienzos_seleccionados, cimg_library::CImg<int> & seg,
		cimg_library::CImg<int> & bbox, cimg_library::CImg<int> & areas);
void busqueda_tercera_cifra(std::vector<std::vector<int> > & comienzos_seleccionados, cimg_library::CImg<int> & bbox);
int SeleccionarEtiquetas_cimg(CImg<int> & segment, CImg<int> & tabla, int & numobj_out);
