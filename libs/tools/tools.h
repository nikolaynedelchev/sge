#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>
#include <memory>

namespace ndn
{
template<typename T>
using Ptr = std::shared_ptr<T>;

template<typename T>
using UPtr = std::unique_ptr<T>;

}
namespace ndn::tools
{
void test_me();
void test_me2();
std::vector<std::filesystem::path> ListFilesRecursive(const std::filesystem::path& root, const std::string& filterRegex = ".*");

}
