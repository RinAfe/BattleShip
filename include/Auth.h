#pragma once

#include <string>
#include <pqxx/pqxx>
#include "ConsoleView.h"

struct PlayerSession {
    int playerId = -1;
    std::string name;
    std::string email;
    bool isAuthenticated = false;
};

class AuthManager {
private:
    ConsoleView& console;
    pqxx::connection& conn;
    PlayerSession currentSession;

    bool checkEmailExists(const std::string& email);
    bool registerUser(const std::string& name, const std::string& email);
    bool loginUser(const std::string& email);
    bool handleLoginWithEmail(const std::string& email);
    bool handleLogin();
    bool handleRegistration();
    int getValidInput(int min, int max);

    bool loadUserInfo(int userId, const std::string& email);

public:
    AuthManager(ConsoleView& consoleRef, pqxx::connection& dbConn);

    bool authenticate();

    const PlayerSession& getSession() const;

    void resetSession();
};
