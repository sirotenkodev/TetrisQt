#ifndef STATS_H
#define STATS_H

class Stats
{
public:
    Stats();
    void addLines(int lines);
    void addScore(int score);
    int lines() const;
    int score() const;

private:
    int m_lines;
    int m_score;
};

#endif // STATS_H
