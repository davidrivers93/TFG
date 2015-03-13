// Created by Antonio Albiol, Nov. 2012

/*******************        INCLUDE FILES       *******************************/




#include <string.h>

#include "CImg.h"
#if cimg_OS==2 //Windows
#include "getopt.h"
#else
#include <unistd.h>
#include <stdlib.h>
#endif
using namespace cimg_library;


void ayuda()
{

	fprintf(stderr,"Uso: cimg_apertura  [opciones] imagen_in [imagen_out] \n");
	fprintf(stderr,"Opciones:\n");
	fprintf(stderr,"\t -s size : Tamanyo del EE.\n");
	fprintf(stderr,"\t -x : Filtrar solo en horiz.\n");
	fprintf(stderr,"\t -y : Filtra solo en vertical.\n");
	fprintf(stderr,"\t -r : Calcula el residuo.\n");
	fprintf(stderr,"\t -v : Mostrar resultado en ventana. Si no se pasa imagen de salida muestra el resultado y sale\n");
	fprintf(stderr,"Funcion: Realiza una apertura  con cuadrados o segmentos horizontales o verticales \n");
}
int main(int argc, char **argv)
{

	int tam = 1;
	int opt;
	int pintarventana=0;
	char titulo[300];
	int filtx,filty;
	bool residue = false;

	filtx=filty=0;


	//Read command line options
	while((opt=getopt(argc,argv,"s:hvxyr"))!=-1) //
	{
		switch(opt)
		{
		case 'h':
			ayuda();
			exit(0);
			break;
		case 's':
			tam=atof(optarg);
			break;
		case 'v':
			pintarventana=1;
			break;
		case 'x':
			filtx=1;
			break;
		case 'y':
			filty=1;
			break;
		case 'r':
			residue = true;
			break;
		}
	}

	if(optind==argc)
	{
		ayuda();
		exit(0);
	}

	int tamx, tamy;
	tamx = tamy = tam;

	if( filtx )
		tamy = 1;
	if( filty )
		tamx = 1;
	CImg<unsigned char> entradaf(argv[optind]);

	CImg<unsigned char> apertura = entradaf.get_erode(tamx,tamy).dilate(tamx,tamy);

	if (residue )
		apertura =  entradaf - apertura;

	if(argc<=optind+1)
	{
		pintarventana=1;
		strcpy(titulo,argv[optind]);
	}
	else
	{
		apertura.save(argv[optind+1]);
		strcpy(titulo,argv[optind+1]);
	}

	if(pintarventana)
		apertura.display(titulo,0);

}

