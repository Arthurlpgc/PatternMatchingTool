#include <iostream>
#include <string>
#include "search.h"

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    Parser p;
    p.argc=argc;
    p.argv=argv;
    p.parse();
    if(p.error) { 
        return -1;
    }
    if (p.help) {
        
    } else {
        Search* search;
        if(p.algorithm == "None" || p.algorithm == "Ukkonen"){
                search = new Ukkonen();
        } else if (p.algorithm == "ShiftOr") {
                search = new ShiftOr();
        } else if (p.algorithm == "WuManber") {
                search = new WuManber();
        } else {
            std::cerr << "Invalid algorithm";
            return -1;
        }
        for(auto pattern: p.patts) {
            search->setPattern(pattern, p.edit_distance);
            search->search(&p);
            if(p.count) {
                std::cout << search->count << "\n";
            }
        }
    }
    return 0;
}