#include <getopt.h>
#include <string>
#include <fstream>
#include "tracer.h"
#include <vector>

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
    Tracer* tracer;
    char** argv;
    int error = 0;
    std::vector<std::string> patts;
    std::queue<std::string> file_names;
    std::queue<std::string> used_file_names;
    std::ifstream* current_file = NULL;
    bool count = false;
    int edit_distance = 0;
    std::string algorithm = "None";
    std::string pattern_file_path = "";
    bool help = false;
    bool time_it = false;
    std::string line;

    inline std::string next_line(){
#ifdef tracerrun
        tracer->start(7);
#endif
        if(current_file != NULL && current_file->eof()){
            current_file->close();
            current_file = NULL;
        }
        if(current_file == NULL) {
            current_file = new std::ifstream();
            current_file->open(file_names.front());
            used_file_names.push(file_names.front());
            file_names.pop();
            if(!current_file->is_open()){
                current_file = NULL;
                return "\n";
            }
        }
        getline(*current_file, line);
#ifdef tracerrun
        tracer->start(1);
#endif
        return line;
    }

    inline bool has_next_line(){
#ifdef tracerrun
        tracer->start(8);
#endif
        if (!file_names.empty()){
            return true;
        }
#ifdef tracerrun
        tracer->start(1);
#endif
        return  current_file!= NULL && !current_file->eof();
    }

    inline void parse(bool first_load=true) {
        tracer->start(0);

        if(first_load) {
            int opt = 0;
            
            while(opt != -1) {
                opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
                switch(opt) {
                    case 'c':
                        count = true;
                        break;
                    case 'e':
                        edit_distance = std::stoi(optarg);
                        break;
                    case 'p':
                        pattern_file_path = std::string(optarg);
                        // TODO
                        break;
                    case 'a':
                        algorithm = std::string(optarg);
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
            if (pattern_file_path != ""){
                std::string pattern;
                std::ifstream* pattern_file = new std::ifstream();
                pattern_file->open(pattern_file_path);
                while(pattern_file->is_open() && !pattern_file->eof()) {
                    getline(*pattern_file, pattern);
                    patts.push_back(pattern);
                }
                pattern_file->close();
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
        } else {
            int ind = optind;
            if(pattern_file_path == "" && ind != argc) ind++;
            while(ind < argc) {
                std::string file_name = std::string(argv[ind++]);
                file_names.push(file_name);
            }
        }
        tracer->start(9);
    }
};