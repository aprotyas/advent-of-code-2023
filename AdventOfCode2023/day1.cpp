//
//  day1.cpp
//  AdventOfCode2023
//
//  Created by Abrar Rahman Protyasha on 12/1/23.
//

#include "day1.hpp"
#include "utilities.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <string_view>
#include <variant>

namespace Day1 {

static constexpr uint64_t leftDigitMultiplier = 10;

auto characterIsDigit(unsigned char character) -> bool
{
    return std::isdigit(character) != 0;
}

void solvePart1(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    auto sumOfCalibrationValues = std::accumulate(input.cbegin(), input.cend(), 0, [](uint64_t sumOfCalibrationValues, const auto& line) {
        const auto leftDigit = static_cast<uint64_t>(*std::ranges::find_if(line.cbegin(), line.cend(), characterIsDigit) - '0');
        const auto rightDigit = static_cast<uint64_t>(*std::ranges::find_if(line.crbegin(), line.crend(), characterIsDigit) - '0');
        const uint64_t calibrationValue = leftDigit * leftDigitMultiplier + rightDigit;
        sumOfCalibrationValues += calibrationValue;
        return sumOfCalibrationValues;
    });

    if (showResults == Utilities::ShowResults::Yes)
        std::cout << "Day 1 part 1 solution: " << sumOfCalibrationValues << '\n';
}

enum class IsDigitReason : uint8_t { Unknown, CharacterIsNumeric, SubstringStartingAtCharacterIsDigitWord };
struct IsDigitResult {
    std::variant<bool, std::optional<uint64_t>> isDigit;
    IsDigitReason reason { IsDigitReason::Unknown };
};

namespace {
using namespace std::string_view_literals;
constexpr std::array<std::string_view, 9> digitWords { "one"sv, "two"sv, "three"sv, "four"sv, "five"sv, "six"sv, "seven"sv, "eight"sv, "nine"sv };
}

auto digitRepresentedBySomeSubstringStartingAtCharacter(const std::string_view line, const std::size_t index) -> std::optional<uint64_t>
{
    for (uint64_t wordIndex = 0; wordIndex < digitWords.size(); ++wordIndex) {
        if (line.substr(index).starts_with(digitWords.at(wordIndex)))
            return wordIndex + 1;
    }
    return std::nullopt;
}

enum class TraversalDirection : bool { LTR, RTL };

void solvePart2(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    auto sumOfCalibrationValues = std::accumulate(input.cbegin(), input.cend(), 0, [](uint64_t sumOfCalibrationValues, const auto& line) {
        auto isDigit = [] (unsigned char character, const std::string_view line, const std::size_t index) -> IsDigitResult {
            if (characterIsDigit(character))
                return { true, IsDigitReason::CharacterIsNumeric };
            if (const auto& result = digitRepresentedBySomeSubstringStartingAtCharacter(line, index))
                return { result, IsDigitReason::SubstringStartingAtCharacterIsDigitWord };
            return { };
        };

        const auto produceDigit = [&isDigit] (const auto& line, const TraversalDirection direction) {
            const auto indices = [length = line.length(), &direction] {
                std::vector<uint64_t> indicesToFill;
                indicesToFill.resize(length);
                std::iota(indicesToFill.begin(), indicesToFill.end(), 0);
                if (direction == TraversalDirection::RTL)
                    std::ranges::reverse(indicesToFill);
                return indicesToFill;
            }();

            for (const auto index : indices) {
                const unsigned char character = line[index];
                const auto [isDigitAnswer, isDigitReason] = isDigit(character, line, index);
                switch (isDigitReason) {
                case IsDigitReason::CharacterIsNumeric:
                    return static_cast<uint64_t>(character - '0');
                case IsDigitReason::SubstringStartingAtCharacterIsDigitWord:
                    return *std::get<std::optional<uint64_t>>(isDigitAnswer);
                case IsDigitReason::Unknown:
                    continue;
                }
            }

            return 0ULL;
        };

        const auto leftDigit = produceDigit(line, TraversalDirection::LTR);
        const auto rightDigit = produceDigit(line, TraversalDirection::RTL);

        const uint64_t calibrationValue = leftDigit * leftDigitMultiplier + rightDigit;
        return sumOfCalibrationValues + calibrationValue;
    });

    if (showResults == Utilities::ShowResults::Yes)
        std::cout << "Day 1 part 2 solution: " << sumOfCalibrationValues << '\n';
}

void solveProblem(const Utilities::ProblemInputs& inputs, Utilities::ShowResults showResults)
{
    const auto& [input1, input2] = inputs;

    solvePart1(input1, showResults);
    solvePart2(input2, showResults);
}

} // namespace Day1
