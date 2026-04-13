#pragma once

#include <db_def.h>
#include <string>

class DBSingleton {
public:
    DBSingleton(const DBSingleton &) = delete;
    DBSingleton &operator=(const DBSingleton &) = delete;
    ~DBSingleton() = default;

    static DBSingleton &instance();

    bool connect(const std::string &ip, const std::string &user, const std::string &pwd, const std::string &db_name,
                 unsigned port);

    bool disconnect();

    bool execute(const char* sql);

    dbng<mysql> &db();

private:
    DBSingleton() = default;
    dbng<mysql> db_;
};