/*
 * bbdd_conn.cpp
 *
 *  Created on: 2/6/2015
 *      Author: David
 */


#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/resultset.h>

#include <cppconn/statement.h>
#include <cppconn/exception.h>


int main()
{
    sql::Driver *driver;
    sql::Connection *connection;
    sql::Statement *statement;
    sql::ResultSet *resultset;

    // Nos conectamos a nuestro servidor MySQL
    driver = get_driver_instance();
    connection = driver->connect("tcp://risign.ddns.net:3306", "david-superadmin", "donbosco02");

    // Elegimos una base datos que ya tengamos creada ("prueba")
    connection->setSchema("prueba");

    // Hacemos nuestra primera consulta
    statement = connection->createStatement();
    resultset = statement->executeQuery("SELECT * FROM races");

    // Pintamos el resultado
    while (resultset->next()) {
        std::cout << " Nombre: " << resultset->getString(2);
        std::cout << "\n Apellido: " << resultset->getString(3);
        std::cout << std::endl;
    }

    // Editamos un registro existente
    // La consulta lanza una excepción con un error tipo 0 (éxito)
    try {
        statement->executeQuery("UPDATE usuarios SET apellido='Stroustrup' WHERE id = 1 AND nombre='Bjarne'");
    } catch (sql::SQLException &e) {}

    // Eliminamos los objetos
    delete resultset;
    delete statement;
    delete connection;
}
