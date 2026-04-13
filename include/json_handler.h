#pragma once

#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class JsonHandler {
public:
    JsonHandler() = default;
    bool init(const std::string &str);
    bool add_value(const std::string &key, const std::string &value);
    bool add_value(const std::string &key, int value);
    bool get_value(const std::string &key, std::string &value);
    std::string to_string() const;

private:
    json root_;
};