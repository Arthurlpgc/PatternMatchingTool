#include "search.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <list>

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    Parser parser(argc, argv);

    if (parser.help) {
        std::cout << "Usage: pmt [options] [pattern] file [file...]\n\nOptions:\n";
        std::cout << "  -a, --algorithm ALGORITHM     Sets the algorithm to be used(can be Ukkonen, AhoCorasik, ShiftOr or WuMamber)\n";
        std::cout << "  -c, --count                   Displays the number of lines each pattern occurs\n";
        std::cout << "  -e, --edit DISTANCE           Tries to find approximate matches with at most the edit distance given, if the algorithm doesnt support, it will be ignored\n";
        std::cout << "  -h, --help                    Shows this guide\n";
        std::cout << "  -p, --pattern PATTERN_FILE    Grabs the patterns given in a file, when this option is passed, the pattern parameter should not be passed.\n";
    } else if(parser.error == 1) {
        std::cerr << "Missing pattern" << std::endl;
        return -1;
    } else if(parser.error == 2) {
        std::cerr << "Pattern file not found" << std::endl;
        return -1;
    } else if(parser.error == 3) {
        std::cerr << "Edit distance error should be less than the pattern size" << std::endl;
        return -1;
    }  else {
        std::list<Search*> searchs;
        if (parser.algorithm == "AhoCorasik") {
            Search* search = new AhoCorasik();
            for(auto pattern: parser.patts) {
                search->setPattern(pattern, parser.edit_distance);
            }
            searchs.push_back(search);
        } else{
            for(auto pattern: parser.patts) {
                Search* search;
                if(parser.algorithm == "None" || parser.algorithm == "Ukkonen"){
                    search = new Ukkonen();
                } else if (parser.algorithm == "AhoCorasik") {
                    search = new AhoCorasik();
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