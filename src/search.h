#include <vector>
#include <string>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include "parser.h"

#define add_answer(X,Y) if(!parser->count)std::cout<<X<<"\n"; count += Y; 
struct Search {
    int count = 0;
    virtual void setPattern(std::string s, int err = 0) = 0;
    virtual void search(Parser* s) = 0;
};

typedef int ukkState;
struct Ukkonen: Search {
    std::set<ukkState> F;
    std::map<ukkState, ukkState> delta[256];
    int keyMap[256];
    std::string pattern;
    int pattern_size;
    std::vector<int> patt_as_int;

    std::vector<int> make_transition(const std::vector<int>& base, int chr, int err) {
        std::vector<int> state = std::vector<int>(pattern_size + 1, 0);
        for(int i = 1; i <= pattern_size; i++) {
            state[i] = std::min(std::min(base[i] + 1, base[i-1] + (chr != patt_as_int[i-1] ? 1 : 0)), std::min(err+1, state[i-1]+1));
        }
        return state;
    }

    void setPattern(std::string s, int err = 0) override {
        memset(keyMap, 0, sizeof keyMap);
        pattern = s;
        pattern_size = pattern.size();
        F.clear();
        for(int i = 0; i < 256; i++)delta[i].clear();

        int c_id = 1;

        std::vector<int> state;
        for(int i = 0, sze = s.length(); i <= sze; i++) {
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
            for(int i = 0; i < c_id; i++) { 
                auto next_state = make_transition(state, i, err);
                int next_state_id = revStateMap[next_state];
                if(!next_state_id) {
                    revStateMap[next_state] = next_free_id;
                    stateMap[next_free_id] = next_state;
                    next_state_id = next_free_id++;
                    q.push(next_state_id);
                }
                if(next_state[pattern_size] <= err) {
                    F.insert(next_state_id);
                }
                delta[i][now] = next_state_id;
            }
        }

    }

    inline int searchLine(const std::string& s) {
        int state = 1, occ = 0;
        if(F.count(state)){
            occ++;
        }
        for(auto c: s){
            state = delta[keyMap[c]][state];
            if(F.count(state)){
                occ++;
            }
        }
        return occ;
    }

    void search(Parser* parser) override {
        while(parser->has_next_line()){
            auto s = parser->next_line();
            auto cnt = searchLine(s);
            if(cnt) 
                add_answer(s, cnt);
        }
    }
};

struct ShiftOr: Search {
    const long MSB = 0x8000000000000000;
    std::vector< std::vector<long> > masks;
    std::vector<long> mask;
    std::string pat;
    int size;

    inline void shiftI(std::vector<long>& m) {
        m[0] <<= 1;
        for(int a=1, len=m.size() ; a<len ; a++) {
            m[a-1] |= (m[a] & MSB ? 1 : 0);
            m[a] <<= 1;
        }
    }

    inline void orI(std::vector<long>& m, std::vector<long>& n) {
        for(int a=0, len=m.size() ; a<len ; a++) {
            m[a] |= n[a];
        }
    }

    inline void andI(std::vector<long>& m, std::vector<long>& n) {
        for(int a=0, len=m.size() ; a<len ; a++) {
            m[a] &= n[a];
        }
    }

    void setPattern(std::string s, int err = 0) override {
        masks.assign(256, std::vector< long >() );
        
        size = ((s.size() - 1) >> 6) + 1;
        mask.assign(size, -1);
        mask[size - 1] -= 1;

        pat = s;

        for(int i=0, len=s.size() ; i<len ; i++) {
            if(masks[s[i]].size() == 0) {
                masks[s[i]].assign(size, -1);
            }
            andI(masks[s[i]], mask);
            shiftI(mask);
            mask[size - 1] |= 1;
        }
    }

    void search(Parser* parser) override {
        long test = 1 << ((pat.size() - 1) % 64);
        std::vector<long> match(size, -1);

        for(; parser->has_next_line() ; ) {
            auto s = parser->next_line();
            int counter = 0;
            for(int j=0, lenJ=s.size() ; j<lenJ ; j++) {
                if(masks[s[j]].size() == 0) {
                    shiftI(match);
                    orI(match, mask);
                } else {
                    shiftI(match);
                    orI(match, masks[s[j]]);
                }
                if(~match[0] & test){
                    counter++;
                }
            }

            if(counter) {
                add_answer(s, counter);
            }
            std::fill(match.begin(), match.end(), -1);
        }
    }
};

struct WuManber: ShiftOr {
    int dist;

    void setPattern(std::string s, int err = 0) override {
        ShiftOr::setPattern(s, err);
        dist = err;
    }

    void search(Parser* parser) override {

        long test = 1 << ((pat.size() - 1) % 64);
        std::vector< std::vector<long> > matchs;
        matchs.assign(dist+1, std::vector<long>(size, -1) );

        while(parser->has_next_line()) {
            auto s = parser->next_line();
            int counter = 0;
            for(int j=0, lenJ=s.size() ; j<lenJ ; j++) {
                if(masks[s[j]].size() == 0) {
                    shiftI(matchs[0]);
                    orI(matchs[0], mask);
                } else {
                    shiftI(matchs[0]);
                    orI(matchs[0], masks[s[j]]);
                }

                std::vector<long> sprev2;
                std::vector<long> sprev = matchs[0];
                for(int q=1, lenQ=dist+1 ; q<lenQ ; q++) {
                    sprev2 = matchs[q];
                    
                    if(masks[s[j]].size() == 0) {
                        shiftI(matchs[q]);
                        orI(matchs[q], mask);
                    } else {
                        shiftI(matchs[q]);
                        orI(matchs[q], masks[s[j]]);
                    }

                    std::vector<long> aux1(matchs[q-1]);
                    shiftI(aux1);
                    andI(matchs[q], aux1);

                    std::vector<long> aux2(sprev);
                    shiftI(aux2);
                    andI(matchs[q], aux2);

                    andI(matchs[q], sprev);

                    sprev = sprev2;
                }

                if(~matchs[dist][0] & test){
                    counter++;
                }
            }

            if(counter) {
                add_answer(s, counter);
            }

            for(int i=0, lenI=dist+1 ; i<lenI ; i++) {
                std::vector<long>& match = matchs[i];
                std::fill(match.begin(), match.end(), -1);
            }
        }
    }

};

// struct AhoCorasick: Search{

// };