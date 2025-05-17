#ifndef CACHE_H_
#define CACHE_H_

#include "compiler.h"
#include <map>

class Cache {
public:
    void addFunction(uint16_t programCounter, CompiledFunction function);
    bool isInCache(uint16_t programCounter);
    CompiledFunction getFunction(uint16_t programCounter);

private:
    std::map<uint16_t, CompiledFunction> functions;
};

#endif
