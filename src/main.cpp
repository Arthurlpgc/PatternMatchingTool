#include <iostream>
#include <string>
#include "parser.h"
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
       ShiftOr ukk;
        for(auto pattern: p.patts) {
            ukk.setPattern(pattern, p.opts.edit_distance);
            for(auto file: p.files)
                ukk.search(file);
            if(p.opts.count) {
                std::cout << ukk.count << std::endl;
            }else {
                for (auto answer: ukk.answers)
                    std::cout << answer << std::endl;
            }
        }
    }
    return 0;
}