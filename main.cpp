#include <iostream>
#include <string>
#include <vector>

void SearchSymbol(const std::vector<int>& prefix_vector, const std::string& pattern, const char& input, int& current_prefix, int& last_prefix ) {
	while ((current_prefix > 0) && (pattern[current_prefix] != input)) {
		current_prefix = prefix_vector[current_prefix - 1];
	}
	if (pattern[current_prefix] == input)
		++current_prefix;
	last_prefix = current_prefix;
}

std::vector<int> ClassicalPrefixFunction(const std::string& pattern) {
	std::vector<int> prefix_vector(pattern.size(), 0);
	for (int i = 1; i < pattern.size(); i++) {
		int current_prefix = prefix_vector[i - 1];
		SearchSymbol(prefix_vector, pattern, pattern[i], current_prefix, prefix_vector[i]);
	}
	return prefix_vector;
}


void ReadLineAndPrintSubstringIndexes(const std::string& pattern) {
	std::vector<int> prefix_vector = ClassicalPrefixFunction(pattern);
	char input;
	int pos = 0;
	int last_prefix = 0, current_prefix = 0;
	current_prefix = last_prefix;
	while (std::cin >> input) {
		SearchSymbol(prefix_vector, pattern, input, current_prefix, last_prefix);
		if (last_prefix == pattern.size())
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
