

#ifndef GPP_PACKET_H
#define GPP_PACKET_H

#include <string>

namespace gpp {

class Packet {
public:
    uint32 type;
std::string command;
    std::string data;
    int64 timestamp;

    Packet();
    Packet(uint32 t, const std::string& d);

    Packet(const Packet& p);
    Packet& operator=(const Packet& p);

    std::string toString() const;
    std::string serialize() const;
    bool deserialize(const std::string& binaryData);
    static int64 currentTimeMillis();

    static void setEncryptionKey(const std::string& key);
    static void setIV(const std::string& iv);

public:
    static std::string encryptionKey;
    static std::string iv;
};
} // namespace gpp
#endif // GPP_PACKET_H
