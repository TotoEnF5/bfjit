#ifndef ANALYZER_H_
#define ANALYZER_H_

#include <cstdint>
#include <map>

class Analyzer {
public:
    Analyzer(uint32_t threshold);

    bool analyzeLoop(uint16_t codePointer);

private:
    uint32_t threshold;
    std::map<uint16_t, uint32_t> callCounts;
};

#endif
