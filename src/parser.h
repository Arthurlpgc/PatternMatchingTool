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
    std::queue<std::string> file_names;
    std::queue<std::string> used_file_names;
    std::ifstream* current_file = NULL;
    matchOptions opts;
    bool help = false;
    bool time_it = false;

    std::string next_line(){
        if(current_file != NULL && current_file->eof()){
            current_file->close();
            current_file = NULL;
        }
        if(current_file == NULL) {
            current_file = new std::ifstream();
            current_file->open(file_names.front());
            used_file_names.push(file_names.front());
            file_names.pop();
        }
        std::string line;
        getline(*current_file, line);
        return line;
    }

    bool has_next_line(){
        bool ret = false;
        if (!file_names.empty()){
            ret = true;
        } else if (current_file == NULL) {
            ret = false;
        } else ret = !current_file->eof();
        return ret;
    }

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
            file_names.push(file_name);
        }
    }
};