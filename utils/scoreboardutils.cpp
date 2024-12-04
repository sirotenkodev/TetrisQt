#include <algorithm>
#include <fstream>

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "scoreboardutils.h"

//-----------------------------------------------------------------------------
ScoreBoardUtils::ScoreBoardUtils()
{
    load();
}

//-----------------------------------------------------------------------------
void ScoreBoardUtils::addScore(std::pair<std::string, int> &score)
{
    m_scores.push_back(score);
    std::sort(
            m_scores.begin(),
            m_scores.end(),
            [](const auto& p1, const auto& p2) {
                return std::tie(p2.second) < std::tie(p1.second);
            }
     );
}

//-----------------------------------------------------------------------------
const std::vector<std::pair<std::string, int> > &ScoreBoardUtils::scores() const
{

    return m_scores;
}

//-----------------------------------------------------------------------------
std::string ScoreBoardUtils::valueByIndex(int idx)
{
    if(m_scores.empty() || idx > m_scores.size()) {
        return "";
    } else {
        std::string s = m_scores[idx].first;
        s += "\t";
        s += std::to_string(m_scores[idx].second);
        return s;
    }
}

//-----------------------------------------------------------------------------
void ScoreBoardUtils::save()
{
    std::ofstream outFile("leaders.json", std::ios_base::out | std::ios_base::trunc);
    nlohmann::json j(m_scores);
    outFile << j;
}

//-----------------------------------------------------------------------------
void ScoreBoardUtils::load()
{
    std::ifstream loadFile("leaders.json");

    if(loadFile.is_open()) {
        m_scores.clear();
        nlohmann::json j;
        loadFile >> j;
        for(const auto &a : j) {
            const auto &i = a.template get<std::pair<std::string, int>>();
            m_scores.push_back(i);
        }
    }
}

//-----------------------------------------------------------------------------
int ScoreBoardUtils::size()
{
    return m_scores.size();
}
