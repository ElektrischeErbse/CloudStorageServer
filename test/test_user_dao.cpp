#include <gtest/gtest.h>
#include <user_dao.h>

#define TEST_DB_HOST "127.0.0.1"
#define TEST_DB_USER "tom"
#define TEST_DB_PASS "tom"
#define TEST_DB_NAME "cloud_storage"
#define TEST_DB_PORT 3306

class UserDAOTest : public testing::Test {
protected:
    void SetUp() override
    {
        // 1. 初始化数据库连接
        bool ok = user_dao_.init(TEST_DB_HOST, TEST_DB_USER, TEST_DB_PASS, TEST_DB_NAME, TEST_DB_PORT);
        ASSERT_TRUE(ok) << "connect mysql failed";

        // 2. 创建测试表！
        const char *create_table_sql = R"(
            CREATE TABLE IF NOT EXISTS `user_info_test` (
                `id` INT PRIMARY KEY AUTO_INCREMENT,
                `username` VARCHAR(50) NOT NULL UNIQUE,
                `password` VARCHAR(50) NOT NULL,
                `create_time` DATETIME DEFAULT CURRENT_TIMESTAMP,
                `update_time` DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                `online_status` tinyint NOT NULL DEFAULT '0' COMMENT '在线状态 0=离线 1=在线'
            );
        )";

        bool ret = user_dao_.execute(create_table_sql);
        ASSERT_TRUE(ret) << "create table failed";
    }

    void TearDown() override
    {
        const char *drop_table_sql = "DROP TABLE IF EXISTS `user_info_test`;";
        user_dao_.execute(drop_table_sql);
        user_dao_.disconnect();
    }

    UserDAO user_dao_;
};

TEST_F(UserDAOTest, init_valid_params_success)
{
    SUCCEED();
}

TEST_F(UserDAOTest, init_invalid_params_failure)
{
    UserDAO dao;
    bool ret = dao.init("wrong_host", "wrong", "wrong", "wrong", 0);
    EXPECT_FALSE(ret);
}

TEST_F(UserDAOTest, register_user_unique_user_success)
{
    bool ret = user_dao_.register_user("test_user", "123456");
    EXPECT_TRUE(ret);
}

TEST_F(UserDAOTest, register_user_repeat_user_failure)
{
    bool ret = user_dao_.register_user("repeat_user", "123");
    ASSERT_TRUE(ret);
    ret = user_dao_.register_user("repeat_user", "456");
    EXPECT_FALSE(ret);
}

TEST_F(UserDAOTest, update_user_user_exist_success)
{
    user_info user;
    user.username = "update_test";
    user.password = "123456";
    user_dao_.register_user(user.username, user.password);

    auto opt_user = user_dao_.get_user_by_name("update_test");
    ASSERT_TRUE(opt_user.has_value());

    opt_user->password = "new_password_666";

    bool ret = user_dao_.update_user(opt_user.value());

    EXPECT_TRUE(ret);

    auto updated = user_dao_.get_user_by_name("update_test");
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(updated->password, "new_password_666");
}

TEST_F(UserDAOTest, update_user_user_not_exist_success)
{
    user_info user;
    user.id = 99999;
    user.username = "ghost";
    user.password = "pwd";

    bool ret = user_dao_.update_user(user);
    EXPECT_TRUE(ret);
}

TEST_F(UserDAOTest, update_user_username_duplicate_failure)
{
    // 插入用户A
    user_info userA;
    userA.username = "A";
    userA.password = "a";
    user_dao_.register_user(userA.username, userA.password);

    // 插入用户B
    user_info userB;
    userB.username = "B";
    userB.password = "b";
    user_dao_.register_user(userB.username, userB.password);

    auto optB = user_dao_.get_user_by_name("B");
    ASSERT_TRUE(optB.has_value());

    // 尝试把B的名字改成A
    optB->username = "A";

    bool ret = user_dao_.update_user(optB.value());
    EXPECT_FALSE(ret);
}

TEST_F(UserDAOTest, update_user_online_status_success)
{
    // 插入
    user_info user;
    user.username = "status_user";
    user.password = "123";
    user_dao_.register_user(user.username, user.password);

    // 查询
    auto opt = user_dao_.get_user_by_name("status_user");
    ASSERT_TRUE(opt.has_value());

    // 修改状态
    opt->online_status = 1;

    // 更新
    bool ret = user_dao_.update_user(opt.value());
    EXPECT_TRUE(ret);

    // 验证
    auto updated = user_dao_.get_user_by_name("status_user");
    EXPECT_EQ(updated->online_status, 1);
}

TEST_F(UserDAOTest, get_user_by_name_username_exist_success)
{
    bool ret = user_dao_.register_user("alice", "pass123");
    ASSERT_TRUE(ret);
    auto user = user_dao_.get_user_by_name("alice");
    ASSERT_TRUE(user.has_value());
    EXPECT_EQ(user->username, "alice");
}

TEST_F(UserDAOTest, get_user_by_name_username_not_exist_failure)
{
    auto user = user_dao_.get_user_by_name("not_exist");
    EXPECT_FALSE(user.has_value());
}

TEST_F(UserDAOTest, get_user_by_id_id_exist_success)
{
    bool ret = user_dao_.register_user("bob", "qwe");
    ASSERT_TRUE(ret);
    auto user_before = user_dao_.get_user_by_name("bob");
    ASSERT_TRUE(user_before.has_value());

    auto user_after = user_dao_.get_user_by_id(user_before->id);
    ASSERT_TRUE(user_after.has_value());
}

TEST_F(UserDAOTest, get_user_by_id_id_not_exist_failure)
{
    auto user = user_dao_.get_user_by_id(99999);
    EXPECT_FALSE(user.has_value());
}

TEST_F(UserDAOTest, exists_username_exist_success)
{
    bool ret = user_dao_.register_user("test_exist", "123");
    ASSERT_TRUE(ret);
    ret = user_dao_.exists("test_exist");
    EXPECT_TRUE(ret);
}

TEST_F(UserDAOTest, exists_username_not_exist_failure)
{
    bool ret = user_dao_.exists("not_exist");
    EXPECT_FALSE(ret);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}