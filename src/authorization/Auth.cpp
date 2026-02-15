#include "Auth.h"
#include <iostream>
#include <limits>
#include <cstdlib>

AuthManager::AuthManager(ConsoleView& consoleRef) : console(consoleRef) {}

bool AuthManager::checkEmailExists(const std::string& email) {

    return std::rand() % 2 == 0;
}

bool AuthManager::registerUser(const std::string& name, const std::string& email, int& userId) {
    // Заглушка для регистрации в БД
    userId = std::rand() % 1000 + 1; // Генерируем ID пользователя
    return true;
}

bool AuthManager::loginUser(const std::string& email, int& userId) {
    // Заглушка для авторизации в БД
    userId = std::rand() % 1000 + 1; // Генерируем ID пользователя
    return true;
}

int AuthManager::getValidInput(int min, int max) {
    int input;
    while (!(std::cin >> input) || input < min || input > max) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        console.incorrectAnswerInMenu();
    }
    return input;
}

bool AuthManager::authenticate() {
    console.greetingMenu();

    int choice = getValidInput(1, 2);

    if (choice == 1) {
        return handleLogin();
    } else {
        return handleRegistration();
    }
}

bool AuthManager::handleLogin() {
    std::string email;
    console.authorization();
    std::cin >> email;

    return handleLoginWithEmail(email);
}

bool AuthManager::handleRegistration() {
    std::string name, email;

    console.registrationEmail();
    std::cin >> email;

    if (checkEmailExists(email)) {
        console.emailAlrdeadyRegistered();

        int choice = getValidInput(1, 2);

        if (choice == 1) {
            return handleLoginWithEmail(email);
        } else {
            return handleRegistration();
        }
    } else {
        console.registrationName();
        std::cin >> name;

        int userId;
        if (registerUser(name, email, userId)) {
            session.playerId = userId;
            session.name = name;
            session.email = email;
            session.isAuthenticated = true;
            return true;
        } else {
            return false;
        }
    }
}

bool AuthManager::handleLoginWithEmail(const std::string& email) {
    int userId;
    if (loginUser(email, userId)) {
        session.playerId = userId;
        session.email = email;
        session.isAuthenticated = true;
        // Здесь можно получить имя пользователя из БД
        session.name = "Игрок_" + std::to_string(userId);
        return true;
    } else {
        return false;
    }
}

void AuthManager::resetSession() {
    session = PlayerSession();
}