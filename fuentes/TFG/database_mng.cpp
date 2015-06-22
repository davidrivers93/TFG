/*
 * database_mng.cpp
 *
 *  Created on: 2/6/2015
 *      Author: David
 */


#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "database_mng.h"

using namespace std;

database_mng::~database_mng()
{
    delete res;
    delete prep_stmt;
    delete stmt;
    delete con;
}

void database_mng::manageException(sql::SQLException& e)
{

      /*  cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;*/

}

void database_mng::connect()
{
    try {
        driver = get_driver_instance();
    } catch (sql::SQLException &e) {
        manageException(e);
    }
    try {
        con = driver->connect(host, user, password);
    } catch (sql::SQLException &e) {
        manageException(e);
    }
}

void database_mng::switchDb(const string& db_name)
{
    try {
        con->setSchema(db_name);
    } catch (sql::SQLException &e) {
        manageException(e);
    }
    try {
        stmt = con->createStatement();
    } catch (sql::SQLException &e) {
        manageException(e);
    }
}

void database_mng::prepare(const string& query)
{
    try {
        prep_stmt = con->prepareStatement(query);
    } catch (sql::SQLException &e) {
        manageException(e);


    }
}

void database_mng::setInt(const int& num, const int& data)
{
    prep_stmt->setInt(num, data);
}

void database_mng::setString(const int& num, const string& data)
{
    prep_stmt->setString(num, data);
}

void database_mng::execute(const string& query)
{
    try {
        res = (query != "") ? stmt->executeQuery(query) : prep_stmt->executeQuery();
    } catch (sql::SQLException &e) {
        manageException(e);
    }
}

bool database_mng::fetch()
{
    return res->next();
}

string database_mng::print(const string& field)
{
    return res->getString(field);
}

string database_mng::print(const int& index)
{
    return res->getString(index);
}
