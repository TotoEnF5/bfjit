#include "compiler.h"
#include "interpreter.h"
#include <cstdint>
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

    /*
    uint8_t memory[0x10000] = { 0 };
    Compiler compiler(memory);
    CompiledFunction func = compiler.compile(sstr.str());
    func(0);
    */

    return 0;
}
