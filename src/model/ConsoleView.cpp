#include "ConsoleView.h"

void ConsoleView::printBoards(std::unique_ptr<Board>& myBoard, std::unique_ptr<Board>& enemyBoard) {
    std::cout << "МОЕ ПОЛЕ";
    std::cout << std::string(20, ' ');
    std::cout << "ПОЛЕ ПРОТИВНИКА\n";

    std::cout << "   ";
    for (char i = 'A'; i < 'A' + 10; i++) {
        std::cout << i << " ";
    }
    std::cout << "       ";
    for (char i = 'A'; i < 'A' + 10; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < 10; i++) {
        std::cout << (i < 9 ? " " : "") << i + 1 << " ";

        for (size_t j = 0; j < 10; j++) {
            std::cout << myBoard->cellAt(i, j) << " ";
        }

        std::cout << "    ";

        std::cout << (i < 9 ? " " : "") << i + 1 << " ";

        for (size_t j = 0; j < 10; j++) {
            char cell = enemyBoard->cellAt(i, j);

            if (cell == '.' || cell == 'X' || cell == 'O') {
                std::cout << cell << " ";
            }
            else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

void ConsoleView::clearBoard() {
    std::cout << "Ваше игровое поле очищено!";
}

void ConsoleView::chooseMode() {
    std::cout << "Выберите режим установки кораблей(1 - авто, 2 - вручную): ";
}

void ConsoleView::incorrectMode() {
    std::cout << "Неверный режим, введите еще раз: ";
}

void ConsoleView::printRules() {
    std::cout << "=== МОРСКОЙ БОЙ ===\n\n";
    std::cout << "ПРАВИЛА ИГРЫ:\n";
    std::cout << "• Игровое поле 10x10 клеток\n";
    std::cout << "• Флот: 1x4, 2x3, 3x2, 4x1\n";
    std::cout << "• Корабли не могут соприкасаться\n";
    std::cout << "• Стрельба по координатам (A1-J10)\n\n";

    std::cout << "РЕЖИМЫ ИГРЫ:\n";
    std::cout << "• PvP - Игрок против Игрока\n";
    std::cout << "• PvAI - Игрок против Компьютера\n\n";

    std::cout << "РАССТАНОВКА КОРАБЛЕЙ:\n";
    std::cout << "• Авто - случайная расстановка\n";
    std::cout << "• Ручная - самостоятельная расстановка\n\n";

    std::cout << "УДАЧИ В БОЮ!\n";
}

void ConsoleView::inputCoordinates() {
    std::cout << "Введите координаты (например A1): ";
}

void ConsoleView::incorrectForm() {
    std::cout << "Неверный формат! Используйте A1-J10\n";
}

void ConsoleView::alreadyShoot() {
    std::cout << "Вы уже стреляли сюда!\n";
}

void ConsoleView::showResult(ShotResult& result) {
    switch (result) {
    case ShotResult::Miss:
        std::cout << "Промах!\n";
        break;
    case ShotResult::Hit:
        std::cout << "Попадание!\n";
        break;
    case ShotResult::Kill:
        std::cout << "Корабль потоплен!\n";
        break;
    }
}

void ConsoleView::greeting() {
    std::cout << "Добро пожаловать в игру МОРСКОЙ БОЙ!" << std::endl;
    std::cout << "1 - Начать игру" << std::endl;
    std::cout << "2 - Инструкция" << std::endl;
    std::cout << "Введите число: ";
}

void ConsoleView::incorrectChoseInMenu() {
    std::cout << "Неверное число, введите еще раз: ";
}

void ConsoleView::incorrectStartMode() {
    std::cout << "Неверное число, попробуйте еще раз!" << std::endl;
    std::cout << "Введите число: ";
}

void ConsoleView::newMove() {
    std::cout << "Новый круг!" << std::endl;
}

void ConsoleView::printBoardForPlacement(Board& board) const {
    std::cout << "\n  ";
    for (char c = 'A'; c <= 'J'; c++) {
        std::cout << c << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < 10; i++) {
        std::cout << (i + 1) << (i < 9 ? " " : "");
        for (int j = 0; j < 10; j++) {
            std::cout << board.cellAt(i, j) << " ";
        }
        std::cout << "\n";
    }
}

void ConsoleView::showShipPlacementInfo(ShipType type, int length, int remaining) {
    std::cout << "\nРазмещаем ";
    switch (type) {
        case ShipType::Battleship: std::cout << "линкор"; break;
        case ShipType::Cruiser: std::cout << "крейсер"; break;
        case ShipType::Destroyer: std::cout << "эсминец"; break;
        case ShipType::Patrol: std::cout << "катер"; break;
    }
    std::cout << " (длина " << length << "). ";
    std::cout << "Осталось разместить: " << remaining << " кораблей\n";
}

void ConsoleView::showPlacementSuccess() {
    std::cout << "Корабль успешно размещен!\n";
}

void ConsoleView::showPlacementError() {
    std::cout << "Нельзя разместить корабль здесь! Попробуйте снова.\n";
}

std::string ConsoleView::getPlacementCoordinates() {
    std::cout << "Введите начальную координату (например, A1 или введите 0, чтобы очистить поле): ";
    std::string input;
    std::cin >> input;
    return input;
}

char ConsoleView::getPlacementOrientation() {
    std::cout << "Выберите ориентацию (H - горизонтально, V - вертикально): ";
    char orientation;
    std::cin >> orientation;
    return orientation;
}

void ConsoleView::showAllShipsPlaced() {
    std::cout << "Все корабли размещены!\n";
}

void ConsoleView::greetingMenu() {
    std::cout << "Добро пожаловать в игру Морской Бой!" << std::endl;
    std::cout << "1. Авторизация" << std::endl;
    std::cout << "2. Регистрация" << std::endl;
    std::cout << "Выберите действие(1/2): ";
}

void ConsoleView::incorrectAnswerInMenu() {
    std::cout << "Неверное число, пожалуйста, введите корректное число(1/2): ";
}

void ConsoleView::authorization() {
    std::cout << "Введите свою почту: ";
}

void ConsoleView::registrationName() {
    std::cout << "Введите свое имя: " << std::endl;
}

void ConsoleView::registrationEmail() {
    std::cout << "Введите свою почту: " << std::endl;
}

void ConsoleView::emailAlrdeadyRegistered() {
    std::cout << "Такой email уже зарегистрирован. Авторизоваться?" << std::endl;
    std::cout << "1 - Да\n2 - Нет: ";
}

void ConsoleView::incorrectAnswerInRegister() {
    std::cout << "Неправильная цифра, желаете авторизоваться?" << std::endl;
    std::cout << "1 - Да\n2 - Нет: ";
}

void ConsoleView::registrationSuccess() {
    std::cout << "Регистрация успешно завершена!" << std::endl;
}

void ConsoleView::registrationFailed() {
    std::cout << "Ошибка при регистрации. Попробуйте снова." << std::endl;
}

void ConsoleView::authorizationSuccess() {
    std::cout << "Авторизация успешна! Добро пожаловать!" << std::endl;
}

void ConsoleView::authorizationFailed() {
    std::cout << "Ошибка авторизации. Неверный email." << std::endl;
}

void ConsoleView::askForNewEmail() {
    std::cout << "Введите другой email для регистрации: " << std::endl;
}

void ConsoleView::errorToLoginInSystem() {
    std::cout << "Не удалось войти в систему. Выход." << std::endl;
}

void ConsoleView::player1Welcome() {
    std::cout << "\n=== ИГРОК 1 ===" << std::endl;
}

void ConsoleView::player2Welcome() {
    std::cout << "\n=== ИГРОК 2 ===" << std::endl;
}

void ConsoleView::bothPlayersAuthorized() {
    std::cout << "\nОба игрока авторизованы! Начинаем игру..." << std::endl;
}

void ConsoleView::switchingPlayers() {
    std::cout << "\nПередача хода другому игроку..." << std::endl;
    std::cout << "Нажмите Enter, чтобы продолжить...";
    std::cin.ignore();
    std::cin.get();
}

void ConsoleView::playerTurn(int playerNumber) {
    std::cout << "\n=== ХОД ИГРОКА " << playerNumber << " ===" << std::endl;
}

void ConsoleView::messageExistingGame() {
    std::cout << "Загрузка незавершенной игры..." << std::endl;
}

void ConsoleView::invalidEmail() const {
    std::cout << "Неверный формат email! Email должен содержать символ '@'." << std::endl;
}

void ConsoleView::nameTooLong() const {
    std::cout << "Имя слишком длинное! Максимальная длина - 10 символов." << std::endl;
}

void ConsoleView::showMainMenu() {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << std::endl;
    std::cout << "1 - Игра с компьютером" << std::endl;
    std::cout << "2 - Игра с человеком" << std::endl;
    std::cout << "3 - Список активных игр" << std::endl;
    std::cout << "0 - Выход" << std::endl;
    std::cout << "Выберите режим: ";
}

void ConsoleView::showActiveGamesMenu(const std::vector<ActiveGameInfo>& games) {
    std::cout << "\n=== АКТИВНЫЕ ИГРЫ ===" << std::endl;

    for (size_t i = 0; i < games.size(); i++) {
        std::cout << i + 1 << ". Против: " << games[i].opponentName;

        if (games[i].gameType == "AI") {
            std::cout << " (компьютер)";
        } else {
            std::cout << " (игрок)";
        }

        std::cout << " - " << (games[i].isMyTurn ? "ТВОЙ ХОД" : "ХОД ПРОТИВНИКА");
        std::cout << std::endl;
    }
    std::cout << "0 - Назад в главное меню" << std::endl;
    std::cout << "Выберите игру: ";
}

void ConsoleView::noActiveGames() {
    std::cout << "У вас нет незавершенных игр." << std::endl;
}

void ConsoleView::pressEnterToContinue() {
    std::cout << "Нажмите Enter, чтобы продолжить...";
    std::cin.get();
}

void ConsoleView::gameIsOver() {
    std::cout << "Игра окончена!" << std::endl;
}

