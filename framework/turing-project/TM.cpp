#include "TM.h"
#include <fstream>
#include <iostream>

Transition::Transition(string nst, string nsym, string d)
    : newState(nst)
    , newSym(nsym)
    , dir(d) {}

void Transition::show_trans() {
    cout << dir << ") -> (" << newSym << ", " << newState << ")" << endl;
}

Tape::Tape(string t, int head, int tail, int pointer)
    : content(t)
    , begin(head)
    , end(tail)
    , ptr(pointer) {}

TM::TM(string path) {
    ifstream ifs(path);
    string line;
    if(ifs) {
        while (getline (ifs, line)) { 
            content.push_back(line);
        }
    }
    else {
        cerr << "syntax error" << endl;
        exit(-1);
    }
}

void TM::Stringsplit(string str, const char split, vector<string>& rst) {
    istringstream iss(str);
    string token;
    while (getline(iss, token, split)) {
        rst.emplace_back(token);
    }
}

void TM::parse() {
    regex rQ("#Q = \\{(.+)\\}");
    regex rS("#S = \\{(.+)\\}");
    regex rG("#G = \\{(.+)\\}");
    regex rq0("#q0 = (.+)");
    regex rB("#B = (_)");
    regex rF("#F = \\{(.+)\\}");
    regex rN("#N = ([0-9]+)");
    regex rb("\\s+");
    smatch mQ, mS, mG, mq0, mB, mF, mN, mb;
    bool fQ, fS, fG, fq0, fB, fF, fN, fb;

    for (auto& line : content) {
        string cur = line;
        line = "";
        for (auto& ch : cur) {
            if (ch == ';') break;
            line += ch;
        }
        if (!line.length()) continue;
        // the shortest length
        if (line.length() <= 5) {
            exit(-1);
        }
        fQ = regex_match(line, mQ, rQ);
        fS = regex_match(line, mS, rS);
        fG = regex_match(line, mG, rG);
        fq0 = regex_match(line, mq0, rq0);
        fB = regex_match(line, mB, rB);
        fF = regex_match(line, mF, rF);
        fN = regex_match(line, mN, rN);
        fb = regex_match(line, mb, rb);

        if (fb) {
            continue;
        }
        else if (fQ) {
            Stringsplit(mQ.str(1), ',', Q);
        }
        else if (fS) {
            Stringsplit(mS.str(1), ',', S);
        }
        else if (fG) {
            Stringsplit(mG.str(1), ',', G);
        }
        else if (fq0) {
            q0 = mq0.str(1);
            curState = q0;
        }
        else if (fB) {
            continue;
        }
        else if (fF) {
            Stringsplit(mF.str(1), ',', F);
        }
        else if (fN) {
            N = stoi(mN.str(1));
        }
        else {
            vector<string> tmp;
            Stringsplit(line, ' ', tmp);
            if (tmp.size() != 5) {
                cerr << "syntax error" << endl;
                exit(-1);
            }
            bool one_in_map = delta.find(tmp[0]) != delta.end() ? true : false;
            if (one_in_map) {
                Transition* trans = new Transition(tmp[4], tmp[2], tmp[3]);
                delta[tmp[0]].insert(make_pair(tmp[1], *trans));
            }
            else {
                Transition* trans = new Transition(tmp[4], tmp[2], tmp[3]);
                unordered_map<string, Transition> m;
                m.insert(make_pair(tmp[1], *trans));
                delta.insert(make_pair(tmp[0], m));
            }
        }
    }
}

void TM::turing(string& input) {
    steps = 0;
    for (int i = 0; i < N; i++) {
        Tape temptape("", 0, 0, 0);
        if (i == 0) {
            temptape.content = input;
            if (input == "") {
                temptape.content = "_";
            }
            temptape.end = temptape.content.length() - 1;
        }
        else {
            temptape.content = "_";
        }
        Tapes.emplace_back(temptape);
    }
    while (true) {
        if (VERBOSE) show_details();
        
        auto trans = delta.find(curState);
        if (trans == delta.end()) break;

        string curSymbol = "";
        for (int i = 0; i < N; i++) {
            curSymbol.push_back(Tapes[i].content[Tapes[i].ptr - Tapes[i].begin]);
        }
        
        auto& mp = trans->second;
        auto nxt = mp.find(curSymbol);
        
        if (nxt == mp.end()) break;
        else {
            execute(nxt->second);
            steps++;
        }
    }

    if (VERBOSE) {
        cout << "Result: " << Tapes[0].content << endl;
        cout << "==================== END ====================\n";
    }
    else {
        cout << Tapes[0].content << endl;
    }
}

