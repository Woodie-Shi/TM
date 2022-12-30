#include <iostream>
#include "TM.h"
#include <unordered_map>
using namespace std;

bool parse(vector<string>& params, string& path, string& input) {
    if (params.size() == 1 && (params[0] == "turing" || params[0] == "./turing")) {
        cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        return 0;
    }
    if (params.size() < 2) {
        cerr << "syntax error\n";
        exit(-1);
    }
    if (params[0] != "turing" && params[0] != "./turing") {
        cerr << "syntax error\n";
        exit(-1);
    }
    if (params[1] == "--help" || params[1] == "-h") {
        cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        return 0;
    }
    if (params[1] == "-v" || params[1] == "--verbose") {
        VERBOSE = 1;
        if (params.size() != 4) {
            cerr << "syntax error\n";
            exit(-1);
        }
        path = params[2];
        input = params[3];
        return 1;
    }
    if (params.size() != 3) {
        cerr << "syntax error\n";
        exit(-1);
    }
    else {
        path = params[1];
        input = params[2];
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    vector<string> params;
    for (int i = 0; i < argc; i++) {
        params.push_back(argv[i]);
    }
    string path, input;
    bool success = parse(params, path, input);
    if (!success) return 0;
    else {
        TM tm(path);
        //tm.show_content();
        tm.parse();
        //tm.show_parse();
        tm.check(input);
        tm.turing(input);
    }
    return 0;
}