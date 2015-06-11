/*
 * contours_utils.cpp
 *
 *  Created on: Mar 30, 2015
 *      Author: aalbiol
 */

#include <vector>
#include <cv.h>

using namespace std;
using namespace cv;

cv::Point2f blobCenter(const std::vector<cv::Point> & points){
	cv::Point2f media(0,0);
	for(int n=0; n  < points.size() ; n++) {
		media.x = media.x + points[n].x;
		media.y = media.y + points[n].y;
	}
	media.x = media.x / points.size();
	media.y = media.y / points.size();
	return media;
}


void drawBlobs (cv::Mat & output, const std::vector < std::vector<cv::Point> > &blobs) {
	for(size_t i=0; i < blobs.size(); i++) {
		unsigned char r = 128 + 128 * (rand()/(1.0 + RAND_MAX));
		unsigned char g = 128 + 128 * (rand()/(1.0 + RAND_MAX));
		unsigned char b = 128 + 128 * (rand()/(1.0 + RAND_MAX));

		for(size_t j=0; j < blobs[i].size(); j++) {
			for(int ox=-1; ox <= 1; ox++)
				for(int oy=-1; oy <= 1; oy++) {
					int x = blobs[i][j].x;
					int y = blobs[i][j].y;

					output.at<cv::Vec3b>(y+oy,x+ox)[0] = b;
					output.at<cv::Vec3b>(y+oy,x+ox)[1] = g;
					output.at<cv::Vec3b>(y+oy,x+ox)[2] = r;
				}
		}
	}
}

void drawRedBlobs (cv::Mat & output, const std::vector < std::vector<cv::Point> > &blobs) {
	for(size_t i=0; i < blobs.size(); i++) {


		for(size_t j=0; j < blobs[i].size(); j++) {
			for(int ox=-1; ox <= 1; ox++)
				for(int oy=-1; oy <= 1; oy++) {
					int x = blobs[i][j].x;
					int y = blobs[i][j].y;

					output.at<cv::Vec3b>(y+oy,x+ox)[0] = 0;
					output.at<cv::Vec3b>(y+oy,x+ox)[1] = 0;
					output.at<cv::Vec3b>(y+oy,x+ox)[2] = 255;
				}
		}
	}
}
	/***************************************************************
	 * void computeContourDepth(const vector<Vec4i> & hierarchy, std::vector<int> & depth)
	 * Impar: contorno externo
	 * Par: contorno externo
	 * ************************************************************* */

	void computeContourDepth(const std::vector<Vec4i> & hierarchy, std::vector<int> & depth) {
		bool continuar = true;

		depth.resize(hierarchy.size());
		while (continuar) {
			continuar = false;
			for(int i=0; i < hierarchy.size(); i++) {
				int next = hierarchy[i][0];
				int prev = hierarchy[i][1];
				int first_child = hierarchy[i][2];
				int parent = hierarchy[i][3];
				if(depth[i] > 0)
					continue;
				if(parent < 0) {
					depth[i] = 1;
					continuar = true;
				}
				else  if( depth[parent] > 0) {
					depth[i] = depth[parent] + 1;
					continuar = true;
				}
			}
		}
	}



	int numberOfSons(const std::vector<Vec4i> & hierarchy, int id) {

		int nsons = 0;
		int son = hierarchy[id][2];
		while ( son >= 0) {
			nsons ++;
			son = hierarchy[son][0];
		}
		return nsons;
	}


