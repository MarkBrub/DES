#include <iostream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <string>
#include "util.hpp"

std::bitset<8> binarize(unsigned char initial) {
	return std::bitset<8>(initial);
}

void clearVector(std::vector<unsigned char>& block) {
	for (int x = 0; x < block.size(); x++) {
		block[x] = 0x00;
	}
}

void printBlock(std::vector<std::vector<unsigned char>>& block) {
	for (int x = 0; x < block.size(); x++) {
		for (int y = 0; y < block[x].size(); y++) {
			std::cout << "(" << binarize(block[x][y]) << "," << block[x][y] << ")" << std::endl;
		}
	}
	std::cout << std::endl;
}

void printBlock(std::vector<unsigned char>& block) {
	for (int x = 0; x < block.size(); x++) {
		std::cout << "(" << binarize(block[x]) << "," << block[x] << ")" << std::endl;
	}
	std::cout << std::endl;
}

void printBlock(unsigned char& block) {
	std::cout << "(" << binarize(block) << "," << block << ")" << std::endl;
}

void printKey(std::vector<unsigned char>& key) { //Make it so that it finds out where it is being called based on the length and print in the right format and combine with printBlock
	switch (key.size())
	{
	case 6:
		for (int x = 0; x < 6; x++) {
			std::cout << binarize(key[x]);
		}
		std::cout << std::endl;
		break;
	case 7:
		std::cout << binarize(key[0]) << binarize(key[1]) << binarize(key[2]) << std::bitset<4>((key[3] & 240) >> 4) << std::endl;
		std::cout << std::bitset<4>(key[3]) << binarize(key[4]) << binarize(key[5]) << binarize(key[6]) << std::endl;
		break;
	default:
		std::cout << "Invalid key length" << std::endl;
		break;
	}
}

void printKey(std::vector<std::vector<unsigned char>>& keys) {
	for (int x = 0; x < keys.size(); x++) {
		printKey(keys[x]);
	}
}

void printAsHex(std::vector<unsigned char>& block) {
	for (int x = 0; x < block.size(); x++) {
		std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << std::bitset<8>(block[x]).to_ulong();
	}
	std::cout << std::endl;
}

std::vector<std::vector<unsigned char>> stringToVector(std::string text) {
	std::vector<std::vector<unsigned char>> result;
	std::vector<unsigned char> block(8);
	for (int x = 0; x < text.length(); x++) {
		int value = (int) text[x];
		block[x % 8] = value;
		if (x % 8 == 7 || x == text.length() - 1) {
			result.push_back(block);
			clearVector(block);
		}
	}
	return result;
}

std::vector<unsigned char> permutate(std::vector<unsigned char> initial, std::vector<int> permutation, int length) {
	std::vector<unsigned char> output(length);

	for (int x = 0; x < permutation.size(); x++) {
		int character = (permutation[x] - 1) / 8;
		int bit = permutation[x] - (8 * character);

		if (initial[character] & (128 >> (bit - 1))) { //check if the bit being accessed is set
			output[x / 8] += (128 >> (x % 8)); //set the appropriate bit in the output string
		}
	}

	return output;
}

void unwrappedShift(std::vector<unsigned char>& text) {
	bool temp1 = false;
	bool temp2 = false;

	temp1 = (text[0] & 128) >> 7;
	text[0] <<= 1;
	text[0] += (text[1] & 128) >> 7;
	text[1] <<= 1;
	text[1] += (text[2] & 128) >> 7;
	text[2] <<= 1;
	text[2] += (text[3] & 128) >> 7;

	temp2 = text[3] & 8;
	text[3] <<= 1;
	text[3] += (text[4] & 128) >> 7;
	text[3] &= 239;
	text[3] += temp1 << 4;

	text[4] <<= 1;
	text[4] += (text[5] & 128) >> 7;
	text[5] <<= 1;
	text[5] += (text[6] & 128) >> 7;
	text[6] <<= 1;
	text[6] += temp2;
}

std::vector<unsigned char> sustitute(const std::vector<unsigned char>& initial) {
	std::vector<unsigned char> result(4);
	const std::vector<std::vector<std::vector<int>>> subTable = {{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
		{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
		{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
		{{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
		{{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
		{{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
		{{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6,11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
		{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};
	int out, in;

	for (int set = 0; set < 8; set++) {
		out = (getBit(initial, set, 0) << 1) + getBit(initial, set, 5);
		in = (getBit(initial, set, 1) << 3) + (getBit(initial, set, 2) << 2) + (getBit(initial, set, 3) << 1) + getBit(initial, set, 4);
		
		
		if (set % 2 == 0) {
			result[set / 2] += subTable[set][out][in] << 4;
		} else {
			result[set / 2] += subTable[set][out][in];
		}
	}
	return result;
}

bool getBit(const std::vector<unsigned char>& initial, const int& set, const int& bit) {
	return (initial[((6 * set) + bit) / 8] & 128 >> (((6 * set) + bit) % 8));
}

std::vector<unsigned char>& operator ^= (std::vector<unsigned char>& left, const std::vector<unsigned char>& right) {
	for (int x = 0; x < left.size(); x++) {
		left[x] ^= right[x];
	}
	return left;
}