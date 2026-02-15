#include <iostream>
#include <ostream>
#include "Databases.h"
#include <string>

void initDataBases(pqxx::connection& conn) {

    //Инициализация БД games
    try {
        pqxx::work work(conn);
        work.exec("CREATE TABLE IF NOT EXISTS games ("
                  "id SERIAL PRIMARY KEY,"
                  "type_game VARCHAR(2),"
                  "winner_game VARCHAR(20) DEFAULT NULL,"
                  "date_start TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                  "date_end TIMESTAMP DEFAULT NULL)");
        work.commit();
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    //Инициализация БД players
    try {
        pqxx::work work(conn);
        work.exec("CREATE TABLE IF NOT EXISTS players("
                  "id SERIAL PRIMARY KEY,"
                  "name VARCHAR(10),"
                  "email VARCHAR(30) UNIQUE,"
                  "count_games INT DEFAULT 0,"
                  "count_wins INT DEFAULT 0,"
                  "count_loses INT DEFAULT 0)");
        work.commit();
    } catch (std::exception& e) {
        std::cerr << "Exception caught: " << e.what()  << std::endl;
    }

    //Инициализируем БД активных игр
    try {
        pqxx::work work(conn);
        work.exec("CREATE TABLE IF NOT EXISTS activeGames("
                  "id SERIAL PRIMARY KEY,"
                  "game_id INT REFERENCES games(id) ON DELETE CASCADE,"
                  "first_field VARCHAR(101),"
                  "last_field VARCHAR(101),"
                  "id_first_player INT,"
                  "id_last_player INT DEFAULT 0,"
                  "current_turn INT)");
        work.commit();
    } catch (std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}