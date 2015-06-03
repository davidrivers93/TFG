/*
 * database_mng.h
 *
 *  Created on: 2/6/2015
 *      Author: David
 */

#ifndef TRABAJO_DATABASE_MNG_H_
#define TRABAJO_DATABASE_MNG_H_

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class database_mng
{

    public:

        /* Your MySQL server settings */
        database_mng()
        {
            host     = "tcp://risign.ddns.net:3306";
            user     = "david-superadmin";
            password = "donbosco02";
        };
        struct race_data{
        	string race_data;
        	string date_data;
        };

        race_data race_data_query;
        ~database_mng();
        void manageException(sql::SQLException& e);
        void connect();
        void switchDb(const string& db_name);
        void prepare(const string& query);
        void setInt(const int& num, const int& data);
        void setString(const int& num, const string& data);
        void execute(const string& query = "");
        bool fetch();
        string print(const string& field);
        string print(const int& index);

    private:

        string host;
        string user;
        string password;
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::PreparedStatement* prep_stmt;
        sql::ResultSet* res;

};



#endif /* TRABAJO_DATABASE_MNG_H_ */
