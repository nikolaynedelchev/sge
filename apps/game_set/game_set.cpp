#include "game_set.h"
#include <thread>
#include <tools/Rng.h>
#include <stdio.h>
#include <stdlib.h>
#include <fmt/format.h>

#define GUARD_THIS_FUNCTION() std::lock_guard<std::mutex> guard(m_locker);
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

int GameSet::InitAndGetSnapshotId(const std::string& gameSetName, std::vector<int> paytable)
{
    GUARD_THIS_FUNCTION();

    m_paytable = std::move(paytable);
    for(size_t i = 0; i < m_paytable.size(); i++)
    {
        ThrowIf(m_idxByWin.insert({m_paytable[i], i}).second == false, "Duplicated paytable elements");
    }
    m_name = gameSetName;
    m_fileName = m_name + "_gameset.db";
    RecoverGameSet();
    return m_snapshotId;
}

void GameSet::ResetGameSet(const std::vector<int> &gamesetDistribution)
{
    GUARD_THIS_FUNCTION();
    m_gamesPlayed = 0;
    m_snapshotId++;
    m_dist.Init(gamesetDistribution);
    FlushGameSet();
}

void GameSet::RecoverGames(const std::vector<Game> &games)
{
    GUARD_THIS_FUNCTION();

    for(const auto& game : games)
    {
        if (game.isValid && game.snapshotId == m_snapshotId)
        {
            m_dist.AddByElementIndex(m_idxByWin.at(game.win));
            m_gamesPlayed++;
        }
    }
}

Game GameSet::DrawGame()
{
    GUARD_THIS_FUNCTION();

    Game game;
    auto count = m_dist.ElementsCount();
    if (count == 0)
    {
        game.isValid = false;
        return game;
    }
    m_gamesPlayed++;
    size_t gameNum = tools::Rng::GetRandom(count - 1);
    size_t gameIdx = m_dist.TakeByElementNumber(gameNum, 1);
    game.win = m_paytable[gameIdx];
    game.isValid = true;
    if (m_gamesPlayed > 1'000'000)
    {
        FlushGameSet();
    }
    game.snapshotId = m_snapshotId;
    return game;
}

size_t GameSet::GetGameSetSize() const
{
    GUARD_THIS_FUNCTION();
    return m_dist.ElementsCount();
}

void GameSet::FlushGameSet()
{
    std::vector<int> mem = m_dist.Dump();
    mem.push_back(m_snapshotId);

    auto file = fopen(m_fileName.c_str(), "wb");
    ThrowIf(file == nullptr, "Error opening file for clearing");

    auto written = fwrite(&mem.front(), sizeof(mem.front()) * mem.size(), 1, file);
    ThrowIf(written != 1, "Write error");
    fflush(file);
    fclose(file);

    m_snapshotId++;
    m_gamesPlayed = 0;
}

void GameSet::RecoverGameSet()
{
    std::vector<int> mem;
    mem.resize(m_paytable.size() + 1);
    auto file = fopen(m_fileName.c_str(), "rb");
    if (file == nullptr)
    {
        for(auto v : mem) v = 0;
    }
    else
    {
        auto r = fread(&mem.front(), sizeof(mem.front()) * mem.size(), 1, file);
        ThrowIf(r != 1, "Write error");
    }
    m_snapshotId = mem.back();
    mem.pop_back();
    m_gamesPlayed = 0;
    m_dist.Init(mem);
}

}
