#include <user_service.h>

bool UserService::init(const std::string &host, const std::string &user, const std::string &pwd, const std::string &db_name,
                       unsigned port)
{
    return user_dao_.init(host, user, pwd, db_name, port);
}

bool UserService::disconnect()
{
    return user_dao_.disconnect();
}

bool UserService::register_service(const std::string &username, const std::string &pwd)
{
    if (user_dao_.exists(username)) {
        return false;
    }
    if (!user_dao_.register_user(username, pwd)) {
        return false;
    }
    return true;
}

bool UserService::login_service(const std::string &username, const std::string &pwd)
{
    auto user_opt = user_dao_.get_user_by_name(username);
    if (!user_opt.has_value()) {
        return false;
    }
    auto user_info = user_opt.value();
    if (user_info.password != pwd) {
        return false;
    }
    if (user_info.online_status == 1) {
        return false;
    }
    // 更新用户在线状态
    user_info.online_status = 1;
    if (!user_dao_.update_user(user_info)) {
        return false;
    }
    return true;
}

bool UserService::set_offline(const std::string &username)
{
    auto user_opt = user_dao_.get_user_by_name(username);
    if (!user_opt.has_value()) {
        return false;
    }
    auto user_info = user_opt.value();
    user_info.online_status = 0;
    if (!user_dao_.update_user(user_info)) {
        return false;
    }
    return true;
}