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

void solvePart1(const Utilities::ProblemInput& input)
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
    std::cout << "Day 1 part 1 solution: " << sumOfCalibrationValues << '\n';
}

void solveProblem(const Utilities::ProblemInputs& inputs, Utilities::ShowResults)
{
    const auto& [input1, _] = inputs;

    solvePart1(input1);
}

} // namespace Day1
