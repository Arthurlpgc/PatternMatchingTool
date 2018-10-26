#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>

struct Search {
    virtual void setPattern(std::string s, int err = 0) = 0;
    virtual int search(std::string s) = 0;
};

typedef int ukkState;
struct Ukkonen: Search {
    std::set<ukkState> F;
    std::unordered_map<ukkState, ukkState> delta[256];
    std::string pattern;
    int pattern_size;
    int err;

    inline std::vector<int> make_transition(const std::vector<int>& base, int chr) {
        std::vector<int> state = std::vector<int>(pattern_size + 1, 0);
        for(int i = 1; i <= pattern_size; i++) {
            state[i] = std::min(std::min(base[i] + 1, base[i-1] + (chr != pattern[i-1] ? 1 : 0)), std::min(err+1, state[i-1]+1));
        }
        return state;
    }

    void setPattern(std::string s, int error = 0) override {
        err = error;
        pattern = s;
        pattern_size = pattern.size();
        F.clear();
        for(int i = 0; i < 256; i++)
            delta[i].clear();

        std::vector<int> state;
        for(int i = 0, sze = s.length(); i <= sze; i++) {
            state.push_back(i);
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
            for(int i = 0; i < 256; i++) { 
                auto next_state = make_transition(state, i);
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
            return 1;
        }
        for(unsigned char c: s){
            state = delta[c][state];
            if(F.count(state)){
                return 1;
            }
        }
        state = delta[0][state];
        if(F.count(state)){
            return 1;
        }
        return 0;
    }

    int search(std::string s) override {
        auto cnt = searchLine(s);
        return cnt;
    }
};

struct ShiftOr: Search {

    #define ull unsigned long long

    #define shiftI(m)                           \
        m[0] <<= 1;                             \
        for(int a=1 ; a<size ; a++) {           \
            m[a-1] |= m[a] >> 63;               \
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

    ull** masks = NULL;
    ull test;
    int size;

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
        test = 1ll << (lenS % 64);
        const int size1 = size - 1;
        
        ull mask[size];
        const int buf = size * sizeof(ull);
        memset(mask, -1, buf);
        mask[size1] = -2;

        masks = new ull*[256];
        memset(masks, 0, 256 * sizeof(ull*));

        for(char c : s) {
            ull* aux = masks[c];
            if(aux == NULL) {
                masks[c] = new ull[size];
                aux = masks[c];
                memset(aux, -1, buf);
            }
            andI(aux, mask);
            shiftI(mask);
            mask[size1] |= 1;
        }
    }

    int search(std::string s) override {
        const int buf = size * sizeof(ull);
        
        ull match[size];
        memset(match, -1, buf);
        
        for(char c : s) {
            const ull* mask = masks[c];
            shiftOr(mask, match, buf);
            if(~match[0] & test){
                return 1;
            }
        }
        return 0;
    }
};

struct WuManber: ShiftOr {

    #define clone(a, b)                                         \
        for(int i=0 ; i<size ; i++) {                           \
            a[i] = b[i];                                        \
        }                                                       \

    #define copies(a, b, len)                                   \
        for(int j=0 ; j<len ; j++) {                            \
            clone(a[j], b[j])                                   \
        }                                                       \

    #define compute(c, matchs, olds, temp, buf, dist, dstSize)  \
        matchs[0];                                              \
        copies(olds, matchs, dist);                             \
        const ull* mask = masks[c];                             \
        shiftOr(mask, match, buf);                              \
        for(int q=1 ; q<dstSize ; q++) {                        \
            ull* old = olds[q-1];                               \
            match = matchs[q];                                  \
            shiftOr(mask, match, buf);                          \
            clone(temp, matchs[q-1]);                           \
            shiftI(temp);                                       \
            andI(match, temp);                                  \
            andI(match, old);                                   \
            shiftI(old);                                        \
            andI(match, old);                                   \
        }                                                       \

    int dist;

    void setPattern(std::string s, int err = 0) override {
        ShiftOr::setPattern(s, err);
        dist = err;
    }

    int search(std::string s) override {

        const int size1 = size - 1;
        const int dstSize = dist + 1;
        const int buf = size * sizeof(ull);
        const int clrSize = dstSize * size * sizeof(ull);
        ull matchs[dstSize][size];
        ull olds[dstSize][size];
        ull temp[size];

        memset(matchs, -1, clrSize);
        for(int i=1 ; i<dstSize ; i++) {
            matchs[i][size1] <<= i;
        }
        
        for(char c : s) {
            
            ull* match = compute(c, matchs, olds, temp, buf, dist, dstSize);

            if(~match[0] & test) {
                return 1;
            }
        }

        ull* match = compute('\n', matchs, olds, temp, buf, dist, dstSize);

        if(~match[0] & test) {
            return 1;
        }

        return 0;
    }
};

struct AhoCorasick: Search {
    std::vector<int> fail, word;
    std::vector<std::vector<int> > transition;
    bool ready = true;
    void setPattern(std::string s, int err = 0) override {  
        if (transition.size() == 0) {
            transition.push_back(std::vector<int>(256,-1));
            word.push_back(0);
            fail.push_back(0);
        }
        int node = 0, pos = 0;
        while(true){
            if (pos == s.size()) {
                word[node] = 1;
                break;
            } else {
                unsigned char c = s[pos];
                if (transition[node][c] == -1) {
                    transition[node][c] = transition.size();
                    transition.push_back(std::vector<int>(256,-1));
                    fail.push_back(0);
                    word.push_back(0);
                }
                node = transition[node][c];
                pos++;
            }
        }
        ready = false;
    }
    void prepare(){
        if(ready)return;
        std::queue<int> q;
        for(auto s: transition[0]) 
            if(s != -1)
                q.push(s);
        while(!q.empty()) {
            int s = q.front();
            q.pop();
            for(int c=0;c<256;c++){
                if (transition[s][c] != -1) {
                    int s2 = transition[s][c];
                    q.push(s2);
                    int f = fail[s];
                    while(transition[f][c]!=-1 && f) {
                        f = fail[f];
                    }
                    if(transition[f][c] == -1)
                        fail[s2] = 0;
                    else
                        fail[s2] = transition[f][c];

                }
            }
        }
        
        ready = true;
    }
    int search(std::string s) override {
        int state = 0;
        for(unsigned char c: s) {
            while(state && transition[state][c]==-1)
                state = fail[state];
            if(transition[state][c]!=-1)
                state = transition[state][c];
            if(word[state])
                return 1;
        }
        return 0;
    }
};
