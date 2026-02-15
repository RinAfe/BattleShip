#include "Auth.h"
#include <iostream>
#include <limits>
#include <string>

AuthManager::AuthManager(ConsoleView& consoleRef, pqxx::connection& dbConn)
    : console(consoleRef), conn(dbConn) {}

bool AuthManager::checkEmailExists(const std::string& email) {
    try {
        pqxx::work work(conn);
        pqxx::result res = work.exec(
            "SELECT EXISTS (SELECT 1 FROM players WHERE email = $1)",
            pqxx::params{email}
        );
        work.commit();
        return res[0][0].as<bool>();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при проверке email: " << e.what() << std::endl;
        return false;
    }
}

bool AuthManager::registerUser(const std::string& name, const std::string& email) {
    try {
        pqxx::work work(conn);

        pqxx::result res = work.exec(
            "INSERT INTO players (name, email) VALUES ($1, $2) RETURNING id",
            pqxx::params{name, email}
        );

        work.commit();

        // Сохраняем данные сессии
        currentSession.playerId = res[0][0].as<int>();
        currentSession.name = name;
        currentSession.email = email;
        currentSession.isAuthenticated = true;

        console.registrationSuccess();
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при регистрации: " << e.what() << std::endl;
        console.registrationFailed();
        return false;
    }
}

bool AuthManager::loginUser(const std::string& email) {
    try {
        pqxx::work work(conn);

        pqxx::result res = work.exec(
            "SELECT id, name FROM players WHERE email = $1",
            pqxx::params{email}
        );

        work.commit();

        if (res.empty()) {
            console.authorizationFailed();
            return false;
        }

        currentSession.playerId = res[0][0].as<int>();
        currentSession.name = res[0][1].as<std::string>();
        currentSession.email = email;
        currentSession.isAuthenticated = true;

        console.authorizationSuccess();
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при входе: " << e.what() << std::endl;
        console.authorizationFailed();
        return false;
    }
}

int AuthManager::getValidInput(int min, int max) {
    int input;
    while (true) {
        std::cin >> input;
        if (std::cin.fail() || input < min || input > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            console.incorrectAnswerInMenu();
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return input;
        }
    }
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

    while (true) {
        console.authorization();
        std::getline(std::cin, email);

        if (email.empty()) {
            std::cout << "Email не может быть пустым!" << std::endl;
            continue;
        }

        // Пробуем войти
        if (loginUser(email)) {
            return true; // Успешный вход
        }

        std::cout << "Что хотите сделать?" << std::endl;
        std::cout << "1. Попробовать другой email" << std::endl;
        std::cout << "2. Зарегистрироваться" << std::endl;
        std::cout << "3. Выйти из игры" << std::endl;
        std::cout << "Выберите действие (1/2/3): ";

        int choice = getValidInput(1, 3);

        if (choice == 1) {
        }
        else if (choice == 2) {
            return handleRegistration();
        }
        else {
            return false;
        }
    }
}

bool AuthManager::handleRegistration() {
    std::string name, email;

    while (true) {
        console.registrationEmail();
        std::cin >> email;

        if (email.empty()) {
            std::cout << "Email не может быть пустым!" << std::endl;
            continue;
        }

        if (email.find('@') == std::string::npos) {
            console.invalidEmail();
            continue;
        }

        break;
    }

    // Проверка существования email
    if (checkEmailExists(email)) {
        console.emailAlrdeadyRegistered();

        int choice = getValidInput(1, 2);

        if (choice == 1) {
            return handleLoginWithEmail(email);
        } else {
            console.askForNewEmail();
            return handleRegistration();
        }
    }

    while (true) {
        console.registrationName();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, name);

        if (name.empty()) {
            std::cout << "Имя не может быть пустым!" << std::endl;
            continue;
        }

        if (name.length() > 20) {
            console.nameTooLong();
            continue;
        }

        break; // Имя введено корректно
    }

    // Регистрация пользователя
    return registerUser(name, email);
}

bool AuthManager::handleLoginWithEmail(const std::string& email) {
    return loginUser(email);
}

void AuthManager::resetSession() {
    currentSession = PlayerSession();
}

const PlayerSession& AuthManager::getSession() const {
    return currentSession;
}