#pragma once

#include <cstdint>
#include <ormpp/dbng.hpp>
#include <ormpp/mysql.hpp>
#include <string>

using namespace ormpp;

// 用户表
struct user_info {
    int id;
    std::string username;
    std::string password;
    int8_t online_status;
    std::string create_time;
    std::string update_time;
    static constexpr std::string_view get_alias_struct_name(user_info *)
    {
#if defined(USER_DAO_TEST)
        return "user_info_test";
#else
        return "user_info";
#endif
    }
};
REGISTER_AUTO_KEY(user_info, id)
// ormpp 不支持datetime类型
// TODO: 更换ORM
YLT_REFL(user_info, id, username, password, online_status)

// 好友关系表
struct friend_info {
    int id;
    int user_id;
    int friend_id;
    std::string create_time;
    std::string update_time;
};
REGISTER_AUTO_KEY(friend_info, id);
// ormpp 不支持datetime类型
YLT_REFL(friend_info, id, user_id, friend_id);