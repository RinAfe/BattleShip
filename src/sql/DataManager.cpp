#include "DataManager.h"

#include <iostream>
#include "pqxx/pqxx"

DataManager::DataManager() {
    try {
        conn = std::make_unique<pqxx::connection>(
        "host=localhost "
        "port=5432 "
        "dbname=testdb "
        "user=rinat"
        );
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void DataManager::initDataBase() {
    try {
        pqxx::work txn(*conn);

        // Проверяем существует ли ENUM game_type_enum
        pqxx::result check_type = txn.exec(
            "SELECT 1 FROM pg_type WHERE typname = 'game_type_enum'"
        );

        if (check_type.empty()) {
            // Создаем если не существует
            txn.exec("CREATE TYPE game_type_enum AS ENUM ('PB', 'PP')");
            std::cout << "✅ Created game_type_enum" << std::endl;
        }

        // Проверяем winner_enum
        check_type = txn.exec(
            "SELECT 1 FROM pg_type WHERE typname = 'winner_enum'"
        );

        if (check_type.empty()) {
            txn.exec("CREATE TYPE winner_enum AS ENUM ('P', 'B', 'P1', 'P2')");
            std::cout << "✅ Created winner_enum" << std::endl;
        }

        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS players (
                id SERIAL PRIMARY KEY,
                name VARCHAR(10) NOT NULL,
                email VARCHAR(30) UNIQUE NOT NULL,
                count_games INT DEFAULT 0,
                count_wins INT DEFAULT 0,
                count_loses INT GENERATED ALWAYS AS
                    (count_games - count_wins) STORED,

                -- Проверки
                CONSTRAINT non_negative_games CHECK (count_games >= 0),
                CONSTRAINT non_negative_wins CHECK (count_wins >= 0),
                CONSTRAINT wins_not_greater_than_games CHECK (count_wins <= count_games)
            )
        )");

        // Создаем таблицу games
        txn.exec(
            "CREATE TABLE IF NOT EXISTS games ("
            "game_id SERIAL PRIMARY KEY, "
            "game_type game_type_enum NOT NULL, "
            "winner winner_enum NULL, "
            "game_date_start TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
            "game_date_end TIMESTAMP DEFAULT NULL, "
            "CONSTRAINT valid_dates CHECK ("
            "    game_date_end IS NULL OR game_date_end > game_date_start"
            ")"
            ")"
        );

        txn.commit();
        std::cout << "✅ Database initialized successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Database init error: " << e.what() << std::endl;
    }
}


