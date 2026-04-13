#include "logger.h"
#include <exception>
#include <json_handler.h>

bool JsonHandler::init(const std::string &str)
{
    try {
        root_ = json::parse(str);
        return true;
    } catch (const std::exception &e) {
        LOG_ERROR("json parse error: {}", e.what());
        return false;
    }
}
bool JsonHandler::add_value(const std::string &key, const std::string &value)
{
    if (!root_.is_object()) {
        return false;
    }
    root_[key] = value;
    LOG_DEBUG("add value: {} = {}", key, value);
    return true;
}
bool JsonHandler::add_value(const std::string &key, int value)
{
    if (!root_.is_object()) {
        return false;
    }
    root_[key] = value;
    LOG_DEBUG("add value: {} = {}", key, value);
    return true;
}
bool JsonHandler::get_value(const std::string &key, std::string &value)
{
    if (!root_.is_object()) {
        return false;
    }
    value = root_.value(key, "");
    LOG_DEBUG("get value: {} = {}", key, value);
    return !value.empty();
}
std::string JsonHandler::to_string() const
{
    return root_.dump(-1);
}