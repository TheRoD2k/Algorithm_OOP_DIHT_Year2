#include <iostream>
#include <vector>

uint32_t getIndex(char ch) {
	return (uint32_t)(ch - 'a');
}

char getLetter(uint32_t index) {
	return (char)('a' + index);
}

void removeBit(uint32_t& alphabet, uint32_t index) {
	alphabet &= (~((uint32_t)(1) << index));
}

char getAvailableLetter(uint32_t word) {
	uint32_t index = 0;
	while (word % 2 == 0) {
		word >>= (uint32_t)(1);
		++index;
	}
	return (char)('a' + index);
}

int main() {
	uint32_t c_alphabet_size = 26;
	uint32_t c_full_alphabet = (~((uint32_t)(0)) << (32 - c_alphabet_size)) >> (32 - c_alphabet_size);
	std::vector<int> z_vector;
	int input;
	while (std::cin >> input)
		z_vector.push_back(input);
	std::vector<uint32_t> bitset_vector(z_vector.size(), c_full_alphabet);
	std::vector<char> string(z_vector.size());
	string[0] = 'a';
	for (int i = 0; i < string.size()-1 && z_vector[i+1] == 0; i++)
		string[i] = 'a';
	for (int i = 1; i < string.size(); i++) {
		if (z_vector[i] > 0) {
			for (int j = 0; j < z_vector[i]; j++) {
				string[i + j] = string[j];
			}
			if (i + z_vector[i] < string.size() && z_vector[i] < string.size()) {
				removeBit(bitset_vector[i + z_vector[i]], getIndex(string[z_vector[i]]));
			}
		}
		else {
			removeBit(bitset_vector[i], getIndex('a'));
			string[i] = getAvailableLetter(bitset_vector[i]);
		}
	}
	for (int i = 0; i < string.size(); i++)
		std::cout << string[i];
}
