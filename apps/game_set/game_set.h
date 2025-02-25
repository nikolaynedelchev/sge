#pragma once
#include <cinttypes>
#include <vector>
#include "distribution.h"
#include <mutex>
#include <map>
namespace ndn::game_set
{

struct Game
{
    bool isValid;
    int snapshotId;
    int win;
};

class GameSet
{
public:
    int InitAndGetSnapshotId(const std::string& gameSetName, std::vector<int> paytable);
    void ResetGameSet(const std::vector<int>& gamesetDistribution);
    void RecoverGames(const std::vector<Game>& games);
    Game DrawGame();
    size_t GetGameSetSize() const;

private:
    void FlushGameSet();
    void RecoverGameSet();
    Distribution m_dist;
    int m_gamesPlayed = 0;
    int m_snapshotId = 0;
    std::vector<int> m_paytable;
    std::map<int, size_t> m_idxByWin;
    std::string m_name;
    std::string m_fileName;
    mutable std::mutex m_locker;
};

}
