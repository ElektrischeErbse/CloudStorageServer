#include <cstddef>
#include <cstdint>
#include <cstring>
#include <logger.h>
#include <protocol.h>
#include <string>
#include <vector>

std::vector<char> encode_msg(const Msg &msg)
{
    uint32_t total_size = msg.header.total_size;
    std::vector<char> result(total_size);
    memcpy(result.data(), &msg.header, sizeof(msg.header));
    memcpy(result.data() + sizeof(msg.header), msg.data.data(), msg.data.size());
    return result;
}

bool decode_header(const char *data, size_t len, MsgHeader &header)
{
    if (len < sizeof(MsgHeader)) {
        return false;
    }
    memcpy(&header, data, sizeof(MsgHeader));
    uint32_t total_size = header.total_size;
    uint32_t msg_type = header.msg_type;
    std::string filename = header.filename;
    uint32_t real_size = header.real_size;
    LOG_DEBUG("header[{}, {}, {}, {}]", total_size, msg_type, filename, real_size);
    return true;
}

bool decode_msg(const char *data, size_t len, Msg &msg)
{
    if (!decode_header(data, len, msg.header)) {
        return false;
    }
    uint32_t total_size = msg.header.total_size;
    uint32_t real_size = msg.header.real_size;
    if (len < total_size) {
        return false;
    }
    msg.data.resize(real_size);
    memcpy(msg.data.data(), data + sizeof(MsgHeader), real_size);
    return true;
}