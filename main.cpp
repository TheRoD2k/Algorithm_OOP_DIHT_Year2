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

std::string getMinLexString(std::vector<int> prefix_vector) {
	uint32_t c_alphabet_size = 26;
	uint32_t c_full_alphabet = (~((uint32_t)(0)) << (32 - c_alphabet_size)) >> (32 - c_alphabet_size);
	std::vector<char> string_vec(prefix_vector.size());
	std::vector<uint32_t> bitset_vector(string_vec.size(), c_full_alphabet);
	string_vec[0] = 'a';
	for (int i = 1; i < string_vec.size(); i++) {
		if (prefix_vector[i] > 0)
			string_vec[i] = string_vec[prefix_vector[i] - 1];
		else {
			int j = i - 1;
			for (int k = 0; k <= prefix_vector[j]; k++)
			{
				bool suits = true;
				for (int t = 0; (t < k) && suits; t++)
					if (string_vec[i - k + t] != string_vec[t])
						suits = false;
				if (suits)
					removeBit(bitset_vector[i], getIndex(string_vec[k]));
			}
			removeBit(bitset_vector[i], getIndex(string_vec[0]));
			string_vec[i] = getAvailableLetter(bitset_vector[i]);
		}
	}
	std::string min_lex_string;
	for (size_t i = 0; i < string_vec.size(); i++)
		min_lex_string += string_vec[i];
	return min_lex_string;
}

int main() {
	
	std::vector<int> prefix_vector;
	int input;
	while (std::cin >> input)
		prefix_vector.push_back(input);

	std::string min_lex_string = getMinLexString(prefix_vector);	
	std::cout << min_lex_string << std::endl;

	return 0;
}
