#include <vector>
#include <string>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include "parser.h"

#define add_answer(X,Y) if(!parser->opts.count)std::cout<<X<<"\n"; count += Y; 
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

    int searchLine(const std::string& s) {
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
    #define MSB 0x8000000000000000

    #define shiftI(m)                           \
        m[0] <<= 1;                             \
        for(int a=1 ; a<size ; a++) {           \
            m[a-1] |= m[a] >> 63;     			\
            m[a] <<= 1;                         \
        }                                       \

    #define orI(m, n)                           \
        for(int a=0 ; a<size ; a++) {           \
            m[a] |= n[a];                       \
        }                                       \

    #define andI(m, n)                          \
        for(int a=0 ; a<size ; a++) {           \
            m[a] &= n[a];                       \
        }                                       \

    #define shiftOr(msk, mtc, len)				\
        if(msk == NULL) {						\
            memset(mtc, -1, len);				\
        } else {								\
            shiftI(mtc);						\
            orI(mtc, msk);						\
        }										\

    long** masks = NULL;
    long test;
    long size;

    void setPattern(std::string s, int err = 0) override {

        if(masks != NULL) {
            for(int i=0 ; i<256 ; i++) {
                if(masks[i] != NULL) {
                    delete[] masks[i];
                }
            }
            delete[] masks;
        }
        
        const int lenS = s.size() - 1;
        size = (lenS >> 6) + 1;
        test = 1 << (lenS % 64);
        const int size1 = size - 1;
        
        long mask[size];
        const int buf = size * sizeof(long);
        memset(mask, -1, buf);
        mask[size1] -= 1;

        masks = new long*[256];
        memset(masks, 0, 256 * sizeof(long*));

        for(char c : s) {
            long* aux = masks[c];
            if(aux == NULL) {
                masks[c] = new long[size];
                aux = masks[c];
                memset(aux, -1, buf);
            }
            andI(aux, mask);
            shiftI(mask);
            mask[size1] |= 1;
        }
    }

    void search(Parser* parser) override {
        const int buf = size * sizeof(long);
        const int flagCount = !parser->opts.count;
        long match[size];

        while(parser->has_next_line()) {
            std::string s = parser->next_line();
            memset(match, -1, buf);
            int counter = 0;
            for(char c : s) {
                const long* mask = masks[c];
                shiftOr(mask, match, buf);
                if(~match[0] & test){
                    counter++;
                    if(flagCount){
                        break;
                    }
                }
            }
            if(counter) {
                add_answer(s, counter);
            }
        }
    }
};

struct WuManber: ShiftOr {

    #define copy(a, b)                      \
        for(int i=0 ; i<size ; i++) {       \
            a[i] = b[i];                    \
        }                                   \

    int dist;

    void setPattern(std::string s, int err = 0) override {
        ShiftOr::setPattern(s, err);
        dist = err;
    }

    void search(Parser* parser) override {

        const int size1 = size - 1;
        const int dstSize = dist + 1;
        const int buf = size * sizeof(long);
        const int flagCount = !parser->opts.count;
        const int clrSize = dstSize * size * sizeof(long);
        long matchs[dstSize][size];

        while(parser->has_next_line()) {
            std::string s = parser->next_line() + '\n';
            memset(matchs, -1, clrSize);
            for(int i=1 ; i<dstSize ; i++) {
                matchs[i][size1] -= 1;
            }
            int counter = 0;

            for(char c : s) {

                const long* mask = masks[c];
                long* match = matchs[0];
                long prev1[size];
                long prev2[size];
                long aux[size];

                copy(prev1, match);

                shiftOr(mask, match, buf);

                if(dist > 0){

                    for(int q=1 ; q<dstSize ; q++) {
                        copy(prev2, matchs[q]);
                        match = matchs[q];
                        
                        shiftOr(mask, match, buf);

                        copy(aux, matchs[q-1]);
                        shiftI(aux);
                        andI(match, aux);

                        andI(match, prev1);

                        shiftI(prev1);
                        andI(match, prev1);

                        copy(prev1, prev2);
                    }
                }

                if(~matchs[dist][0] & test) {
                    counter++;
                    if(flagCount){
                        break;
                    }
                }
            }

            if(counter) {
                add_answer(s, counter);
            }
        }
    }

};

// struct AhoCorasick: Search{

// };