#include "tools.h"
#include "iostream"
#include <fmt/printf.h>

namespace ndn::tools
{
namespace fs = std::filesystem;

void test_me()
{
    fmt::println("Format lib tested from tools lib: {}", 666);
    std::cout << "Tools lib tesed" << std::endl;
}

std::vector<fs::path> ListFilesRecursive(const fs::path& root, const std::string& filterRegex)
{
    std::vector<fs::path> files;
    std::regex filter(filterRegex); // Създаваме регулярния израз

    try
    {
        for (const auto& entry : fs::recursive_directory_iterator(root))
        {
            if (!fs::is_regular_file(entry))
            {
                continue;
            }
            std::string fileName = entry.path().filename().string();
            if (std::regex_match(fileName, filter))
            {
                files.push_back(entry.path());
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    return files;
}

}
