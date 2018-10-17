#include <iostream>
#include <string>
#include "search.h"

int main(int argc, char* argv[]) {
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
        if(p.opts.algorithm == "None" || p.opts.algorithm == "Ukkonen"){
                search = new Ukkonen();
        } else if (p.opts.algorithm == "ShiftOr") {
                search = new ShiftOr();
        } else {
            std::cerr << "Invalid algorithm";
            return -1;
        }
        for(auto pattern: p.patts) {
            search->setPattern(pattern, p.opts.edit_distance);
            search->search(&p);
            if(p.opts.count) {
                std::cout << search->count << std::endl;
            }
        }
    }
    return 0;
}