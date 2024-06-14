#include "pch.h"
#include <string>
using namespace std;


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

TEST(TokenService_test, split_test) {
    TokenService ob1;
    vector <string> words;
    string str;

    str = "()*";
    ob1.init(str);
    words = {};
    EXPECT_EQ(words, ob1.split());

    str = "Wonders of the World";
    ob1.init(str);
    words = { "Wonders", "of", "the", "World"};
    EXPECT_EQ(words, ob1.split());

    str = "Won!ders of* (-) t4he World/ !";
    ob1.init(str);
    words = { "Won", "ders", "of", "t4he", "World" };
    EXPECT_EQ(words, ob1.split());

    str = "14) Won!ders of* (-) t456he World/ WOW!";
    ob1.init(str);
    words = { "14", "Won", "ders", "of", "t456he", "World", "WOW"};
    EXPECT_EQ(words, ob1.split());


    //EXPECT_TRUE(true);
}