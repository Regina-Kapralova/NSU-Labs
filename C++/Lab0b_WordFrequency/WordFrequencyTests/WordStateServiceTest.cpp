#include "pch.h"
#include <string>
using namespace std;

class WordStateService {
    int count = 0;
    map <string, int> frequency;
    vector <pair<string, double>> prosent;
    vector < vector <string> > result;
    void createstate() {
        prosent.clear();
        map <string, int>::iterator it;
        for (it = frequency.begin(); it != frequency.end(); ++it)
            prosent.push_back(make_pair((*it).first, (((double)(*it).second / (double)count) * 100)));
        sort(prosent.begin(), prosent.end(), [](auto& left, auto& right) {
            return left.second > right.second;
            });
    }
public:
    map <string, int> accumulation(vector <string> words) {
        for (int i = 0; i < words.size(); ++i) {
            count++;
            if (frequency.find(words[i]) == frequency.end())
                frequency.insert(pair<string, int>(words[i], 1));
            else
                (frequency.find(words[i]))->second++;
        }
        return frequency;
    }

    vector < vector <string> > getresult() {
        createstate();
        result.clear();
        result.resize(3);
        for (int i = 0; i < prosent.size(); ++i) {
            result[0].push_back(prosent[i].first);
            result[1].push_back(to_string((*frequency.find(prosent[i].first)).second));
            result[2].push_back(to_string(prosent[i].second));        }
        return result;
    }
};

TEST(WordStateService_test, accumulation_test) {
    WordStateService ob1;
    vector <string> words;
    map <string, int> frequency;

    words = {};
    frequency = {};
    EXPECT_EQ(frequency, ob1.accumulation(words));

    words = { "Humans", "are", "generally", "good", "at", "building", "beautiful", "things" };
    frequency = { {"Humans", 1}, {"are", 1}, {"generally", 1}, {"good", 1}, {"at", 1}, {"building", 1}, {"beautiful", 1}, {"things", 1} };
    EXPECT_EQ(frequency, ob1.accumulation(words));

    words = { "Humans", "are", "also", "the", "best", "at", "noticing", "beautiful", "things" };
    frequency = { {"Humans", 2}, {"also", 1}, {"are", 2}, {"generally", 1}, {"good", 1}, {"the", 1}, {"best", 1}, {"at", 2}, {"building", 1}, {"noticing", 1}, {"beautiful", 2}, {"things", 2} };
    EXPECT_EQ(frequency, ob1.accumulation(words));
}

TEST(WordStateService_test, getresult_test) {
    WordStateService ob1;
    vector <string> words;
    vector < vector <string> > result;

    words = {};
    ob1.accumulation(words);
    result = { {}, {}, {} };
    EXPECT_EQ(result, ob1.getresult());

    words = { "Humans", "are", "generally", "good", "at", "building", "beautiful", "things" };    
    ob1.accumulation(words);
    result = { 
        { "Humans", "are", "at", "beautiful", "building", "generally", "good", "things" }, 
        { "1", "1", "1", "1", "1", "1", "1", "1" }, 
        { "12.500000", "12.500000", "12.500000", "12.500000", "12.500000", "12.500000", "12.500000", "12.500000" } 
    };
    EXPECT_EQ(result, ob1.getresult());

    words = { "Humans", "are", "also", "the", "best", "at", "noticing", "beautiful", "things" };
    ob1.accumulation(words);
    result = { 
        { "Humans", "are", "at", "beautiful", "things", "also", "best", "building", "generally", "good", "noticing", "the" }, 
        { "2", "2", "2", "2", "2", "1", "1", "1", "1", "1", "1", "1" }, 
        { "11.764706", "11.764706", "11.764706", "11.764706", "11.764706", "5.882353", "5.882353", "5.882353", "5.882353", "5.882353", "5.882353", "5.882353" } 
    };
    EXPECT_EQ(result, ob1.getresult());
}