/*
#
#  File        : cimgcvMat.h
#                ( C++ header file - CImg plug-in )
#
#  Description : CImg plug-in providing the CImg->cvMat and cvMat->CImg
#                conversions for generic image types
#                ( IPL = Intel Performance Library )
#                This file is a part of the CImg Library project.
#                ( http://cimg.sourceforge.net )
#
#  Copyright   : Alberto Albiol
#                alalbiol@iteam.upv.es
#
#  How to use  : In the main program include:
 
 
        #include "cv.h"
        #include "highgui.h"
        #define cimg_plugin1 "cimgcvMat.h"

        #include "CImg.h"
 
 #
*/

#ifndef cimg_plugin_cimgcvMat
#define cimg_plugin_cimgcvMat



// Conversion IPL -> CImg (constructor)
CImg(const cv::Mat& src):_width(0),_height(0),_depth(0),_spectrum(0),_is_shared(false),_data(0) {
  assign(src);
}

// Conversion IPL -> CImg (in-place constructor)
CImg<T>& assign(const cv::Mat & src) {
  
  if( src.isContinuous() )
  {
    switch (src.depth()) 
    {
        // case CV_1U: { // 1-bit int.
        //    IplImage *src1 = cvCreateImage(cvGetSize(src),CV_8U,1);
        //    cvConvert(src,src1);
        //    CImg<ucharT>((unsigned char*)src1->imageData,src1->nChannels,src1.cols,src1.rows,1,true).
        //      get_permute_axes("yzcx").move_to(*this);
        //    cvReleaseImage(&src1);
        //  } break;
      case CV_8U: // 8-bit unsigned int.
        if (src.channels()==1)
        {
          CImg<ucharT>((ucharT*)src.ptr(),src.cols,src.rows,true).move_to(*this);
        }
        else
        {
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          cimg_library::CImg<ucharT> tmp(src.cols,src.rows,1,3);
          cimg_library::CImg<ucharT> R=tmp.get_shared_channel(2);
          cimg_library::CImg<ucharT> G=tmp.get_shared_channel(1);
          cimg_library::CImg<ucharT> B=tmp.get_shared_channel(0);
          memcpy(R.data(), channels[0].ptr(), src.cols*src.rows*sizeof(ucharT));
          memcpy(G.data(), channels[1].ptr(), src.cols*src.rows*sizeof(ucharT));
          memcpy(B.data(), channels[2].ptr(), src.cols*src.rows*sizeof(ucharT));
          tmp.move_to(*this);
        }
        break;
      case CV_8S: // 8-bit signed int.
        if (src.channels()==1)
        {
          CImg<charT>((charT*)src.ptr(),src.cols,src.rows,true).move_to(*this);
        }
        else
        {
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          cimg_library::CImg<ucharT> tmp(src.cols,src.rows,1,3);
          cimg_library::CImg<ucharT> R=tmp.get_shared_channel(2);
          cimg_library::CImg<ucharT> G=tmp.get_shared_channel(1);
          cimg_library::CImg<ucharT> B=tmp.get_shared_channel(0);
          memcpy(R.data(), channels[0].ptr(), src.cols*src.rows*sizeof(charT));
          memcpy(G.data(), channels[1].ptr(), src.cols*src.rows*sizeof(charT));
          memcpy(B.data(), channels[2].ptr(), src.cols*src.rows*sizeof(charT));
          tmp.move_to(*this);
        }
        break;
      case CV_16U: // 16-bit unsigned int.
        if (src.channels()==1)
        {
          CImg<ushortT>((ushortT*)src.ptr(),src.cols,src.rows,true).move_to(*this);
        }
        else
        {
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          cimg_library::CImg<ushortT> tmp(src.cols,src.rows,1,3);
          cimg_library::CImg<ushortT> R=tmp.get_shared_channel(2);
          cimg_library::CImg<ushortT> G=tmp.get_shared_channel(1);
          cimg_library::CImg<ushortT> B=tmp.get_shared_channel(0);
          memcpy(R.data(), channels[0].ptr(), src.cols*src.rows*sizeof(ushortT));
          memcpy(G.data(), channels[1].ptr(), src.cols*src.rows*sizeof(ushortT));
          memcpy(B.data(), channels[2].ptr(), src.cols*src.rows*sizeof(ushortT));
          tmp.move_to(*this);
        } 
        break;
      case CV_16S: // 16-bit signed int.
        if (src.channels()==1)
        {
          CImg<shortT>((shortT*)src.ptr(),src.cols,src.rows,true).move_to(*this);
        }
        else
        {
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          cimg_library::CImg<shortT> tmp(src.cols,src.rows,1,3);
          cimg_library::CImg<shortT> R=tmp.get_shared_channel(2);
          cimg_library::CImg<shortT> G=tmp.get_shared_channel(1);
          cimg_library::CImg<shortT> B=tmp.get_shared_channel(0);
          memcpy(R.data(), channels[0].ptr(), src.cols*src.rows*sizeof(shortT));
          memcpy(G.data(), channels[1].ptr(), src.cols*src.rows*sizeof(shortT));
          memcpy(B.data(), channels[2].ptr(), src.cols*src.rows*sizeof(shortT));
          tmp.move_to(*this);
        }   
        break;
      case CV_32S: // 32-bit signed int.
        if (src.channels()==1)
        {
          CImg<intT>((intT*)src.ptr(),src.cols,src.rows,true).move_to(*this);
        }
        else
        {
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          cimg_library::CImg<intT> tmp(src.cols,src.rows,1,3);
          cimg_library::CImg<intT> R=tmp.get_shared_channel(2);
          cimg_library::CImg<intT> G=tmp.get_shared_channel(1);
          cimg_library::CImg<intT> B=tmp.get_shared_channel(0);
          memcpy(R.data(), channels[0].ptr(), src.cols*src.rows*sizeof(intT));
          memcpy(G.data(), channels[1].ptr(), src.cols*src.rows*sizeof(intT));
          memcpy(B.data(), channels[2].ptr(), src.cols*src.rows*sizeof(intT));
          tmp.move_to(*this);
        }     
        break;
      case CV_32F: // 32-bit float.
        if (src.channels()==1)
        {
          CImg<floatT>((floatT*)src.ptr(),src.cols,src.rows,true).move_to(*this);
        }
        else
        {
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          cimg_library::CImg<floatT> tmp(src.cols,src.rows,1,3);
          cimg_library::CImg<floatT> R=tmp.get_shared_channel(2);
          cimg_library::CImg<floatT> G=tmp.get_shared_channel(1);
          cimg_library::CImg<floatT> B=tmp.get_shared_channel(0);
          memcpy(R.data(), channels[0].ptr(), src.cols*src.rows*sizeof(floatT));
          memcpy(G.data(), channels[1].ptr(), src.cols*src.rows*sizeof(floatT));
          memcpy(B.data(), channels[2].ptr(), src.cols*src.rows*sizeof(floatT));
          tmp.move_to(*this);
        }     
        break;
      case CV_64F: // 64-bit double.
        if (src.channels()==1)
        {
          CImg<doubleT>((doubleT*)src.ptr(),src.cols,src.rows,true).move_to(*this);
        }
        else
        {
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          cimg_library::CImg<doubleT> tmp(src.cols,src.rows,1,3);
          cimg_library::CImg<doubleT> R=tmp.get_shared_channel(2);
          cimg_library::CImg<doubleT> G=tmp.get_shared_channel(1);
          cimg_library::CImg<doubleT> B=tmp.get_shared_channel(0);
          memcpy(R.data(), channels[0].ptr(), src.cols*src.rows*sizeof(doubleT));
          memcpy(G.data(), channels[1].ptr(), src.cols*src.rows*sizeof(doubleT));
          memcpy(B.data(), channels[2].ptr(), src.cols*src.rows*sizeof(doubleT));
          tmp.move_to(*this);
        }     
        break;
      default:
        throw CImgInstanceException("CImg<%s>::assign(const cv::Mat img) : Mat depth is invalid.",
                                    pixel_type());
        break;
    }
  }
  else {
    cv::Size size = src.size();
    switch (src.depth()) {
      {

      case CV_8U: // 8-bit unsigned int.
        if (src.channels()==1)
        {
          CImg<ucharT> tmp(src.cols,src.rows) ;
          for( int i = 0; i < size.height; i++ )
          {
            const ucharT* row_i = src.ptr<unsigned char>(i);
            ucharT *row_o = tmp.data(0,i);
            memcpy(row_o,row_i, size.width * sizeof(ucharT));
          }
          tmp.move_to(*this);
        }
        else
        {
          CImg<ucharT> tmp(src.cols,src.rows,1,src.channels()) ;
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          
          for( int c = 0; c < src.channels() ; c++)
          {
            cimg_library::CImg<ucharT> plane=tmp.get_shared_channel(src.channels()-1-c);
            
            for( int i = 0; i < size.height; i++ )
            {
              const ucharT* row_i = channels[c].ptr<unsigned char>(i);
              ucharT *row_o = plane.data(0,i);
              memcpy(row_o,row_i, size.width * sizeof(ucharT));
            }
          } 
          tmp.move_to(*this);
        }
        break;
      case CV_8S: // 8-bit int.
        if (src.channels()==1)
        {
          CImg<charT> tmp(src.cols,src.rows) ;
          for( int i = 0; i < size.height; i++ )
          {
            const charT* row_i = src.ptr<char>(i);
            charT *row_o = tmp.data(0,i);
            memcpy(row_o,row_i, size.width * sizeof(charT));
          }
          tmp.move_to(*this);
        }
        else
        {
          CImg<charT> tmp(src.cols,src.rows,1,src.channels()) ;
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          
          for( int c = 0; c < src.channels() ; c++)
          {
            cimg_library::CImg<charT> plane=tmp.get_shared_channel(src.channels()-1-c);
            
            for( int i = 0; i < size.height; i++ )
            {
              const charT* row_i = channels[c].ptr<char>(i);
              charT *row_o = plane.data(0,i);
              memcpy(row_o,row_i, size.width * sizeof(charT));
            }
          } 
          tmp.move_to(*this);
        }
        break;
      case CV_16S: // 16-bit int.
        if (src.channels()==1)
        {
          CImg<shortT> tmp(src.cols,src.rows) ;
          for( int i = 0; i < size.height; i++ )
          {
            const shortT* row_i = src.ptr<short>(i);
            shortT *row_o = tmp.data(0,i);
            memcpy(row_o,row_i, size.width * sizeof(shortT));
          }
          tmp.move_to(*this);
        }
        else
        {
          CImg<shortT> tmp(src.cols,src.rows,1,src.channels()) ;
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          
          for( int c = 0; c < src.channels() ; c++)
          {
            cimg_library::CImg<shortT> plane=tmp.get_shared_channel(src.channels()-1-c);
            
            for( int i = 0; i < size.height; i++ )
            {
              const shortT* row_i = channels[c].ptr<short>(i);
              shortT *row_o = plane.data(0,i);
              memcpy(row_o,row_i, size.width * sizeof(shortT));
            }
          } 
          tmp.move_to(*this);
        }
        break;
      case CV_32F: // 32-bit float.float
        if (src.channels()==1)
        {
          CImg<floatT> tmp(src.cols,src.rows) ;
          for( int i = 0; i < size.height; i++ )
          {
            const floatT* row_i = src.ptr<float>(i);
            floatT *row_o = tmp.data(0,i);
            memcpy(row_o,row_i, size.width * sizeof(floatT));
          }
          tmp.move_to(*this);
        }
        else
        {
          CImg<floatT> tmp(src.cols,src.rows,1,src.channels()) ;
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          
          for( int c = 0; c < src.channels() ; c++)
          {
            cimg_library::CImg<floatT> plane=tmp.get_shared_channel(src.channels()-1-c);
            
            for( int i = 0; i < size.height; i++ )
            {
              const floatT* row_i = channels[c].ptr<float>(i);
              floatT *row_o = plane.data(0,i);
              memcpy(row_o,row_i, size.width * sizeof(floatT));
            }
          } 
          tmp.move_to(*this);
        }
        break;
      case CV_64F: // 64-bit double.
       if (src.channels()==1)
        {
          CImg<doubleT> tmp(src.cols,src.rows) ;
          for( int i = 0; i < size.height; i++ )
          {
            const doubleT* row_i = src.ptr<double>(i);
            doubleT *row_o = tmp.data(0,i);
            memcpy(row_o,row_i, size.width * sizeof(doubleT));
          }
          tmp.move_to(*this);
        }
        else
        {
          CImg<doubleT> tmp(src.cols,src.rows,1,src.channels()) ;
          std::vector<cv::Mat> channels;
          cv::split(src, channels);
          
          for( int c = 0; c < src.channels() ; c++)
          {
            cimg_library::CImg<doubleT> plane=tmp.get_shared_channel(src.channels()-1-c);
            
            for( int i = 0; i < size.height; i++ )
            {
              const doubleT* row_i = channels[c].ptr<double>(i);
              doubleT *row_o = plane.data(0,i);
              memcpy(row_o,row_i, size.width * sizeof(doubleT));
            }
          } 
          tmp.move_to(*this);
        }
        break;        
      default:
        throw CImgInstanceException("CImg<%s>::assign(const cv::Mat img) : Mat depth is invalid.",
                                    pixel_type());
        break;
      }
        
 
    }
  }

//  if (!std::strcmp(src->channelSeq,"BGR")) mirror('v');
//  else if (!std::strcmp(src->channelSeq,"BGRA")) get_shared_channels(0,2).mirror('v');
  return *this;
}

