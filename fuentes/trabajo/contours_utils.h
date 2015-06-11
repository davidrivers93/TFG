/*
 * contour_utils.h
 *
 *  Created on: Mar 30, 2015
 *      Author: aalbiol
 */

#ifndef P8_CONTOURS_UTILS_H_
#define P8_CONTOURS_UTILS_H_


cv::Point2f blobCenter(const std::vector<cv::Point> & points);
void drawBlobs (cv::Mat & output, const std::vector < std::vector<cv::Point> > &blobs);
void drawRedBlobs (cv::Mat & output, const std::vector < std::vector<cv::Point> > &blobs);
void computeContourDepth(const std::vector<cv::Vec4i> & hierarchy, std::vector<int> & depth);

int numberOfSons(const std::vector<cv::Vec4i> & hierarchy, int id);

#endif /* P8_CONTOURS_UTILS_H_ */
