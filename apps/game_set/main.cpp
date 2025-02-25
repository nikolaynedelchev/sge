#include <tools/tools.h>
#include <fmt/format.h>
#include "game_set.h"
#include "worker.h"
#include <memory.h>
#include <tools/Stopwatch.h>
#include <tools/Rng.h>
#include <filesystem>

namespace ndn::game_set{int Main();}
int main(){return ::ndn::game_set::Main();}

namespace ndn::game_set
{

static void Cleanup()
{
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator("."))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".db")
        {
            fs::remove(entry.path());
            std::cout << "Deleted: " << entry.path() << '\n';
        }
    }
}

static constexpr size_t s_gameSize = 10'000;
static std::vector<int> GeneratePaytable()
{
    std::vector<int> pt;
    for(size_t i = 0; i < s_gameSize; i++)
    {
        pt.push_back(i);
    }
    return pt;
}

static std::vector<int> GenerateGameset()
{
    std::vector<int> gs;
    for(size_t i = 0; i < s_gameSize; i++)
    {
        gs.push_back(tools::Rng::GetRandom(1'000'000 / (i + 1)));
    }
    return gs;
}

void Test()
{
    std::vector<int> paytable = {0, 1, 2};
    std::vector<int> dist = {30, 20, 10};
    auto gameSet = std::make_shared<GameSet>();
    auto snapshotId = gameSet->InitAndGetSnapshotId("test_gameset", paytable);
    gameSet->ResetGameSet(dist);

    gameSet.reset();
    gameSet = std::make_shared<GameSet>();
    snapshotId = gameSet->InitAndGetSnapshotId("test_gameset", paytable);
}

int Main()
{
    if (true)
    {
        Cleanup();
        Test();
        Cleanup();
    }
    auto paytable = GeneratePaytable();
    auto gameSetDistribution = GenerateGameset();

    auto gameSet = std::make_shared<GameSet>();
    auto snapshotId = gameSet->InitAndGetSnapshotId("gameset", paytable);

    std::vector<std::shared_ptr<Worker>> workers;
    for(size_t i = 0; i < 1; i++)
    {
        workers.push_back(std::make_shared<Worker>());
        auto recoveryGames = workers.back()->InitAndGetRecoveryTransactions(gameSet, i, snapshotId);
        gameSet->RecoverGames(recoveryGames);
        workers.back()->Run();
    }
    tools::Stopwatch sw;
    int displayCounter = 0;
    while(true)
    {
        displayCounter++;
        tools::Stopwatch::SleepForMs(16);
        auto gameSetSize = gameSet->GetGameSetSize();
        if (gameSet->GetGameSetSize() == 0)
        {
            gameSet->ResetGameSet(gameSetDistribution);
        }
        if (displayCounter > 200)
        {
            displayCounter = 0;
            int64_t games = 0;
            for(auto& w : workers) games += w->GetGamesPlayed();
            auto ms = sw.Measure();
            int64_t gamesPerSec = (games * 1000) / ms;
            fmt::print("Total speed: {}, total games: {}, Draw size: {} \r", gamesPerSec, games, gameSetSize);
        }
    }
    return 0;
}

}
