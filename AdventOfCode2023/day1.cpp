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
#include <ranges>

namespace Day1 {

void solvePart1(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    uint64_t sumOfCalibrationValues = 0;
    std::ranges::for_each(input.cbegin(), input.cend(), [&sumOfCalibrationValues](const auto& line) {
        const auto isDigit = [] (unsigned char ch) {
            return std::isdigit(ch);
        };
        uint64_t calibrationValue = static_cast<uint64_t>(*std::ranges::find_if(line.cbegin(), line.cend(), isDigit) - '0') * 10;
        calibrationValue += static_cast<uint64_t>(*std::ranges::find_if(line.crbegin(), line.crend(), isDigit) - '0');
        sumOfCalibrationValues += calibrationValue;
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
