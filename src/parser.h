#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>

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
    int error = 0;
    std::vector<std::string> patts;
    std::vector<std::string> file_names;
    std::vector<std::vector<std::string> > files;
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
                    // TODO
                    break;
                case 'a':
                    opts.algorithm = std::string(optarg);
                    break;
                case 't':
                    time_it = true;
                    break;
                case -1:
                    break;
                default:
                    help = true;
            }
        }

        int ind = optind;
        if (opts.pattern_file_path != ""){

        } else if (ind != argc) {
            patts.push_back(std::string(argv[ind]));
            ind++;
        } else {
            std::cerr << "missing pattern" << std::endl;
            error = -1;
            return;
        }
        while(ind < argc) {
            std::string file_name = std::string(argv[ind++]);
            std::vector<std::string> file;
            std::ifstream fin;
            fin.open(file_name);
            std::string line;
            while(getline(fin, line)) {
                file.push_back(line);
            }
            files.push_back(file);
            fin.close();
        }
    }
};