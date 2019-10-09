#include <iostream>
#include <string>
#include <vector>

std::vector<int> ClassicalPrefixFunction(std::string &str) {
	std::vector<int> prefix_vector(str.size(), 0);
	for (int i = 1; i < str.size(); i++) {
		int temp = prefix_vector[i - 1];
		while ((temp > 0) && (str[i] != str[temp]))
			temp = prefix_vector[temp - 1];
		if (str[i] == str[temp])
			++temp;
		prefix_vector[i] = temp;
	}
	return prefix_vector;
}

struct PrimitivePair {
	char letter;
	int prefix = 0;
};

bool CheckIfSubstringPrefixFunction(std::string& pattern, std::vector<int>& prefix_vector, char input) {
	static int last_prefix = 0, current_prefix = 0;
	current_prefix = last_prefix;
	while ((current_prefix > 0) && (pattern[current_prefix] != input)) {
		current_prefix = prefix_vector[current_prefix - 1];
	}
	if (pattern[current_prefix] == input)
		++current_prefix;
	last_prefix = current_prefix;
	return (last_prefix == pattern.size() ? true : false);
}

void ReadLineAndPrintSubstringIndexes(std::string& pattern) {
	std::vector<int> prefix_vector = ClassicalPrefixFunction(pattern);
	char input;
	int pos = 0;
	while (std::cin >> input) {
		if (CheckIfSubstringPrefixFunction(pattern, prefix_vector, input))
			std::cout << pos - pattern.size() + 1 << " ";
		++pos;
	}
}

int main() {
	std::string pattern;
	std::cin.tie(nullptr);
	std::cin >> pattern;
	ReadLineAndPrintSubstringIndexes(pattern);
	return 0;
}
