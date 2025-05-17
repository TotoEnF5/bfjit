#include "analyzer.h"

Analyzer::Analyzer(uint32_t threshold)
    : threshold(threshold)
{}

bool Analyzer::analyzeLoop(uint16_t codePointer) {
    // for some reason, c++ introduces map::contains only in c++20
    // but for some other reason, calling the [] operator with a key that does not exist in the map
    // creates the pair and assigns it the default value.
    // like, checking manually if the key exists and throwing an exception when accessing a key that does not exist
    // make more sense to me
    // but c++ is weird i guess
    uint32_t callCount = ++this->callCounts[codePointer];
    return callCount > this->threshold;
}
