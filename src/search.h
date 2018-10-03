#include <vector>
#include <string>

#define add_answer(X,Y) answers.push_back(X); count += Y;
struct Search {
    int count = 0;
    std::vector<std::string> answers;
};

struct Ukkonen: Search{

};

struct ShiftOr: Search{

};

struct AhoCorasick: Search{

};