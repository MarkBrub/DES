//g++ main.cpp util.cpp des.cpp -std=c++11 -o main.exe
#include <iostream>
#include <vector>
#include <string>
#include "util.hpp"
#include "des.hpp"

int main() {
	std::string text = "I can't believe that this actually works!";
	std::vector<std::vector<unsigned char>> plaintext;
	std::vector<unsigned char> key = {0x15, 0xF4, 0x6A, 0x79, 0xBB, 0x0C, 0xDF, 0xFE};
	std::vector<std::vector<unsigned char>> results;
	
	plaintext = stringToVector(text);
	
	printBlock(plaintext);
	
	results = des::encrypt(plaintext, key);
	
	printBlock(results);
	
	results = des::decrypt(results, key);
	
	printBlock(results);
	
	return 0;
}