/*
 * ftp_tranfer.cpp
 *
 *  Created on: 20/6/2015
 *      Author: David
 */

#ifndef TFG_FTP_TRANFER_CPP_
#define TFG_FTP_TRANFER_CPP_

int upload(std::string filename, database_mng & database);
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);



#endif /* TFG_FTP_TRANFER_CPP_ */
