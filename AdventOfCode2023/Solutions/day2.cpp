//
//  day2.cpp
//  AdventOfCode2023
//
//  Created by Abrar Rahman Protyasha on 12/3/23.
//

#include "day2.hpp"
#include "utilities.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <string_view>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace Day2 {

using namespace std::string_view_literals;

auto substringStartIndicesForDelimiter(const std::string_view string, const char delimiter, const std::string_view contentAfterDelimiter) -> std::vector<std::size_t>
{
    std::vector<std::size_t> startIndices { 0 };
    std::size_t startIndex = startIndices.front();
    while ((startIndex = string.find(delimiter, startIndex)) != std::string_view::npos) {
        startIndex += contentAfterDelimiter.length() + 1;
        startIndices.push_back(startIndex);
    }
    startIndices.push_back(string.length());
    return startIndices;
}

struct Game {
    struct Cubes {
        enum class Colors : uint8_t { Red, Green, Blue };

        uint64_t red { 0 };
        uint64_t green { 0 };
        uint64_t blue { 0 };
        
        static auto createCubes(const std::string_view line) -> std::vector<Cubes>
        {
            const auto cubesInfo { line.substr(line.find(Game::gameInfoDelimiter) + 2) };
            const auto cubeStartIndices = substringStartIndicesForDelimiter(cubesInfo, cubeDelimiter, " "sv);

            std::vector<std::string_view> cubeStrings;
            for (std::size_t index = 0; index < cubeStartIndices.size() - 1; ++index) {
                const auto startIndex = cubeStartIndices[index];
                const auto endIndex = cubeStartIndices[index + 1];
                cubeStrings.push_back(cubesInfo.substr(startIndex, endIndex - startIndex - (index == cubeStartIndices.size() - 2 ? 0 : 2)));
            }

            std::vector<Cubes> cubes;
            std::ranges::transform(cubeStrings, std::back_inserter(cubes), [] (const auto cubeString) -> Cubes {
                const auto colorStartIndices = substringStartIndicesForDelimiter(cubeString, colorDelimiter, " "sv);
                Cubes cubes { };
                for (std::size_t index = 0; index < colorStartIndices.size() - 1; ++index) {
                    const auto startIndex = colorStartIndices[index];
                    const auto endIndex = colorStartIndices[index + 1];
                    const auto colorString = cubeString.substr(startIndex, endIndex - startIndex - (index == colorStartIndices.size() - 2 ? 0 : 2));
                    const auto spaceIndex = colorString.find(' ');

                    const auto quantity = std::stoul(std::string { colorString.substr(0, spaceIndex) });
                    const auto color = [] (const auto colorString) {
                        if (colorString == "red"sv)
                            return Colors::Red;
                        if (colorString == "green"sv)
                            return Colors::Green;
                        if (colorString == "blue"sv)
                            return Colors::Blue;
                        std::unreachable();
                    }(colorString.substr(spaceIndex + 1));

                    switch (color) {
                    case Colors::Red:
                        cubes.red = quantity;
                        break;
                    case Colors::Green:
                        cubes.green = quantity;
                        break;
                    case Colors::Blue:
                        cubes.blue = quantity;
                        break;
                    default:
                        std::unreachable();
                    }
                }
                return cubes;
            });

            return cubes;
        }

        static constexpr char colorDelimiter { ',' };
        static constexpr char cubeDelimiter { ';' };

        auto operator>(const Cubes& rhs) const -> bool
        {
            return red > rhs.red || green > rhs.green || blue > rhs.blue;
        }
    };

    std::vector<std::vector<Cubes>> rounds;

    Game(const Utilities::ProblemInput& input)
    {
        std::ranges::transform(input, std::back_inserter(rounds), Cubes::createCubes);
    }

    static constexpr char gameInfoDelimiter { ':' };
    static constexpr Cubes maxCubeInfo { 12, 13, 14 };
};

auto solvePart1(const Utilities::ProblemInput& input) -> uint64_t
{
    const Game game { input };

    std::size_t sumOfPossibleGameIds = 0;
    for (std::size_t index = 0; index < game.rounds.size(); ++index) {
        bool gameIsPossible = true;
        for (const auto& cubes : game.rounds[index]) {
            if (cubes > Game::maxCubeInfo) {
                gameIsPossible = false;
                break;
            }
        }
        sumOfPossibleGameIds += gameIsPossible ? index + 1 : 0;
    }

    return sumOfPossibleGameIds;
}

auto solvePart2(const Utilities::ProblemInput& input) -> uint64_t
{
    const Game game { input };

    uint64_t sumOfSetPowers = 0;
    std::ranges::for_each(game.rounds, [&sumOfSetPowers] (const std::vector<Game::Cubes>& round) {
        Game::Cubes minCubeInfo = round.front();
        std::ranges::for_each(round, [&minCubeInfo] (const Game::Cubes& cube) {
            minCubeInfo.red = std::max(minCubeInfo.red, cube.red);
            minCubeInfo.green = std::max(minCubeInfo.green, cube.green);
            minCubeInfo.blue = std::max(minCubeInfo.blue, cube.blue);
        });
        sumOfSetPowers += minCubeInfo.red * minCubeInfo.green * minCubeInfo.blue;
    });

    return sumOfSetPowers;
}

void solveProblem(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    const auto part1Solution = solvePart1(input);
    const auto part2Solution = solvePart2(input);

    if (showResults == Utilities::ShowResults::Yes)
        Utilities::displayAnswers(2, part1Solution, part2Solution);
}

} // namespace Day2
