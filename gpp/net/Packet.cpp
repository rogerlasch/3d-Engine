
#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>

#include"../types.h"
#include "Packet.h"
#include <chrono>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>

using namespace std;

namespace gpp {

string Packet::encryptionKey="0123456789abcdef0123456789abcdef";
string Packet::iv="";

Packet::Packet(){
type=0;
command="";
data="";
timestamp=0;
}

Packet::Packet(uint32_t t, const string& d)
    : type(t), data(d), timestamp(currentTimeMillis()) {}

Packet::Packet(const Packet& p) {
    *this = p;
}

Packet& Packet::operator=(const Packet& p) {
    if (&p != this) {
        type = p.type;
command=p.command;
        data = p.data;
        timestamp = p.timestamp;
    }
    return *this;
}

string Packet::toString() const {
    stringstream ss;
    ss << "Type=" << type << ", Command: "<<command << endl;
    ss << "Data=" << data << endl;
    ss << "Timestamp=" << timestamp << endl;
    return ss.str();
}

int64_t Packet::currentTimeMillis() {
    return chrono::duration_cast<chrono::milliseconds>(
               chrono::system_clock::now().time_since_epoch()).count();
}

void Packet::setEncryptionKey(const string& key) {
    encryptionKey = key;
}

void Packet::setIV(const string& ivValue) {
    iv = ivValue;
}

string encrypt(const string& plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(Packet::encryptionKey.c_str()), reinterpret_cast<const unsigned char*>(Packet::iv.c_str()));

    string ciphertext;
    ciphertext.resize(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    int len;
    EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
    int ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]) + len, &len);
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}

string decrypt(const string& ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(Packet::encryptionKey.c_str()), reinterpret_cast<const unsigned char*>(Packet::iv.c_str()));

    string plaintext;
    plaintext.resize(ciphertext.size());

    int32 len;
    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    int32 plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]) + len, &len);
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}

string Packet::serialize() const {

    stringstream ss;

    {
        cereal::PortableBinaryOutputArchive oarchive(ss);
oarchive(type, command, data, timestamp);
    }

    return encrypt(ss.str());
}

bool Packet::deserialize(const string& binaryData) {
    string decryptedData = decrypt(binaryData);
    stringstream ss(decryptedData);
    try {
        cereal::PortableBinaryInputArchive iarchive(ss);
        iarchive(type, command, data, timestamp);
        return true;
    } catch (const exception&) {
        return false;
    }
}
} // namespace gpp
