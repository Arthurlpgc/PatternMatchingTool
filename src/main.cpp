#include <iostream>
#include "parser.h"
#include "search.h"

int main(int argc, char* argv[]) {
    Parser p;
    p.argc=argc;
    p.argv=argv;
    p.parse();
    if (p.help) {

    } else {

    }
    return 0;
}