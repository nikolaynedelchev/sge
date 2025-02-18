#pragma once
#include <cinttypes>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace ndn::game_set
{

class Database
{
    std::vector<int> InitAndGetRecoveryGames(int workerId, uint64_t snapshotId);
    void SaveGame(int win, uint64_t snapshotId);

private:
    void OpenFile();
    void ResetFile();
    int m_id;
    int m_gamesInDatabase = 0;
    FILE* m_file = nullptr;
    uint64_t m_lastSnapshot = 0;
    std::string m_fileName;
};

}
