#include <iostream>
#include <ostream>
#include "Databases.h"
#include <string>

void initDataBase() {

    pqxx::connection conn("host=localhost port=5432 dbname=battleship user=postgres password=postgres");

    pqxx::work work(conn);

    try {
        work.exec("CREATE TABLE IF NOT EXISTS games (id NUMERIC UNIQUE PRIMARY KEY,"
                  "type_game VARCHAR(2),"
                  "winner_game VARCHAR(2),"
                  "date_start TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                  "date_end TIMESTAMP DEFAULT NULL)");
        work.commit();
    }catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


