/** @file
@brief

Copyright (C) 2020 ifm electronic GmbH, deabtma
See accompanied file licence.txt for license information.
*/
#ifndef VALIDATORS_HPP
#define VALIDATORS_HPP

#include <filesystem>

#if __has_include(<cli11/CLI11.hpp>)
#include <cli11/CLI11.hpp>
#elif __has_include(<CLI/CLI.hpp>)
#include <CLI/CLI.hpp>
#endif

#include <regex>


namespace detail {

class PathExistsValidator : public CLI::Validator
// checks whether the given path is reachable
{
public:
    PathExistsValidator()
    : Validator("DIR")
    {
        func_ = [](std::string& filePath) {
            std::filesystem::path path(filePath);
            if (path.has_parent_path() && !std::filesystem::exists(path.remove_filename()))
            {
                return "Path does not exist: " + path.remove_filename().string();
            }
            return std::string();
        };
    }
};

class MacValidator : public CLI::Validator
// checks whether the given path is reachable
{
public:
    MacValidator()
    : Validator("MAC")
    {
        func_ = [](std::string& mac) {
            std::regex rx(
                R"(^(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])([0-9A-Fa-f]{2})$)",
                std::regex::icase);
            std::smatch match;

            if (!std::regex_match(mac, match, rx))
            {
                return "Mac is not valid: " + mac;
            }
            return std::string();
        };
    }
};

}  // detail

const detail::PathExistsValidator ExistingPath;
const detail::MacValidator ValidMac;

#endif  // VALIDATORS_HPP
