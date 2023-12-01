//
//  main.cpp
//  AdventOfCode2023
//
//  Created by Abrar Rahman Protyasha on 12/1/23.
//

#include "solutions.hpp"
#include "utilities.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

using Day = std::size_t;
using namespace std::string_view_literals;

bool isCaseInsensitiveCharEqual(const char a, const char b)
{
    return std::tolower(static_cast<unsigned char>(a)) ==
           std::tolower(static_cast<unsigned char>(b));
}

bool isCaseInsensitiveEqual(std::string_view lhs, std::string_view rhs)
{
    return std::ranges::equal(lhs, rhs, isCaseInsensitiveCharEqual);
}

// Lifted from https://stackoverflow.com/a/26221725.
template<typename ... Args>
std::string stringFormat(const std::string& format, Args ... args )
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'.
    if (size_s <= 0)
        throw std::runtime_error("Error during formatting.");
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside.
}

Utilities::ProblemInput inputFromPath(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path)) {
        std::cerr << "Problem input at path " << path << " does not exist.\n";
        return { };
    }

    if (!std::filesystem::is_regular_file(path)) {
        std::cerr << "Problem input at path " << path << " is not a regular file.\n";
        return { };
    }

    std::ifstream inputFile { path };
    Utilities::ProblemInput input;
    for (std::string line; std::getline(inputFile, line);)
        input.push_back(line);

    return input;
}

int solveProblem(Day day, Utilities::ShowResults showResults)
{
    // This should be constexpr, but I'm too lazy to adopt https://github.com/serge-sans-paille/frozen.
    static std::unordered_map<Day, Utilities::ProblemSolver> problemSolvers {
        { 1, Day1::solveProblem },
    };

    if (!problemSolvers.contains(day)) {
        std::cerr << "Day " << day << " not solved yet!\n";
        return EXIT_FAILURE;
    }

    std::cout << "Acquiring solution inputs for " << day << '\n';
    const auto inputFileName = [&day] (int part) { return stringFormat("day%dpart%d.in", day, part); };
    const auto inputFilePath = [] (const std::string& name) { return std::filesystem::path(__FILE__).parent_path() / "Inputs" / name; };
    const auto inputFileNames = std::make_pair(inputFileName(1), inputFileName(2));
    const auto inputFilePaths = std::make_pair(inputFilePath(inputFileNames.first), inputFilePath(inputFileNames.second));
    const auto inputs = std::make_pair(inputFromPath(inputFilePaths.first), inputFromPath(inputFilePaths.second));

    std::cout << "Solving day " << day << '\n';
    problemSolvers[day](inputs, showResults);
    return EXIT_SUCCESS;
}

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        std::cerr << "Wrong arguments provided. Either pass the <day> you'd like to see solved, or pass `all` for all solutions.\n";
        return EXIT_FAILURE;
    }

    std::optional<Day> dayToSolve;
    const bool solveAll = isCaseInsensitiveEqual(argv[1], "all"sv);
    if (solveAll)
        std::cout << "Solving all problems!\n";
    else {
        auto maybeDayToSolve = std::atoi(argv[1]);
        if (maybeDayToSolve)
            dayToSolve = maybeDayToSolve;
        if (!dayToSolve) {
            std::cerr << "Invalid day provided!\n";
            return EXIT_FAILURE;
        }
    }

    int returnValue = EXIT_SUCCESS;
    constexpr Day daysSolved = 1;
    std::ranges::for_each(std::views::iota(solveAll ? 1 : *dayToSolve) | std::views::take(solveAll ? daysSolved : *dayToSolve), [&returnValue] (Day day) {
        returnValue &= solveProblem(day, Utilities::ShowResults::Yes);
    });

    return returnValue;
}
