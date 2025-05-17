#include "cache.h"

void Cache::addFunction(uint16_t programCounter, CompiledFunction function) {
    this->functions[programCounter] = function;
}

bool Cache::isInCache(uint16_t programCounter) {
    // another c++ l
    // (map::contains does not exist until c++20)
    return this->functions.find(programCounter) != this->functions.end();
}

CompiledFunction Cache::getFunction(uint16_t programCounter) {
    return this->functions[programCounter];
}
