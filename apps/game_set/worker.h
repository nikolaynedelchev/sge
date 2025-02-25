#pragma once
#include <cinttypes>
#include "database.h"
#include <atomic>
#include "game_set.h"
#include <memory>
#include <thread>

namespace ndn::game_set
{

class Worker
{
public:
    std::vector<Game> InitAndGetRecoveryTransactions(std::shared_ptr<GameSet> gameSet,
                                                            int workerId, uint64_t snapshotId);

    void Run();
    void Stop();
    int64_t GetGamesPlayed() const;

private:
    void WorkerLoop();

    std::shared_ptr<GameSet> m_gameSet;
    Database m_db;
    std::atomic_bool m_shouldStop;
    std::atomic_int64_t m_gamesPlayed;
    int m_id;
    std::thread m_workerThread;
};

}
