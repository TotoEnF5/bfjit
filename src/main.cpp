#include "argParser.h"
#include "compiler.h"
#include "interpreter.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    ArgParser parser(argc, argv);
    
    const std::string& filename = parser.getOption("-f");
    if (filename.empty()) {
        std::cerr << "no file provided! please provided a file with the `-f` option." << std::endl;
        return 1;
    }
    
    std::ifstream file(filename);
    if (!file.good()) {
        std::cerr << "failed to open " << filename << "!" << std::endl;
        return 2;
    }
    
    std::ostringstream sstr;
    sstr << file.rdbuf();
    file.close();
    
    std::string code = sstr.str();
    
    if (parser.optionExists("--compiled")) {
        uint8_t memory[0x10000] = { 0 };
        Compiler compiler(memory);
        CompiledFunction func = compiler.compile(code);
        func(0);
    }
    else if (parser.optionExists("--interpreted")) {
        Interpreter interpreter(code, false);
        interpreter.run();
    }
    else {
        Interpreter interpreter(code);
        interpreter.run();
    }
    
    return 0;
}
