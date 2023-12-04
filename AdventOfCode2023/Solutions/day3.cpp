//
//  day3.cpp
//  AdventOfCode2023
//
//  Created by Abrar Rahman Protyasha on 12/4/23.
//

#include "day3.hpp"
#include "utilities.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <compare>
#include <numeric>
#include <optional>
#include <vector>

namespace Day3 {

struct Position {
    // Origin is at top-left, with +x and +y pointing right and down respectively.
    int x { 0 };  // column
    int y { 0 };  // row

    auto operator<=>(const Position&) const = default;
};

struct Number {
    uint64_t value { 0 };
    Position left;
    Position right;
};

struct Gear {
    Position position;
    std::array<Number, 2> adjacentNumbers;
};

auto generateAdjacencyRange(const auto& number) -> std::vector<Position>
{
    std::vector<int> xRange;
    xRange.resize(number.right.x - number.left.x + 3);
    std::iota(xRange.begin(), xRange.end(), number.left.x - 1);

    std::vector<int> yRange;
    yRange.resize(3);
    std::iota(yRange.begin(), yRange.end(), number.left.y - 1);

    std::vector<Position> adjacencyRange;
    // This should really be std::ranges::views::cartesian_product, but clang-1500.3.3.4 doesn't implement it.
    for (const auto xVal : xRange) {
        for (const auto yVal : yRange)
            adjacencyRange.push_back({ xVal, yVal });
    }

    return adjacencyRange;
}

auto solvePart1(const auto& symbolPositions, const auto& numbers) -> uint64_t
{
    const auto isPartNumber = [&symbolPositions] (const auto& number) -> bool {
        return std::ranges::any_of(symbolPositions, [adjacencyRange = generateAdjacencyRange(number)] (const auto& symbolPosition) -> bool {
            return std::ranges::find(adjacencyRange, symbolPosition) != adjacencyRange.cend();
        });
    };

    return std::accumulate(numbers.cbegin(), numbers.cend(), 0, [&isPartNumber, &symbolPositions](uint64_t sumOfPartNumbers, const auto& number) {
        return sumOfPartNumbers + (isPartNumber(number) ? number.value : 0);
    });
}

auto solvePart2(const auto& potentialGearPositions, const auto& numbers) -> uint64_t
{
    const auto makeGearIfPossible = [&numbers] (const auto& potentialGearPosition) -> std::optional<Gear> {
        std::size_t adjacencyCount = 0;
        std::array<Number, 2> adjacentNumbers;
        for (const auto& number : numbers) {
            const auto adjacencyRange = generateAdjacencyRange(number);
            if (std::ranges::find(adjacencyRange, potentialGearPosition) != adjacencyRange.cend()) {
                adjacentNumbers.at(adjacencyCount++) = number;
            }
            if (adjacencyCount > 2)
                break;
        }
        if (adjacencyCount != 2)
            return std::nullopt;

        return Gear{ potentialGearPosition, adjacentNumbers };
    };
    
    return std::accumulate(potentialGearPositions.cbegin(), potentialGearPositions.cend(), 0, [&makeGearIfPossible](uint64_t sumOfGearRatios, const auto& gearPosition) {
        if (const auto gear = makeGearIfPossible(gearPosition))
            sumOfGearRatios += gear->adjacentNumbers[0].value * gear->adjacentNumbers[1].value;
        return sumOfGearRatios;
    });
}

void solveProblem(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    std::vector<Position> symbolPositions;
    std::vector<Position> potentialGearPositions;
    std::vector<Number> numbers;

    for (int row { 0 }; row < input.size(); ++row) {
        for (int col { 0 }; col < input.at(row).size(); ++col) {
            if (const unsigned char character = input.at(row).at(col); std::ispunct(character) != 0) {
                if (character != '.')
                    symbolPositions.push_back({ col, row });
                if (character == '*')
                    potentialGearPositions.push_back({ col, row });
            }
            else if (std::isdigit(character) != 0) {
                // Find the nearest symbol (or dot, or EOL), and record the digit.
                int iterCol = col + 1;
                for (; iterCol < input.at(row).size() && std::ispunct(input.at(row).at(iterCol)) == 0; ++iterCol);
                const Position left { col, row };
                const Position right { iterCol - 1, row };
                // clang-1500.3.3.4 doesn't like emplace_back here...
                numbers.push_back({ std::stoull(input.at(row).substr(col, iterCol - col)), left, right });
                col = iterCol - 1;
            }
        }
    }

    const auto part1Solution = solvePart1(symbolPositions, numbers);
    const auto part2Solution = solvePart2(potentialGearPositions, numbers);

    if (showResults == Utilities::ShowResults::Yes)
        Utilities::displayAnswers(3, part1Solution, part2Solution);
}

} // namespace Day3
