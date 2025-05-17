#ifndef ANALYZER_H_
#define ANALYZER_H_

#include <cstdint>
#include <map>

/**
 * The code analyzer.
 * For this project, it is really simple: just a class to counts the number of loop "calls".
 * For other project it's more complex I guess.
 */
class Analyzer {
public:
    /**
     * Initializes the analyzer.
     * If a loop is analyzed <threshold> or more times, it's time to compile it!
     */
    Analyzer(uint32_t threshold);

    /**
     * Analyze a loop.
     * Returns true if it must be compiled.
     */
    bool analyzeLoop(uint16_t codePointer);

private:
    uint32_t threshold;
    std::map<uint16_t, uint32_t> callCounts;
};

#endif
