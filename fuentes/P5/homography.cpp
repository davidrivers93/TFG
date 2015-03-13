//#include "cimg_affine.h"
#include "CImg.h"
using namespace cimg_library;
#undef min
#undef max
#include <iostream>
#include <vector>
#include "tgeometry.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if cimg_OS==2 //Windows
#include  <windows.h>
#endif
// Main procedure
//---------------
int main(int argc, char **argv) {
	
	
	

	int save_counter=0;
	//	char file_op[256] = "polygon.dlm";
	//  const char *file_oim = "marked_image.jpg";
	
	
	// Read command line parameters
	//-----------------------------
	cimg_usage("Homography Creator");
        if(argc< 3)
          {
            std::cout<<"Usage: homgrafia srcimg dstimg\n";
          }
	const char *file_i0 = argv[argc-2];
	const char *file_i1 = argv[argc-1];
	
	//const float contrast = cimg_option("-contrast",0.6f,"Image contrast");
	
	const char *file_ip = cimg_option("-ip",(char*)0,"Input control origin points");
	
	//	const char *file_oc = cimg_option("-oc",(char*)0,"Output curve points");
	//	*file_op = cimg_option("-op",(char)0,"Output control points");
	//	const char *file_od = cimg_option("-od",(char*)0,"Output distance function");
	bool interp = true;
	//	bool closed = cimg_option("-closed",true,"Closed curve");
	//	bool show_tangents = cimg_option("-tangents",false,"Show tangents");
	bool show_points = true;
	bool show_projection = false;
	bool show_indices = true;
	bool show_coordinates = false;
	//	const float precision = cimg_option("-prec",0.05f,"Precision of curve discretization");
	
	bool save = false;
	
      if(argc< 3)
          {
            exit(0);
          }	// Init image data
	//-----------------
	const unsigned char yellow[] = { 255,255,0 }, white[] = { 255,255,255 }, green[] = { 0,255,0 },
	red[] = { 255,0,0 },  black[] = { 0,0,0 };
	CImg<unsigned char> img0, img1, img_orig, img_dest, help_img;
	if (file_i0 && file_i1) {
		std::fprintf(stderr,"\n - Load input images '%s' and '%s'\n ",cimg::basename(file_i0),cimg::basename(file_i1));
		img0 = CImg<>(file_i0).normalize(0,255.0f);
		img1 = CImg<>(file_i1).normalize(0,255.0f);
		//std::fprintf(stderr,"Size = %dx%dx%dx%d \n",img0.width(),img0.height(),img0.depth(),img0.spectrum());
		img0.resize(-100,-100,1,3);
		img1.resize(-100,-100,1,3);
	}
	else {
		std::fprintf(stderr,"\n - No input image specified\n");
		exit(0);
		
	}
	
	
	help_img.assign(220,210,1,3,0).
    draw_text(5,5,
			  "------------------------------------------\n"
			  "Homography Annotator\n"
			  "------------------------------------------\n"
			  "Left button : Create or move control point\n"
			  "Right button : Delete control point\n"
			  "Key 'P' : Show/hide control points\n"
			  "Key 'O' : Show/hide projected points\n"
			  "Key 'N' : Show/hide points indices\n"
			  "Key 'U' : Show/hide points coordinates\n"
			  "Key 'H' : Show/hide this help\n"
			  "Key 'S' : Save points and homography matrix\n"
			  "Key 'R' : Reset points\n",
              green);
	
	
	CImgDisplay disp_orig(img0,"Src View",0);
	CImgDisplay disp_dest(img1,"Dst View",0);


	CImgList<float> points, opoints, dpoints;
	bool omoving = false, dmoving = false, help = !file_i0;
	
	CImg<float> H;
	
	//TODO Que lea los puntos si es una archivo de 4 columnas y los asigne a opoints y dpoints
	if (file_ip) {
		std::fprintf(stderr," - Load input control points '%s' : ",cimg::basename(file_ip));
		points = CImg<>(file_ip).transpose()<'x';
		points.print();
		cimglist_for(points,p)
		{
			opoints.insert(CImg<>::vector(points(p,0),points(p,1)));
			dpoints.insert(CImg<>::vector(points(p,2),points(p,3)));
		}
		std::fprintf(stderr," %u points\n",points.size());
	}
	
	// Enter interactive loop
	//------------------------
	while (!disp_orig.is_closed() && !disp_orig.is_keyESC() && !disp_orig.is_keyQ() && !disp_dest.is_closed() && !disp_dest.is_keyESC() && !disp_dest.is_keyQ()) {
		std::vector< CImg<float> > ovector, dvector;
		// Handle mouse manipulation
		//---------------------------
		
		const unsigned int obutton = disp_orig.button();
		const unsigned int dbutton = disp_dest.button();
		const float
		xo = disp_orig.mouse_x()*(float)img0.width()/disp_orig.width(),
		yo = disp_orig.mouse_y()*(float)img0.height()/disp_orig.height();
		
		const float
		xd= disp_dest.mouse_x()*(float)img1.width()/disp_dest.width(),
		yd = disp_dest.mouse_y()*(float)img1.height()/disp_dest.height();
		
		if( !obutton && ! dbutton && ! disp_orig.key() && !disp_dest.key())
		{
#if cimg_OS==2 // Windows
	Sleep(30);
#else
	usleep(30000);
#endif
		}

		//Origen
//		std::cout <<"Hola\n";
		if (opoints && obutton && xo>=0 && yo>=0) {
			
			// Find nearest point and nearest segment
			float dmin_pt = cimg::type<float>::max();//, dmin_seg = dmin_pt;
			unsigned int p_pt = 0;//, p_seg = 0;
			cimglist_for(opoints,p) {
				//				const unsigned int
				//				pnext = closed?(p+1)%opoints.size():(p+1<(int)opoints.size()?p+1:p);
				const float
				xp = opoints(p,0),
				yp = opoints(p,1);
				//				xm = 0.5f*(xp + points(pnext,0)),
				//				ym = 0.5f*(yp + points(pnext,1));
				const float
				d_pt  = (xp-xo)*(xp-xo) + (yp-yo)*(yp-yo);
				//				d_seg = (xm-x)*(xm-x) + (ym-y)*(ym-y);
				if (d_pt<dmin_pt)   { dmin_pt = d_pt; p_pt = p; }
				//				if (d_seg<dmin_seg) { dmin_seg = d_seg; p_seg = p; }
			}
			
			// Handle button
			if (obutton&1) {
				if (dmin_pt<100 || omoving) { opoints(p_pt,0) = xo; opoints(p_pt,1) = yo; }
				else
				{
					//Estimar la homografia con los ptos anteriores
					CImg<float> auxp(3);
					cimglist_for(opoints,p) {
						auxp(0)=opoints[p](0);
						auxp(1)=opoints[p](1);
						auxp(2)=1.0;
						ovector.push_back(auxp);
						
						auxp(0)=dpoints[p](0);
						auxp(1)=dpoints[p](1);
						auxp(2)=1.0;						
						dvector.push_back(auxp);
					}
					H=ComputeHomography(ovector,dvector);
					std::fprintf(stderr,"Matriz recalculada\n");
					H.print("H");
					//Añado un pto en el origen
					opoints.push_back(CImg<>::vector(xo,yo));
					
					//Proyectar el pto q acabo de insertar
					CImg<float> ptr(2);
					ptr(0)=xo;
					ptr(1)=yo;
					ptr = TransformPoint(H, ptr);
					float xxo = ptr(0);
					float yyo = ptr(1);
					CImg<> vvtt = CImg<>::vector(xxo , yyo );
					dpoints.push_back( vvtt );
					
					
				}
				omoving = true;
			}
			if ( (obutton & 2) && (dmin_pt<100) ) {
				if (opoints.size()>4) 
				{
					opoints.remove(p_pt);
					dpoints.remove(p_pt);
				}
				disp_orig.set_button();
			}
			
		}
		
		if (!obutton) omoving = false;

		
		if (dpoints && dbutton && xd>=0 && yd>=0) {
			
			// Find nearest point and nearest segment
			float dmin_pt = cimg::type<float>::max();//, dmin_seg = dmin_pt;
			unsigned int p_pt = 0;//, p_seg = 0;
			cimglist_for(dpoints,p) {
				//				const unsigned int
				//				pnext = closed?(p+1)%dpoints.size():(p+1<(int)dpoints.size()?p+1:p);
				const float
				xp = dpoints(p,0),
				yp = dpoints(p,1);
				//				xm = 0.5f*(xp + points(pnext,0)),
				//				ym = 0.5f*(yp + points(pnext,1));
				const float
				d_pt  = (xp-xd)*(xp-xd) + (yp-yd)*(yp-yd);
				//				d_seg = (xm-x)*(xm-x) + (ym-y)*(ym-y);
				if (d_pt<dmin_pt)   { dmin_pt = d_pt; p_pt = p; }
				//				if (d_seg<dmin_seg) { dmin_seg = d_seg; p_seg = p; }
			}
			
			// Handle button
			if (dbutton&1) {
				if (dmin_pt<100 || dmoving) { dpoints(p_pt,0) = xd; dpoints(p_pt,1) = yd; }
				else 
				{
					//Estimar la homografia con los ptos anteriores
					CImg<float> auxp(3);
					cimglist_for(opoints,p) {
						auxp(0)=opoints[p](0);
						auxp(1)=opoints[p](1);
						auxp(2)=1.0;
						ovector.push_back(auxp);
						
						auxp(0)=dpoints[p](0);
						auxp(1)=dpoints[p](1);
						auxp(2)=1.0;						
						dvector.push_back(auxp);
					}
					H=ComputeHomography(ovector,dvector);
					std::fprintf(stderr,"Matriz recalculada 2\n");
					H.print("H");
					
					//Añado un punto en el destino
					dpoints.push_back(CImg<>::vector(xd,yd));
					
					//Proyectar el pto q acabo de insertar
					CImg<float> ptr(2);
					ptr(0)=xd;
					ptr(1)=yd;
					ptr = InvertPoint( H, ptr);
					float xxo = ptr(0);
					float yyo = ptr(1);
					CImg<> aac = CImg<>::vector( xxo, yyo);
					opoints.push_back( aac );
			
//					p_dpoints=H.InvertPoints(dpoints);
//					p_opoints=H.TransformPoints(opoints);
				}
				dmoving = true;
			}
			if ( ( dbutton&2) && dmin_pt<100) {
				if (dpoints.size()>4)
				{
					dpoints.remove(p_pt);
					opoints.remove(p_pt);
									}
				disp_dest.set_button();
			}
		}
		
		
		if (!dbutton) dmoving = false;


		if (disp_orig.key()) {
			switch (disp_orig.key()) {
				case cimg::keyP : show_points = !show_points; break;
				case cimg::keyO : show_projection = !show_projection; break;
				case cimg::keyN : show_indices = !show_indices; break;
				case cimg::keyU : show_coordinates = !show_coordinates; break;
				case cimg::keyR : opoints.assign(); dpoints.assign(); break;
				case cimg::keyH : help = !help; break;
				case cimg::keyS : save=true; break;
			}
			disp_orig.set_key();
		}
		
		
		
		
		if (disp_dest.key()) {
			switch (disp_dest.key()) {
				case cimg::keyP : show_points = !show_points; break;
				case cimg::keyO : show_projection = !show_projection; break;
				case cimg::keyN : show_indices = !show_indices; break;
				case cimg::keyU : show_coordinates = !show_coordinates; break;
				case cimg::keyR : dpoints.assign(); opoints.assign(); break;
				case cimg::keyH : help = !help; break;
				case cimg::keyS : save = true; break;
			}
			disp_dest.set_key();
		}


		// Save files
		//------------------------------
		if (save)
		{
			char a[50];
			sprintf(a, "points_%03d.dlm",save_counter);
			cimglist_for(opoints,p)
			points.insert(CImg<>::vector(opoints(p,0),opoints(p,1),dpoints(p,0),dpoints(p,1)));
			(points>'x').transpose().save_dlm(a);
			
			points.assign();
			sprintf(a,"points_origin_%03d.dlm",save_counter);
			cimglist_for(opoints,p)
			points.insert(CImg<>::vector(opoints(p,0),opoints(p,1) ) );
			(points>'x').transpose().save_dlm(a);

			points.assign();
			sprintf(a, "points_dest_%03d.dlm",save_counter);
			cimglist_for(opoints,p)
			points.insert(CImg<>::vector(dpoints(p,0),dpoints(p,1)));
			(points>'x').transpose().save_dlm(a);
			
			sprintf(a,"orig_display_%03d.jpg",save_counter);
			img_orig.get_resize(img0.width(),img0.height()).save(a);
			
			sprintf(a, "dest_display_%03d.jpg",save_counter);
			img_dest.get_resize(img1.width(),img1.height()).save(a);
			
			sprintf(a, "dest_display_%03d.jpg",save_counter);
			img_dest.get_resize(img1.width(),img1.height()).save(a);

			CImg<float> auxp(3);
			cimglist_for(opoints,p) {
				auxp(0)=opoints[p](0);
				auxp(1)=opoints[p](1);
				auxp(2)=1.0;
				ovector.push_back(auxp);

				auxp(0)=dpoints[p](0);
				auxp(1)=dpoints[p](1);
				auxp(2)=1.0;
				dvector.push_back(auxp);
			}
			H=ComputeHomography(ovector,dvector);
			std::fprintf(stderr,"Matriz recalculada to save\n");
			H.print("H");

			
			if(H.size())
			{
				sprintf(a,"H_%03d.dlm",save_counter);
				H.save_dlm(a);
				
				sprintf(a,"Hinv_%03d.dlm",save_counter);
				H.get_invert().save_dlm(a);
			}
			
			save_counter++;
			save = false;
		}
		

		// Init list of points if empty
		//------------------------------
		if (!opoints || ! dpoints) {
			float
			x0 = img0.width()/4.0f,
			y0 = img0.height()/4.0f,
			x1 = img0.width() - x0,
			y1 = img0.height() - y0;
			opoints.insert(CImg<>::vector(x0,y0)).
			insert(CImg<>::vector(x1,y0)).
			insert(CImg<>::vector(x1,y1)).
			insert(CImg<>::vector(x0,y1));
			
			
			x0 = img1.width()/4.0f,
			y0 = img1.height()/4.0f,
			x1 = img1.width() - x0,
			y1 = img1.height() - y0;
			dpoints.insert(CImg<>::vector(x0,y0)).
			insert(CImg<>::vector(x1,y0)).
			insert(CImg<>::vector(x1,y1)).
			insert(CImg<>::vector(x0,y1));

			CImg<float> auxp(3);
			cimglist_for(opoints,p) {
				auxp(0)=opoints[p](0);
				auxp(1)=opoints[p](1);
				auxp(2)=1.0;
				ovector.push_back(auxp);

				auxp(0)=dpoints[p](0);
				auxp(1)=dpoints[p](1);
				auxp(2)=1.0;
				dvector.push_back(auxp);
			}
			H=ComputeHomography(ovector,dvector);
			std::fprintf(stderr,"Matriz recalculada\n");
			H.print("H");
			
		}

		// Recalcular la homografía si es necesario
		//-----------------------------------------
/*
		if(recalcula) {recalcula=false;}
		//TODO Recalcular la homografia
		
		
		// Draw curve and display image
		//-------------------------------
	*/
		//Imagen de origen
		
		float
		ofactx = (float)disp_orig.width()/img0.width(),
		ofacty = (float)disp_orig.height()/img0.height(),
		dfactx = (float)disp_dest.width()/img1.width(),
		dfacty = (float)disp_dest.height()/img1.height();
		
		img_orig = img0.get_resize(disp_orig.width(),disp_orig.height());
		img_dest = img1.get_resize(disp_dest.width(),disp_dest.height());
		
		if (help) img_orig.draw_image(help_img,0.6f);
		
		
		
			if (help) img_dest.draw_image(help_img,0.6f);
		
		if (interp) {
			CImg<> nopoints = opoints>'x';
			nopoints.get_shared_row(0)*=ofactx;
			nopoints.get_shared_row(1)*=ofacty;
			
			CImg<> ndpoints = dpoints>'x';
			ndpoints.get_shared_row(0)*=dfactx;
			ndpoints.get_shared_row(1)*=dfacty;
			
		}
		
		
		
		if (show_points)
			
		{
			cimglist_for(opoints,p) {
				const float
				xo = opoints(p,0)*ofactx,
				yo = opoints(p,1)*ofacty,
				xd = dpoints(p,0)*dfactx,
				yd = dpoints(p,1)*dfacty;
				
				img_orig.draw_circle((int)xo,(int)yo,3,red,0.7f);	
				img_dest.draw_circle((int)xd,(int)yd,3,red,0.7f);
				
				if (show_projection) 
				{

					CImgList<float> p_dpoints = InvertPoints(H, dpoints);
					CImgList<float> p_opoints = TransformPoints(H, opoints);

					const float
					xop= p_opoints(p,0)*dfactx,
					yop= p_opoints(p,1)*dfacty,
					xdp= p_dpoints(p,0)*ofactx,
					ydp= p_dpoints(p,1)*ofacty;
					
					
					img_orig.draw_triangle((int)xdp,(int)ydp-3,(int)xdp-3,(int)ydp+3,(int)xdp+3,(int)ydp+3,yellow,0.7f);
					img_orig.draw_line((int)xo, (int)yo, (int)xdp, (int)ydp, green, 0.7f);
					
					img_dest.draw_triangle((int)xop,(int)yop-3,(int)xop-3,(int)yop+3,(int)xop+3,(int)yop+3,yellow,0.7f);
					img_dest.draw_line((int)xd, (int)yd, (int)xop, (int)yop, green, 0.7f);
					
					//Dibujarlo y dibujar la linea q lo une al pto
				}
				if (show_indices)
				{ 
					img_orig.draw_text((int)xo,(int)(yo-19),"%d",white,black,0.8f,16,p);  
					img_dest.draw_text((int)xd,(int)(yd-19),"%d",white,black,0.8f,16,p);
				}
				if (show_coordinates)
				{
					img_orig.draw_text((int)(xo-24),(int)(yo+8),"(%d,%d)",yellow,black,0.8f,13,(int)opoints(p,0),(int)opoints(p,1));
					img_dest.draw_text((int)(xd-24),(int)(yd+8),"(%d,%d)",yellow,black,0.8f,13,(int)dpoints(p,0),(int)dpoints(p,1));
				}
			}	
		}
		

		
		img_orig.display(disp_orig);
		//disp_orig.wait();
		
		if (disp_orig.is_resized()) disp_orig.resize(false);
		
		img_dest.display(disp_dest);
		//disp_dest.wait();
		
		if (disp_dest.is_resized()) disp_dest.resize(false);
	}
	
	// Save output result and exit
	//-----------------------------
	
	if(!save_counter)
	{
		char a[50];
		sprintf(a,"points_%03d.dlm",save_counter);
		cimglist_for(opoints,p)
		points.insert(CImg<>::vector(opoints(p,0),opoints(p,1),dpoints(p,0),dpoints(p,1)));
		(points>'x').transpose().save_dlm(a);
		
		
		sprintf(a,"orig_display_%03d.jpg",save_counter);
		img_orig.get_resize(img0.width(),img0.height()).save(a);
		
		sprintf(a,"dest_display_%03d.jpg",save_counter);
		img_dest.get_resize(img1.width(),img1.height()).save(a);
		std::vector< CImg<float> > ovector, dvector;

		CImg<float> auxp(3);
		cimglist_for(opoints,p) {
			auxp(0)=opoints[p](0);
			auxp(1)=opoints[p](1);
			auxp(2)=1.0;
			ovector.push_back(auxp);

			auxp(0)=dpoints[p](0);
			auxp(1)=dpoints[p](1);
			auxp(2)=1.0;
			dvector.push_back(auxp);
		}

		H=ComputeHomography(ovector,dvector);
		std::fprintf(stderr,"Matriz recalculada to save\n");
		H.print("H");

				
		if(H.size())
		{
			sprintf(a,"H_%03d.dlm",save_counter);
			H.save_dlm(a);
			
			sprintf(a,"Hinv_%03d.dlm",save_counter);
			H.save_dlm(a);
		}
		
	}
	
	
	std::fprintf(stderr," - Exit.\n");
	std::exit(0);
	return 0;
}