// Conversion CImg -> IPL
cv::Mat get_MAT(const unsigned int z=0) const {
  if (is_empty())
    throw CImgInstanceException("CImg<%s>::get_IPL() : instance image (%u,%u,%u,%u,%p) is empty.",
                                pixel_type(),_width,_height,_depth,_spectrum,_data);
  if (z>=_depth)
    throw CImgInstanceException("CImg<%s>::get_IPL() : specified slice %u is out of image bounds (%u,%u,%u,%u,%p).",
                                pixel_type(),z,_width,_height,_depth,_spectrum,_data);
  
  
  const CImg<T>
  _slice = _depth>1?get_slice(z):CImg<T>(),
  &slice = _depth>1?_slice:*this;
  CImg<T> buf(slice, true);
  
  
  
  int cols=buf.width();
  int rows=buf.height();
  int nchannels=buf.spectrum();
  
  
  int matType=-1;
    
    
  if (!cimg::strcasecmp(buf.pixel_type(),"unsigned char")) matType = CV_8UC1;
  if (!cimg::strcasecmp(buf.pixel_type(),"char")) matType = CV_8SC1;
  if (!cimg::strcasecmp(buf.pixel_type(),"unsigned short")) matType = CV_16UC1;
  if (!cimg::strcasecmp(buf.pixel_type(),"short")) matType = CV_16SC1;
  if (!cimg::strcasecmp(buf.pixel_type(),"int")) matType = CV_32SC1;
  if (!cimg::strcasecmp(buf.pixel_type(),"float")) matType = CV_32FC1;
  if (!cimg::strcasecmp(buf.pixel_type(),"double")) matType = CV_64FC1;
  if (matType<0)
    throw CImgInstanceException("CImg<%s>::get_MAT() : Cimg type is invalid.",
                                buf.pixel_type());
  
  cv::Mat out;
    
  std::vector<cv::Mat> channels(nchannels);
    
 if(nchannels > 1)
 {
  for (int c=0;c<nchannels;c++)
  {
    channels[c]=  cv::Mat(rows,cols, matType,const_cast<T *>(buf.data()+ rows*cols*(nchannels-1-c)));
  } // for channels

  cv::merge(channels,out);
 }
 else
	 out = cv::Mat( rows , cols, matType, const_cast<T *>( buf.data() ) ).clone();
  return out;

}

  
  

#endif
