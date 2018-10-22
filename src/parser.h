#include <getopt.h>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

const char* const short_opts = "tce:p:a:h";
const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'},
    {"count", no_argument, nullptr, 'c'},
    {"edit", required_argument, nullptr, 'e'},
    {"pattern", required_argument, nullptr, 'p'},
    {"algorithm", required_argument, nullptr, 'a'},
};

struct Parser {
    std::queue<std::string> filenames;
    std::string pattern_file_path = "";
    std::vector<std::string> patts;
    std::string algorithm = "None";
    std::ifstream* file = NULL;
    std::string filename = "";
    int edit_distance = 0;
    std::string buffer;
    bool help = false;
    bool line = true;
    bool dist = false;
    int error = 0;
    
    Parser(int argc, char** argv) {
        int opt = 0;
            
        while(opt != -1) {
            opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
            switch(opt) {
                case 'c':
                    line = false;
                    break;
                case 'e':
                    edit_distance = std::stoi(optarg);
                    dist = true;
                    break;
                case 'p':
                    pattern_file_path = std::string(optarg);
                    break;
                case 'a':
                    algorithm = std::string(optarg);
                    break;
                case -1:
                    break;
                default:
                    help = true;
                    return;
            }
        }

        if (argc <= 1) {
            help = true;
            return;
        }

        int ind = optind;
        if (pattern_file_path != "") {
            std::string pattern;
            std::ifstream* pattern_file = new std::ifstream();
            pattern_file->open(pattern_file_path);
            if(!pattern_file->is_open()){
                error = 2;
                return;
            }
            while(!pattern_file->eof()) {
                getline(*pattern_file, pattern);
                patts.push_back(pattern);
                if(dist && pattern.length() <= edit_distance) {
                    error = 3;
                    return;
                }
            }
            pattern_file->close();
        } else if (ind != argc) {
            std::string pattern = std::string(argv[ind++]);
            patts.push_back(pattern);
            if(dist && pattern.length() <= edit_distance) {
                error = 3;
                return;
            }
        } else {
            error = 1;
            return;
        }
        while(ind < argc) {
            std::string file_name = std::string(argv[ind++]);
            filenames.push(file_name);
        }
        if(filenames.size() > 1) {
            filename = filenames.front();
        }
    }

    inline bool iterator() {
        if(filenames.empty()){
            return false;
        }
        if(file != NULL) {
            file->close();
            delete file;
        }
        if(filename != "") {
            filename = filenames.front() + ": ";
        }
        file = new std::ifstream();
        file->open(filenames.front());
        if(!file->is_open())file = NULL;
        filenames.pop();
        return true;
    }

    inline std::string next() {
        if(file == NULL) {
            return "\n";
        }
        getline(*file, buffer);
        return buffer;
    }

    inline bool has_next() {
        return file != NULL && !file->eof();
    }
};
