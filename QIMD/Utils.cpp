#include "Utils.h"

std::vector<std::string> Utils::SplitStringToVector(std::string input)
{
    std::vector<std::string> container;
    std::istringstream iss(input);
    std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        std::back_inserter(container));

    return container;
}
