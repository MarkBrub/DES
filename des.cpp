#include <vector>
#include "util.hpp"
#include "des.hpp"

namespace des {
	const std::vector<int> PC1 = { 57, 49, 41, 33, 25, 17, 9, 1,
		58, 50, 42, 34, 26, 18, 10, 2,
		59, 51, 43, 35, 27, 19, 11, 3,
		60, 52, 44, 36, 63, 55, 47, 39,
		31, 23, 15, 7, 62, 54, 46, 38,
		30, 22, 14, 6, 61, 53, 45, 37,
		29, 21, 13, 5, 28, 20, 12, 4 };
	const std::vector<int> PC2 = { 14, 17, 11, 24, 1, 5,
		3, 28, 15, 6, 21, 10,
		23, 19, 12, 4, 26, 8,
		16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32 };
	const std::vector<int> shiftKey = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
	const std::vector<int> IP = { 58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17,  9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7 };
	const std::vector<int> E = { 32,  1, 2,  3,  4, 5,
		4,  5, 6,  7,  8, 9,
		8,  9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32, 1 };
	const std::vector<int> P = {16, 7, 20, 21, 29, 12, 28, 17,
		1, 15, 23, 26, 5, 18, 31, 10,
		2, 8, 24, 14, 32, 27, 3, 9,
		19, 13, 30, 6, 22, 11, 4, 25};
	const std::vector<int> IPPrime = { 40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41, 9, 49, 17, 57, 25};
	
	std::vector<std::vector<unsigned char>> generateKeys(std::vector<unsigned char> key) {
		std::vector<unsigned char> output(7);
		std::vector<std::vector<unsigned char>> keys;
		std::vector<std::vector<unsigned char>> subkeys;
	
		//runs the key through PC1
		output = permutate(key, PC1, 7);
	
		//generates the next set of keys using shifts
		for (int x = 0; x < shiftKey.size(); x++) {
			for (int y = 0; y < shiftKey[x]; y++) {
				unwrappedShift(output);
			}
			keys.push_back(output);
		}
	
		//genertes the final keys by running the shifted keys though PC2
		for (int x = 0; x < keys.size(); x++) {
			subkeys.push_back(permutate(keys[x], PC2, 6));
		}
	
		return subkeys;
	}
	
	std::vector<std::vector<unsigned char>> encrypt(std::vector<std::vector<unsigned char>> plaintext, std::vector<unsigned char> key) {
		std::vector<std::vector<unsigned char>> cypherText;
		std::vector<std::vector<unsigned char>> subkeys;
		
		//generate the subkeys form the key
		subkeys = des::generateKeys(key);
		
		for (int x = 0; x < plaintext.size(); x++) {
			std::vector<unsigned char> l;
			std::vector<unsigned char> lPrime;
			std::vector<unsigned char> r;
			std::vector<unsigned char> rPrime;
			std::vector<unsigned char> result;
		
			//run the plaintext through IP
			plaintext[x] = permutate(plaintext[x], IP);
		
			//split the plaintext into left and right halves
			std::copy(plaintext[x].begin(), plaintext[x].begin() + 4, std::back_inserter(lPrime));
			std::copy(plaintext[x].begin() + 4, plaintext[x].begin() + 8, std::back_inserter(rPrime));
		
			//iterate through for the bulk of the encryption
			for (int x = 0; x < 16; x++) {
				l = rPrime;
				r = permutate(rPrime, E, 6);
				r ^= subkeys[x];
				r = sustitute(r);
				r = permutate(r, P, 4);
				r ^= lPrime;
				rPrime = r;
				lPrime = l;
			}
			
			//combine the left and right hlaves
			result.insert(result.begin(), r.begin(), r.end());
			result.insert(result.end(), l.begin(), l.end());
			
			//run the final result through IPPrime
			cypherText.push_back(permutate(result, IPPrime));
		}
		return cypherText;
	}
	
	std::vector<std::vector<unsigned char>> decrypt(std::vector<std::vector<unsigned char>> plaintext, std::vector<unsigned char> key) {
		std::vector<std::vector<unsigned char>> cypherText;
		std::vector<std::vector<unsigned char>> subkeys;
		
		//generate the subkeys form the key
		subkeys = des::generateKeys(key);
		
		for (int x = 0; x < plaintext.size(); x++) {
			std::vector<unsigned char> l;
			std::vector<unsigned char> lPrime;
			std::vector<unsigned char> r;
			std::vector<unsigned char> rPrime;
			std::vector<unsigned char> result;
		
			//run the plaintext through IP
			plaintext[x] = permutate(plaintext[x], IP);
		
			//split the plaintext into left and right halves
			std::copy(plaintext[x].begin(), plaintext[x].begin() + 4, std::back_inserter(lPrime));
			std::copy(plaintext[x].begin() + 4, plaintext[x].begin() + 8, std::back_inserter(rPrime));
		
			//iterate through for the bulk of the encryption
			for (int x = 0; x < 16; x++) {
				l = rPrime;
				r = permutate(rPrime, E, 6);
				r ^= subkeys[15 - x];
				r = sustitute(r);
				r = permutate(r, P, 4);
				r ^= lPrime;
				rPrime = r;
				lPrime = l;
			}
			
			//combine the left and right hlaves
			result.insert(result.begin(), r.begin(), r.end());
			result.insert(result.end(), l.begin(), l.end());
			
			//run the final result through IPPrime
			cypherText.push_back(permutate(result, IPPrime));
		}
		return cypherText;
	}
}