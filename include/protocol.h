#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

enum MsgType {
    MSG_REGISTER_REQUEST,
    MSG_REGISTER_RESPONSE,
    MSG_LOGIN_REQUEST,
    MSG_LOGIN_RESPONSE,
};

// 1字节对齐
struct __attribute__((packed)) MsgHeader {
    uint32_t total_size;
    uint32_t msg_type;
    char filename[256];
    uint32_t real_size;
};

struct Msg {
    MsgHeader header;
    std::vector<char> data;
};

// TODO: 字节序
std::vector<char> encode_msg(const Msg &msg);

bool decode_header(const char *data, size_t len, MsgHeader &header);

bool decode_msg(const char *data, size_t len, Msg &msg);
