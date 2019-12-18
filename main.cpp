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

std::string getMinLexString(const std::vector<int>& z_vector) {
	uint32_t c_alphabet_size = 26;
	uint32_t c_full_alphabet = (~((uint32_t)(0)) << (32 - c_alphabet_size)) >> (32 - c_alphabet_size);
	std::vector<uint32_t> bitset_vector(z_vector.size(), c_full_alphabet);
	std::vector<char> string_vec(z_vector.size());
	string_vec[0] = 'a';
	for (int i = 0; i < string_vec.size() - 1 && z_vector[i + 1] == 0; i++)
		string_vec[i] = 'a';
	for (int i = 1; i < string_vec.size(); i++) {
		if (z_vector[i] > 0) {
			for (int j = 0; j < z_vector[i]; j++) {
				string_vec[i + j] = string_vec[j];
			}
			if (i + z_vector[i] < string_vec.size() && z_vector[i] < string_vec.size()) {
				removeBit(bitset_vector[i + z_vector[i]], getIndex(string_vec[z_vector[i]]));
			}
		}
		else {
			removeBit(bitset_vector[i], getIndex('a'));
			string_vec[i] = getAvailableLetter(bitset_vector[i]);
		}
	}

	std::string min_lex_string;
	for (size_t i = 0; i < string_vec.size(); i++)
		min_lex_string += string_vec[i];
	return min_lex_string;
}

int main() {
	
	std::vector<int> z_vector;
	int input;
	while (std::cin >> input)
		z_vector.push_back(input);
	
	std::string min_lex_string = getMinLexString(z_vector);
	std::cout << min_lex_string << std::endl;
}
