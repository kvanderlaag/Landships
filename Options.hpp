#ifndef _OPTIONS_H
#define _OPTIONS_H

class Options {

public:

    Options(int type, const int score, const int stock, const int time);
    ~Options();

    const int GetMatchType() const;
    const int GetStock() const;
    const int GetScore() const;
    const int GetTime() const;

private:
    int mMatchType;
    int mStock;
    int mScore;
    int mTime;

};
#endif // _OPTIONS_H
