#ifndef _OPTIONS_H
#define _OPTIONS_H

class Options {

public:

    Options(int type, const int score, const int stock, const int time, const bool goBack);
    ~Options();

    const int GetMatchType() const;
    const int GetStock() const;
    const int GetScore() const;
    const int GetTime() const;
    const bool Back() const;

private:
    int mMatchType;
    int mStock;
    int mScore;
    int mTime;
    int mBack;

};
#endif // _OPTIONS_H
