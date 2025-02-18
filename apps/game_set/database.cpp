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

std::vector<int> Database::InitAndGetRecoveryGames(int workerId, uint64_t snapshotId)
{
    ThrowIf(m_file != nullptr, "The service is already inited");

    m_id = workerId;
    m_fileName = fmt::format("gameset_{}.db", m_id);

    m_file = fopen(m_fileName.c_str(), "rb");

    std::vector<int> result;
    m_gamesInDatabase = 0;
    m_lastSnapshot = snapshotId;

    if(m_file != nullptr)
    {
        int gameWin = 0;
        uint64_t gameSnapshotId = 0;

        while (fscanf(m_file, "%llu %d", &gameSnapshotId, &gameWin) == 2)
        {
            m_gamesInDatabase++;
            if (gameSnapshotId == snapshotId)
            {
                result.push_back(gameWin);
            }
        }
        fclose(m_file);
    }
    OpenFile();
    return result;
}

void Database::SaveGame(int win, uint64_t snapshotId)
{
    m_gamesInDatabase++;

    if (m_gamesInDatabase > 100'000 &&
        m_lastSnapshot != snapshotId)
    {
        // the file is too big
        // this is the first entry from the new snapshot (m_lastSnapshot != snapshotId)
        // this is the moment to delete the current db and start over
        ResetFile();
        m_gamesInDatabase = 0;
    }

    m_lastSnapshot = snapshotId;
    fprintf(m_file, "%llu %d\n", snapshotId, win);
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
