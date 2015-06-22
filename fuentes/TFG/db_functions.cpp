/*
 * db_functions.cpp
 *
 *  Created on: 18/6/2015
 *      Author: David
 */

#ifndef TFG_DB_FUNCTIONS_CPP_
#define TFG_DB_FUNCTIONS_CPP_

#include "database_mng.h"

#include "database_mng.h"
#include "CImg.h"
#include <opencv2/opencv.hpp>

using namespace cimg_library;
using namespace std;
using namespace cv;

void list_races(std::vector<string> & list_races, database_mng & database) {

	string db_races = "testing";
	database.switchDb(db_races);
	database.database_name = db_races;
	database.execute("SELECT * FROM races");
	int index_bbdd_races = 0;
	while (database.fetch()) {
		std::cout << " Race " << database.print(1) << " -> " << database.print(2) << " " << database.print(3) << " " << database.print(4) << "\n";
		index_bbdd_races++;
	}
	std::cerr << "En el caso de que la carrera no este metida en la base de datos escribe 0." << endl;
	std::cerr << "Elige la carrera: \n";
	std::cerr << "El indice es " << index_bbdd_races << endl;
	string index_race;
	std::cin >> index_race;

	if(index_race == "0"){

		std::cout << "Introduce el nombre de la carrera : ";
		string new_race_name;
		std::cin >> new_race_name;
		database.race_data_query.race_data = new_race_name;
		std::cerr << "Introduce la fecha de la carrera (formato dd-mm-aa 31-07-15 ) : ";
		string new_race_date;
		std::cin >> new_race_date;
		database.race_data_query.race_data = new_race_date;
		std::cerr << "Introduce la distancia de la carrera (en metros) : ";
		string distance_string;
		std::cin >> distance_string;
		int distance;
		istringstream ( distance_string ) >> distance;
		database.race_data_query.distance = distance;
		index_bbdd_races++;

		stringstream new_race_table;
		new_race_table << new_race_date.at(0) << new_race_date.at(1) << new_race_name.at(0) << new_race_name.at(1) << new_race_date.at(6) << new_race_date.at(7);
		string new_race_tables = new_race_table.str();
		string race_ins = "ins" + new_race_tables;
		database.prepare("INSERT INTO prueba.races (idraces, races_name, races_date, race_dbname, race_distance, race_ins) VALUES (?, ?, ?, ?, ?, ?)");
		database.setInt(1,index_bbdd_races);
		database.setString(2,new_race_name);
		database.setString(3,new_race_date);
		database.setString(4,new_race_tables);
		database.setInt(5,distance);
		database.setString(6,race_ins);
		database.execute();
		std::cerr << "He llegado" << endl;
		//actualizo struct
		database.race_data_query.race_data = new_race_name;
		database.race_data_query.date_data =  new_race_date;
		database.race_data_query.tablen_data = new_race_tables;
		database.race_data_query.distance = distance;
		database.race_data_query.ins_table = race_ins;
		std::cerr << "He llegado" << endl;



	}
	else{

		string query_race = "Select * FROM races WHERE idraces= \'" + index_race + "\'";
		database.execute(query_race);
		database.fetch();
		database.race_data_query.race_data = database.print(2);
		database.race_data_query.date_data = database.print(3);
		database.race_data_query.tablen_data = database.print(4);
		string temp = database.print(5);
		int distance;
		istringstream ( temp ) >> distance;
		database.race_data_query.distance = distance;
		database.race_data_query.ins_table = database.print(6);


	}
	std::cerr << "SALGO DEL ELSE" << endl;
	//Compruebo si existe la tabla de la carrera
	database.prepare("SELECT * FROM ?");
	database.setString(1, database.race_data_query.tablen_data);
	std::cerr << "SALGO DEL ELSE" << endl;
	database.execute();

	string prueba_create = "CREATE TABLE " + database.race_data_query.tablen_data +  "(`dorsal` INT NOT NULL,`path_img` VARCHAR(45) NOT NULL)";
	std::cerr << "SALGO DEL ELSE" << endl;
	database.execute(prueba_create);
	std::cout << "He creado la tabla. \n";


	//Compruebo si existe tabla de inscritos
	database.prepare("SELECT * FROM ?");
	database.setString(1, database.race_data_query.tablen_data);
	database.execute();


	string prueba_create_ins = "CREATE TABLE " + database.race_data_query.ins_table +  "(`dorsal` INT NOT NULL,`Nombre` VARCHAR(45) NULL,`Apellidos` VARCHAR(45) NULL,`Marca` VARCHAR(45) NULL,`Nick` VARCHAR(45) NULL)";
	database.execute(prueba_create_ins);
	std::cout << "He creado la tabla. \n";


}

void add_result_db(std::string result, database_mng & database, std::string imagename){

	std::string query = "INSERT INTO " + database.race_data_query.tablen_data + "(dorsal, path_img) VALUES (" + result + ",\'/" + database.race_data_query.tablen_data + "/" + imagename + "\')";

	std::cerr << "QUERY: " << query << endl;

	database.execute(query);


}



#endif /* TFG_DB_FUNCTIONS_CPP_ */
