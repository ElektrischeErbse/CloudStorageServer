#include "db_def.h"
#include "db_singleton.h"
#include <climits>
#include <optional>
#include <user_dao.h>
#include <vector>

bool UserDAO::init(const std::string &host, const std::string &user, const std::string &pwd, const std::string &db_name,
                   unsigned port)
{
    return DBSingleton::instance().connect(host, user, pwd, db_name, port);
}

bool UserDAO::disconnect()
{
    return DBSingleton::instance().disconnect();
}

bool UserDAO::execute(const char *sql)
{
    return DBSingleton::instance().execute(sql);
}

// 注册用户
bool UserDAO::register_user(const std::string &username, const std::string &pwd)
{
    user_info user = {0, username, pwd, 0};
    // "insert into `user_info` (`username`,`password`,`create_time`,`update_time`) values(?,?,?,?) "
    int ret = DBSingleton::instance().db().insert(user);
    if (ret == INT_MIN) {
        return false;
    }
    return true;
}

// 更新用户
bool UserDAO::update_user(const user_info &user_info)
{
    int ret = DBSingleton::instance().db().update(user_info);
    if (ret == INT_MIN) {
        return false;
    }
    return true;
}

// 按用户名查询
std::optional<user_info> UserDAO::get_user_by_name(const std::string &username)
{
    // "select `id`,`username`,`password`  from `user_info` where 1=1 and  username=? "
    std::vector<user_info> results = DBSingleton::instance().db().query_s<user_info>("username=?", username);
    if (results.empty()) {
        return std::nullopt;
    }
    return results[0];
}

// 按ID查询
std::optional<user_info> UserDAO::get_user_by_id(int id)
{
    std::vector<user_info> results = DBSingleton::instance().db().query_s<user_info>("id=?", id);
    if (results.empty()) {
        return std::nullopt;
    }
    return results[0];
}

// 用户名是否存在
bool UserDAO::exists(const std::string &username)
{
    auto results = DBSingleton::instance().db().query_s<user_info>("username=?", username);
    return !results.empty();
}