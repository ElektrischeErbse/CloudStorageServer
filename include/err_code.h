#pragma once

#include <string>
#include <unordered_map>

enum ErrorCode {
    REGISTER_SUCCESS,
    LOGIN_SUCCESS,
    REMOTE_JSON_PARSE_ERROR,
    REMOTE_GET_USERNAME_ERROR,
    REMOTE_GET_PWD_ERROR,
    REMOTE_REGISTER_USER_ERROR,
    REMOTE_LOGIN_USER_ERROR,
};


static std::unordered_map<ErrorCode, std::string> err_code_msg = {
    {REGISTER_SUCCESS, "注册成功"},
    {LOGIN_SUCCESS, "登录成功"},
    {REMOTE_JSON_PARSE_ERROR, "服务器解析JSON失败"},
    {REMOTE_GET_USERNAME_ERROR, "服务器解析用户名失败"},
    {REMOTE_GET_PWD_ERROR, "服务器解析密码失败"},
    {REMOTE_REGISTER_USER_ERROR, "服务器注册用户失败"},
    {REMOTE_LOGIN_USER_ERROR, "服务器登录用户失败"}
};