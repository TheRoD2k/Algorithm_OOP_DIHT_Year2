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
	std::vector<int> prefix_vector;
	int input;
	while (std::cin >> input)
		prefix_vector.push_back(input);
	std::vector<char> string(prefix_vector.size());
	std::vector<uint32_t> bitset_vector(string.size(), c_full_alphabet);
	string[0] = 'a';
	for (int i = 1; i < string.size(); i++) {
		if (prefix_vector[i] > 0)
			string[i] = string[prefix_vector[i] - 1];
		else {
			int j = i - 1;
			for (int k = 0; k <= prefix_vector[j]; k++)
			{
				bool suits = true;
				for (int t = 0; (t < k) && suits; t++)
					if (string[i - k + t] != string[t])
						suits = false;
				if (suits)
					removeBit(bitset_vector[i], getIndex(string[k]));
			}
			removeBit(bitset_vector[i], getIndex(string[0]));
			string[i] = getAvailableLetter(bitset_vector[i]);
		}
	}

	for (int i = 0; i < string.size(); i++)
		std::cout << string[i];
	return 0;
}
