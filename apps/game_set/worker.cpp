#include "worker.h"
#include <tools/Stopwatch.h>

namespace ndn::game_set
{

std::vector<Game> Worker::InitAndGetRecoveryTransactions(std::shared_ptr<GameSet> gameSet,
                                                                int workerId, uint64_t snapshotId)
{
    m_id = workerId;
    m_gameSet = gameSet;
    auto recovery = m_db.InitAndGetRecoveryTransactions(workerId, snapshotId);
    std::vector<Game> games;
    for(const auto& transaction : recovery)
    {
        if (transaction.pageId == snapshotId)
        {
            games.push_back({});
            games.back().isValid = true;
            games.back().snapshotId = transaction.pageId;
            games.back().win = transaction.win;
        }
    }
    return games;
}

void Worker::Run()
{
    m_shouldStop.store(false);
    m_workerThread = std::thread([this](){
        WorkerLoop();
    });
}

void Worker::Stop()
{
    m_shouldStop.store(true);
}

int64_t Worker::GetGamesPlayed() const
{
    return m_gamesPlayed.load();
}

void Worker::WorkerLoop()
{
    while(m_shouldStop.load() == false)
    {
        tools::Stopwatch::SleepForMs(1);

        m_gamesPlayed.fetch_add(1);
        Game game = m_gameSet->DrawGame();
        if (false == game.isValid)
        {
            tools::Stopwatch::SleepForMs(10);
            continue;
        }
        Transaction tr;
        tr.transactionId = m_gamesPlayed.load();
        tr.pageId = game.snapshotId;
        tr.win = game.win;
        m_db.SaveGame(tr);
    }
}

}
