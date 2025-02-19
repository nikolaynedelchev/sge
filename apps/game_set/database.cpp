#include "database.h"
#include <string>
#include <fmt/format.h>
namespace ndn::game_set
{

static inline void ThrowIf(bool cond, const char* msg)
{
    if (cond)
    {
        fmt::println(msg);
        throw std::logic_error(msg);
    }
}

std::vector<Transaction> Database::InitAndGetRecoveryTransactions(int workerId, uint64_t pageId)
{
    ThrowIf(m_file != nullptr, "The service is already inited");

    m_id = workerId;
    m_fileName = fmt::format("gameset_{}.db", m_id);

    m_file = fopen(m_fileName.c_str(), "rb");

    std::vector<Transaction> result;
    m_gamesInDatabase = 0;
    m_pageId = pageId;

    if(m_file != nullptr)
    {
        Transaction tr;

        while (fscanf(m_file, "%llu %llu %d", &tr.transactionId, &tr.pageId, &tr.win) == 2)
        {
            m_gamesInDatabase++;
            if (tr.pageId == pageId)
            {
                result.push_back(tr);
            }
        }
        fclose(m_file);
    }
    OpenFile();
    return result;
}

void Database::SaveGame(Transaction tr)
{
    m_gamesInDatabase++;

    if (m_gamesInDatabase > 100'000 &&
        m_pageId != tr.pageId)
    {
        // the file is too big
        // this is the first entry from the new snapshot (m_lastSnapshot != snapshotId)
        // this is the moment to delete the current db and start over
        ResetFile();
        m_gamesInDatabase = 0;
    }

    m_pageId = tr.pageId;
    fprintf(m_file, "%llu %llu %d\n", tr.transactionId, tr.pageId, tr.win);
    fflush(m_file);
}

void Database::OpenFile()
{
    ThrowIf(m_file != nullptr, "Close file before open it");

    m_file = fopen(m_fileName.c_str(), "ab");
    ThrowIf(m_file == nullptr, "Error opening file for appending");

    fseek(m_file, 0, SEEK_END);
}

void Database::ResetFile()
{
    if (m_file != nullptr)
    {
        fclose(m_file);
    }

    m_file = fopen(m_fileName.c_str(), "wb");
    ThrowIf(m_file == nullptr, "Error opening file for clearing");
}

}
