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

auto characterIsDigit(unsigned char character) -> bool
{
    return std::isdigit(character) != 0;
}

enum class TraversalDirection : bool { LTR, RTL };

enum class IsDigitReason : uint8_t { Unknown, CharacterIsNumeric, SubstringStartingAtCharacterIsDigitWord };
struct IsDigitResult {
    std::variant<bool, std::optional<uint64_t>> isDigit;
    IsDigitReason reason { IsDigitReason::Unknown };
};

namespace {
using namespace std::string_view_literals;
constexpr std::array<std::string_view, 9> digitWords { "one"sv, "two"sv, "three"sv, "four"sv, "five"sv, "six"sv, "seven"sv, "eight"sv, "nine"sv };
}

auto produceDigit(const std::string_view line, const std::function<IsDigitResult(const unsigned char, const std::string_view, const std::size_t)>& isDigit, const TraversalDirection direction) -> uint64_t {
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
}

auto calibrationValueForLine(const std::string_view line, const std::function<IsDigitResult(const unsigned char, const std::string_view, const std::size_t)>& isDigit) -> uint64_t
{
    static constexpr uint64_t leftDigitMultiplier = 10;
    const auto leftDigit = produceDigit(line, isDigit, TraversalDirection::LTR);
    const auto rightDigit = produceDigit(line, isDigit, TraversalDirection::RTL);
    return leftDigit * leftDigitMultiplier + rightDigit;
}

void solvePart1(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    auto sumOfCalibrationValues = std::accumulate(input.cbegin(), input.cend(), 0, [](uint64_t sumOfCalibrationValues, const auto& line) {
        const auto isDigit = [] (const unsigned char character, const std::string_view, const std::size_t) -> IsDigitResult {
            if (characterIsDigit(character))
                return { true, IsDigitReason::CharacterIsNumeric };
            return { };
        };
        return sumOfCalibrationValues + calibrationValueForLine(line, isDigit);
    });

    if (showResults == Utilities::ShowResults::Yes)
        std::cout << "Day 1 part 1 solution: " << sumOfCalibrationValues << '\n';
}

auto digitRepresentedBySomeSubstringStartingAtCharacter(const std::string_view line, const std::size_t index) -> std::optional<uint64_t>
{
    for (uint64_t wordIndex = 0; wordIndex < digitWords.size(); ++wordIndex) {
        if (line.substr(index).starts_with(digitWords.at(wordIndex)))
            return wordIndex + 1;
    }
    return std::nullopt;
}

void solvePart2(const Utilities::ProblemInput& input, Utilities::ShowResults showResults)
{
    auto sumOfCalibrationValues = std::accumulate(input.cbegin(), input.cend(), 0, [](uint64_t sumOfCalibrationValues, const auto& line) {
        auto isDigit = [] (const unsigned char character, const std::string_view line, const std::size_t index) -> IsDigitResult {
            if (characterIsDigit(character))
                return { true, IsDigitReason::CharacterIsNumeric };
            if (const auto& result = digitRepresentedBySomeSubstringStartingAtCharacter(line, index))
                return { result, IsDigitReason::SubstringStartingAtCharacterIsDigitWord };
            return { };
        };

        return sumOfCalibrationValues + calibrationValueForLine(line, isDigit);
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
