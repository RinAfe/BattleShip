#pragma once

#include <pqxx/pqxx>
#include <memory>
#include <vector>
#include <string>
#include "Board.h"

struct ActiveGameInfo {
    int gameId;
    int gameRecordId;
    int opponentId;
    std::string opponentName;
    std::string gameType;
    int currentTurnId;
    bool isMyTurn;
};

struct SavedGame {
    int activeGameId;
    int gameRecordId;
    int player1Id;
    int player2Id;
    std::string player1Name;
    std::string player2Name;
    std::unique_ptr<Board> player1Board;
    std::unique_ptr<Board> player2Board;
    int currentTurn;
    std::string gameType;
};

class GameSaver {
private:
    pqxx::connection& conn;

public:
    GameSaver(pqxx::connection& dbConn);

    int createNewGame(int player1Id, int player2Id, const std::string& gameType,
                      Board& player1Board, Board& player2Board, int currentTurnId);

    bool finishGame(int gameRecordId, const std::string& winner);

    std::vector<ActiveGameInfo> getActiveGames(int playerId);

    std::unique_ptr<SavedGame> loadGameById(int activeGameId);

    bool updateGame(int activeGameId, Board& player1Board, Board& player2Board, int currentTurnId);

    bool deleteActiveGame(int activeGameId);

    bool hasActiveGame(int player1Id, int player2Id);

    void updatePlayerStats(int playerId, bool playerWon);
};