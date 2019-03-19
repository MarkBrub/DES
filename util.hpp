#pragma once
#include <bitset>
#include <vector>

std::bitset<8> binarize(unsigned char raw);
void clearVector(std::vector<unsigned char>& block);
void printBlock(std::vector<std::vector<unsigned char>>& block);
void printBlock(std::vector<unsigned char>& block);
void printBlock(unsigned char& block);
void printKey(std::vector<unsigned char>& block);
void printKey(std::vector<std::vector<unsigned char>>& keys);
void printAsHex(std::vector<unsigned char>& block);
std::vector<std::vector<unsigned char>> stringToVector(std::string text);
std::vector<unsigned char> permutate(std::vector<unsigned char> initial, std::vector<int> permutation, int length = 8);
void unwrappedShift(std::vector<unsigned char>& text);
std::vector<unsigned char> sustitute(const std::vector<unsigned char>& initial);
bool getBit(const std::vector<unsigned char>& initial, const int& set, const int& bit);
std::vector<unsigned char>& operator ^= (std::vector<unsigned char>& left, const std::vector<unsigned char>& right);