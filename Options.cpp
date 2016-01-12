#include "Options.hpp"

Options::Options(int type, const int score, const int stock, const int time) :
    mMatchType(type),
    mStock(stock),
    mScore(score),
    mTime(time)
{

}

const int Options::GetMatchType() const {
    return mMatchType;
}

const int Options::GetStock() const {
    return mStock;
}

const int Options::GetScore() const {
    return mScore;
}

const int Options::GetTime() const {
    return mTime;
}
