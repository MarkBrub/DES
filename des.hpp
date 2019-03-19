#pragma once

namespace des {
    std::vector<std::vector<unsigned char>> generateKeys(std::vector<unsigned char> key);
    std::vector<std::vector<unsigned char>> encrypt(std::vector<std::vector<unsigned char>> plaintext, std::vector<unsigned char> key);
    std::vector<std::vector<unsigned char>> decrypt(std::vector<std::vector<unsigned char>> plaintext, std::vector<unsigned char> key);
}