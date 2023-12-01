//
//  utilities.hpp
//  AdventOfCode2023
//
//  Created by Abrar Rahman Protyasha on 12/1/23.
//

#ifndef utilities_hpp
#define utilities_hpp

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace Utilities {

enum class ShowResults : bool { No, Yes };

using ProblemInput = std::vector<std::string>;
using ProblemInputs = std::pair<ProblemInput, ProblemInput>;
using ProblemSolver = std::function<void(const ProblemInputs&, ShowResults)>;

} // namespace Utilities

#endif /* utilities_hpp */
