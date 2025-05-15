#include "interpreter.h"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
	std::cerr << "no file provided" << std::endl;
	return 1;
    }

    std::ifstream file(argv[1]);
    std::ostringstream sstr;
    sstr << file.rdbuf();
    file.close();

    Interpreter interpreter(sstr.str());
    interpreter.run();

    return 0;
}
