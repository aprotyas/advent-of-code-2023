//
//  utilities.hpp
//  AdventOfCode2023
//
//  Created by Abrar Rahman Protyasha on 12/1/23.
//

#ifndef utilities_hpp
#define utilities_hpp

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace Utilities {

enum class ShowResults : bool { No, Yes };

using Day = std::size_t;

using ProblemInput = std::vector<std::string>;
using ProblemSolver = std::function<void(const ProblemInput&, ShowResults)>;

void displayAnswers(Day day, const auto& part1Solution, const auto& part2Solution)
{
    std::cout << "Day " << day << " Part 1: " << part1Solution << std::endl;
    std::cout << "Day " << day << " Part 2: " << part2Solution << std::endl;
}

} // namespace Utilities

#endif /* utilities_hpp */
