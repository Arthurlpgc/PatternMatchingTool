#include <getopt.h>
#include <string>

struct matchOptions {
    bool count = false;
    int edit_distance = 0;
    std::string algorithm = "None";
    std::string pattern_file_path = "";
};

const char* const short_opts = "tce:p:a:h";
const option long_opts[] = {
    {"count", no_argument, nullptr, 'c'},
    {"edit", required_argument, nullptr, 'e'},
    {"pattern", required_argument, nullptr, 'p'},
    {"algorithm", required_argument, nullptr, 'a'},
    {"time", no_argument, nullptr, 't'},
};

struct Parser {
    int argc;
    char** argv;
    matchOptions opts;
    bool help = false;
    bool time_it = false;

    void parse() {
        int opt = 0;
        
        while(opt != -1) {
            opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
            switch(opt) {
                case 'c':
                    opts.count = true;
                    break;
                case 'e':
                    opts.edit_distance = std::stoi(optarg);
                    break;
                case 'p':
                    opts.pattern_file_path = std::string(optarg);
                    break;
                case 'a':
                    opts.algorithm = std::string(optarg);
                    break;
                case 't':
                    time_it = true;
                    break;
                default:
                    help = true;
            }
        }
    }
};