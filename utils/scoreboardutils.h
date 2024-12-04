#ifndef SCOREBOARDUTILS_H
#define SCOREBOARDUTILS_H

#include <vector>
#include <string>
#include <utility>

class ScoreBoardUtils
{
public:
    ScoreBoardUtils();
    void addScore(std::pair<std::string, int> &score);

    const std::vector<std::pair<std::string, int> > &scores() const;

    std::string valueByIndex(int idx);
    void save();
    void load();
    int size();

private:
    std::vector<std::pair<std::string, int>> m_scores;
};

#endif // SCOREBOARDUTILS_H
