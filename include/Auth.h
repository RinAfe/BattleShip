#pragma once

#include <string>
#include "ConsoleView.h"

struct PlayerSession {
    int playerId;
    std::string name;
    std::string email;
    bool isAuthenticated;

    PlayerSession() : playerId(0), isAuthenticated(false) {}
};

class AuthManager {
private:
    ConsoleView& console;
    PlayerSession session;

    bool checkEmailExists(const std::string& email);
    bool registerUser(const std::string& name, const std::string& email, int& userId);
    bool loginUser(const std::string& email, int& userId);
    int getValidInput(int min, int max);
    bool handleRegistration();
    bool handleLogin();
    bool handleLoginWithEmail(const std::string& email);

public:
    AuthManager(ConsoleView& consoleRef);

    bool authenticate();

    const PlayerSession& getSession() const { return session; }

    void resetSession();

    bool isAuthenticated() const { return session.isAuthenticated; }
};
