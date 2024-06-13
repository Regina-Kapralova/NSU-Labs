#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

class LineReader {
    ifstream input;
    string str;
public:
    LineReader(string in) {
        input.open(in);
    }
    bool readline() {
        return (bool)getline(input, str);
    }
    string getstr() {
        return str;
    }
};

class TokenService {
    string str;
    vector <string> words;
public:
    void init(string stroka) {
        words.clear();
        str = stroka;
    }
    vector <string> split() {
        string word;
        for (int i = 0; i <= str.size(); ++i) {
            if (str[i] >= 97 && str[i] <= 122 || str[i] >= 65 && str[i] <= 90 || str[i] >= 48 && str[i] <= 57)
                word += str[i];
            else {
                if (word.size() > 0)
                    words.push_back(word);
                word = "";
            }
        }
        return words;
    }
};

class WordStateService {
    int count = 0;
    map <string, int> frequency;
    vector <pair<string, double>> prosent;
    vector <vector<string> > result;

    void createstate() {
        map <string, int>::iterator it;
        for (it = frequency.begin(); it != frequency.end(); ++it)
            prosent.push_back(make_pair((*it).first, (((double)(*it).second / (double)count) * 100)));

        sort(prosent.begin(), prosent.end(), [](auto& left, auto& right) {
            return left.second > right.second;
            });
    }
public:
    void accumulation(vector <string> words) {
        for (int i = 0; i < words.size(); ++i) {
            count++;
            if (frequency.find(words[i]) == frequency.end())
                frequency.insert(pair<string, int>(words[i], 1));
            else
                (frequency.find(words[i]))->second++;
        }
    }
    vector <vector<string>> getresult() {
        createstate();
        result.resize(3);
        for (int i = 0; i < prosent.size(); ++i) {
            result[0].push_back(prosent[i].first);
            result[1].push_back(to_string((*frequency.find(prosent[i].first)).second));
            result[2].push_back(to_string(prosent[i].second));
        }
        return result;
    }
};

class ReportService {
    ofstream output;
public:
    ReportService(string out) {
        output.open(out);
    }
    void print(vector <vector<string>> result) {
        for (int i = 0; i < result[0].size(); ++i) {
            for (int j = 0; j < result.size(); ++j)
                output << result[j][i] << " ; ";
            output << endl;
        }
    }
};

int main(int argc, char* argv[]) {

    // LineReader reader((string)argv[1]);
    LineReader reader("input.txt");
    TokenService tokens;
    WordStateService state;

    while (reader.readline()) {
        tokens.init(reader.getstr());
        state.accumulation(tokens.split());
    }

    // ReportService writer((string)argv[2]);
    ReportService writer("output.csv");
    writer.print(state.getresult());


    return 0;
}