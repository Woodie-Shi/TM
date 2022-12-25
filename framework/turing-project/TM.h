#ifndef _TM_H
#define _TM_H
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <regex>
using namespace std;

extern bool VERBOSE;

class Transition {
public:
    string newState;
    string newSym;
    string dir;
    Transition(string nst, string nsym, string d);
    void show_trans();
};

class Tape {
public:
    string content;
    int begin;
    int end;
    int ptr;
    Tape(string t, int head, int tail, int pointer);
};

class TM {
private:
    vector<string> content;
    vector<string> Q;
    vector<string> S;
    vector<string> G;
    string q0;
    char B = '_';
    vector<string> F;
    int N;
    unordered_map<string, unordered_map<string, Transition>> delta;

    string curState;
    vector<Tape> Tapes;
    int steps;
public:
    TM(string path);
    void parse();
    void Stringsplit(string str, const char split, vector<string>& rst);
    void execute(Transition& next);
    void turing(string& input);
    void check(string& input);
    void show_details();
    void show_content();
    void show_parse();
};

#endif