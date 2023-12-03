//
//  day1.cpp
//  AdventOfCode2023
//
//  Created by Abrar Rahman Protyasha on 12/1/23.
//

#include "day1.hpp"
#include "utilities.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>

namespace Day1 {

void solvePart1(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    auto sumOfCalibrationValues = std::accumulate(input.cbegin(), input.cend(), 0, [](uint64_t sumOfCalibrationValues, const auto& line) {
        const auto isDigit = [] (unsigned char character) {
            return std::isdigit(character);
        };
        constexpr uint64_t leftDigitMultiplier = 10;
        const auto leftDigit = static_cast<uint64_t>(*std::ranges::find_if(line.cbegin(), line.cend(), isDigit) - '0');
        const auto rightDigit = static_cast<uint64_t>(*std::ranges::find_if(line.crbegin(), line.crend(), isDigit) - '0');
        const auto calibrationValue = leftDigit * leftDigitMultiplier + rightDigit;
        sumOfCalibrationValues += calibrationValue;
        return sumOfCalibrationValues;
    });

    if (showResults == Utilities::ShowResults::Yes)
        std::cout << "Day 1 part 1 solution: " << sumOfCalibrationValues << '\n';
}

void solvePart2(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{

}

void solveProblem(const Utilities::ProblemInputs& inputs, Utilities::ShowResults showResults)
{
    const auto& [input1, input2] = inputs;

    solvePart1(input1, showResults);
    solvePart2(input2, showResults);
}

} // namespace Day1
