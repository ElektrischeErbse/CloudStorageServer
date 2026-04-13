#pragma once

#include "db_def.h"
#include <cstring>
#include <db_singleton.h>
#include <string>

class UserDAO {
public:
    bool init(const std::string &host, const std::string &user, const std::string &pwd, const std::string &db_name,
              unsigned port);

    bool disconnect();

    bool execute(const char *sql);

    // 注册用户
    bool register_user(const std::string &username, const std::string &pwd);

    // 更新用户
    bool update_user(const user_info &user_info);

    // 按用户名查询
    std::optional<user_info> get_user_by_name(const std::string &username);

    // 按ID查询
    std::optional<user_info> get_user_by_id(int id);

    // 用户名是否存在
    bool exists(const std::string &username);
};