#pragma once

#include "pqxx/pqxx"

class DataManager {
    std::unique_ptr<pqxx::connection> conn;

public:
    DataManager();
    ~DataManager() = default;

    void initDataBase();

};