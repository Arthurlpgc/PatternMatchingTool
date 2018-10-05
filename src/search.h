#include <vector>
#include <string>
#include <map>
#include <queue>
#include <math>
#include <set>

#define add_answer(X,Y) answers.push_back(X); count += Y;
struct Search {
    int count = 0;
    std::vector<std::string> answers;
};

typedef int ukkState;
typedef std::pair<ukkState, int> ukkStateTransition;
struct Ukkonen: Search {
    std::set<ukkState> F;
    std::map<ukkStateTransition, ukkState> delta;
    std::map<char, int> keyMap;
    std::string pattern;
    int pattern_size;
    std::vector<int> patt_as_int;

    std::vector<int> make_transition(const std::vector<int>& base, int chr, int err) {
        std::vector<int> state = std::vector<int>(0, pattern_size + 1);
        for(int i = 1; i <= pattern_size; i++) {
            state[i] = min(min(base[i] + 1, base[i-1] + (chr != patt_as_int[i-1] ? 1 : 0)),min(err+1, state[i-1]+1));
        }
        return state;
    }

    void setPattern(std::string s, int err = 0) {
        pattern = s;
        pattern_size = pattern.size();
        F.clear();
        delta.clear();
        keyMap.clear();

        int c_id = 1;

        std::vector<int> state;
        for(int i = 0; i <= s.length(); i++) {
            state.push_back(i);
            if(i < s.length()) {
                if(!keyMap[s[i]]) {
                    keyMap[s[i]] = c_id++;
                }
                patt_as_int.push_back(keyMap[s[i]]);
            }
        }

        std::queue<int> q;
        std::map<ukkState, std::vector<int> > stateMap;
        std::map<std::vector<int>, ukkState > revStateMap;
        stateMap[1] = state;
        revStateMap[state] = 1;
        q.push(1);
        int next_free_id = 2;
        while(!q.empty()) {
            int now = q.front();
            q.pop();
            state = stateMap[now];
            for(int i = 0; i < c_id; i++){ 
                auto next_state = make_transition(state, i, err);
                int next_state_id = revStateMap[next_state];
                if(!next_state_id) { 
                    revStateMap[next_state] = next_free_id;
                    stateMap[next_free_id++] = next_state;
                }
            }
        }

    }
};

struct ShiftOr: Search{

};

struct AhoCorasick: Search{

};