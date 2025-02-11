#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>

namespace ndn::tools
{
void test_me();
void test_me2();
std::vector<std::filesystem::path> ListFilesRecursive(const std::filesystem::path& root, const std::string& filterRegex = ".*");

}
