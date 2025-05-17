#ifndef CACHE_H_
#define CACHE_H_

#include "compiler.h"
#include <map>

/**
 * The cache holds the compiled code and associates it with the code we interpret.
 */
class Cache {
public:
    /**
     * Adds a function in the cache.
     * It will be associated to the loop at <programCounter>.
     */
    void addFunction(uint16_t programCounter, CompiledFunction function);

    /**
     * Returns whether a loop is compiled in the cache or not.
     */
    bool isInCache(uint16_t programCounter);

    /**
     * Returns the function associated with the loop.
     */
    CompiledFunction getFunction(uint16_t programCounter);

private:
    std::map<uint16_t, CompiledFunction> functions;
};

#endif
