#pragma once

#include "user_dao.h"
#include <string>

class UserService {
public:
    bool init(const std::string &host, const std::string &user, const std::string &pwd, const std::string &db_name,
              unsigned port);

    bool disconnect();

    // TODO: 登录和注册用户名，密码校验
    bool login_service(const std::string &username, const std::string &pwd);

    bool register_service(const std::string &username, const std::string &pwd);

    bool set_offline(const std::string &username);

private:
    UserDAO user_dao_;
};