void TM::execute(Transition& next) {
    curState = next.newState;
    string newSymbols = next.newSym;
    string directions = next.dir;

    for (int i = 0; i < N; i++) {
        Tapes[i].content[Tapes[i].ptr - Tapes[i].begin] = newSymbols[i];
        int preptr = Tapes[i].ptr;
        if (directions[i] == 'l') {
            Tapes[i].ptr--;
        }
        else if (directions[i] == 'r') {
            Tapes[i].ptr++;
        }
        // if the tail is empty
        if (Tapes[i].ptr > Tapes[i].end) {
            Tapes[i].content.push_back('_');
            Tapes[i].end = Tapes[i].ptr;
        }
        // if the head is empty
        if (Tapes[i].ptr < Tapes[i].begin) {
            Tapes[i].content.insert(0, "_");
            Tapes[i].begin = Tapes[i].ptr;
        }
        
        // handle useless blanks 
        if (Tapes[i].ptr >= Tapes[i].begin && Tapes[i].ptr <= Tapes[i].end) {
            if (Tapes[i].ptr == Tapes[i].begin) {
                if (Tapes[i].content.length() > 1 && Tapes[i].content.back() == '_') {
                    Tapes[i].end--;
                    Tapes[i].content.pop_back();
                }
            }
            else if (Tapes[i].ptr == Tapes[i].end) {
                if (Tapes[i].content.length() > 1 && Tapes[i].content[0] == '_') {
                    Tapes[i].begin++;
                    Tapes[i].content.erase(0, 1);
                }
            }
            else {
                if (Tapes[i].ptr + 1 - Tapes[i].begin < Tapes[i].content.length() && Tapes[i].content.back() == '_') {
                    Tapes[i].end--;
                    Tapes[i].content.pop_back();
                }
                
                if (Tapes[i].ptr - 1 - Tapes[i].begin >= 0 && Tapes[i].content[0] == '_') {
                    Tapes[i].begin++;
                    Tapes[i].content.erase(0, 1);
                }
            }
        }
    }
}

void TM::show_details() {
    cout << "Step   : " << steps << endl;
    for (int i = 0; i < N; i++) {
        string index = "Index";
        string tape = "Tape";
        string Head = "Head";
        int t = i / 10;
        if (t == 0) {
            index.push_back((char)(i + '0'));
            index.push_back(' ');
            index.push_back(':');
            index.push_back(' ');
            tape.push_back((char)(i + '0'));
            tape = tape + "  : ";
            Head.push_back((char)(i + '0'));
            Head = Head + "  : ";
        }
        else {
            index.push_back((char)(t + '0'));
            index.push_back((char)(i % 10 + '0'));
            index.push_back(':');
            index.push_back(' ');
            tape.push_back((char)(t + '0'));
            tape.push_back((char)(i % 10 + '0'));
            tape = tape + " : ";
            Head.push_back((char)(t + '0'));
            Head.push_back((char)(i % 10 + '0'));
            Head = Head + " : ";
        }
        cout << index;
        for (int j = Tapes[i].begin; j <= Tapes[i].end; j++) {
            if (j < 0) {
                cout << 0 - j;
            }
            else {
                cout << j;
            }
            //if(j!=setOfTape[i].index_end){
            cout << " ";
            //}
        }
        cout << endl;
        cout << tape;
        for (int j = 0; j < Tapes[i].content.length(); j++) {
            cout << Tapes[i].content[j];
            if ((Tapes[i].begin + j < 10) && (Tapes[i].begin + j > -10)) {
                cout << " ";
            }
            else if (((Tapes[i].begin + j >= 10) && (Tapes[i].begin + j < 100)) || ((Tapes[i].begin + j > -100) && (Tapes[i].begin + j <= -10))) {
                cout << "  ";
            }
            else {
                cout << "   ";
            }
        }
        cout << endl;
        cout << Head;
        int pointpos = Tapes[i].ptr - Tapes[i].begin;
        for (int j = 0; j < pointpos; j++) {
            cout << " ";
            if ((Tapes[i].begin + j < 10) && (Tapes[i].begin + j > -10)) {
                cout << " ";
            }
            else if (((Tapes[i].begin + j >= 10) && (Tapes[i].begin + j < 100)) || ((Tapes[i].begin + j > -100) && (Tapes[i].begin + j <= -10))) {
                cout << "  ";
            }
            else {
                cout << "   ";
            }
        }
        cout << "^" << endl;

    }
    cout << "State  : " << curState << endl;
    cout << "---------------------------------------------\n";
}

void TM::show_content() {
    for (auto &it: content) {
        cout << it << endl;
    }
}

void TM::show_parse() {
    cout << "States: ";
    for (auto& it : Q) {
        cout << it << " ";
    }
    cout << "\nInput Symbols: ";
    for (auto& it : S) {
        cout << it << " ";
    }
    cout << "\nTape Symbols: ";
    for (auto& it : G) {
        cout << it << " ";
    }
    cout << "\nInital State: " << q0 << endl;
    cout << "blank symbol: " << "_" << endl;
    cout << "Final States: ";
    for (auto& it : F) {
        cout << it << " ";
    }
    cout << "\nTransition Functions: \n";
    for (auto& it : delta) {
        for (auto& iter : it.second) {
            cout <<"(" << it.first << ", " << iter.first << ", ";
            iter.second.show_trans();
        }
    }
}