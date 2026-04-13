#include <db_singleton.h>
#include <optional>

DBSingleton &DBSingleton::instance()
{
    static DBSingleton db_singleton;
    return db_singleton;
}

bool DBSingleton::connect(const std::string &ip, const std::string &user, const std::string &pwd, const std::string &db_name,
                          unsigned port)
{
    return db_.connect(ip, user, pwd, db_name, std::nullopt, port);
}

bool DBSingleton::disconnect()
{
    return db_.disconnect();
}

bool DBSingleton::execute(const char *sql)
{
    return db_.execute(sql);
}

dbng<mysql> &DBSingleton::db()
{
    return db_;
}