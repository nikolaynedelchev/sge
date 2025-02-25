#pragma once
#include <cinttypes>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace ndn::game_set
{

struct Transaction
{
    uint64_t transactionId;
    uint64_t pageId;
    int win;
};

class Database
{
public:
    std::vector<Transaction> InitAndGetRecoveryTransactions(int workerId, uint64_t pageId);
    void SaveGame(Transaction);

private:
    void OpenFile();
    void ResetFile();
    void CloseFile();
    int m_id;
    int m_gamesInDatabase = 0;
    FILE* m_file = nullptr;
    uint64_t m_pageId = 0;
    std::string m_fileName;
};

}
