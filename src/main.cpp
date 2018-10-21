#include "search.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <list>

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    Parser parser(argc, argv);

    if(parser.error == 1) {
        std::cerr << "Missing pattern" << std::endl;
        return -1;
    } else if(parser.error == 2) {
        std::cerr << "Pattern file not found" << std::endl;
        return -1;
    } else if(parser.error == 3) {
        std::cerr << "Edit distance error should be less than the pattern size" << std::endl;
        return -1;
    } else if (parser.help) {
        //TODO
    } else {
        std::list<Search*> searchs;
        for(auto pattern: parser.patts) {
            Search* search;
            if(parser.algorithm == "None" || parser.algorithm == "Ukkonen"){
                search = new Ukkonen();
            } else if (parser.algorithm == "WuManber") {
                search = new WuManber();
            } else if (parser.algorithm == "ShiftOr") {
                search = new ShiftOr();
            } else {
                std::cerr << "Invalid algorithm" << std::endl;
                return -1;
            }
            search->setPattern(pattern, parser.edit_distance);
            searchs.push_back(search);
        }

        while(parser.iterator()) {
            int total = 0;
            while(parser.has_next()) {
                std::string s = parser.next();
                for(Search* search : searchs) {
                    if(search->search(s)) {
                        total++;
                        if(parser.line) {
                            std::cout << parser.filename << s << std::endl;
                        }
                        break;
                    }
                }
            }
            if(!parser.line) {
                std::cout << parser.filename << total << std::endl;
            }
        }
    }
    return 0;
}