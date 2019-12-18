#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <iomanip>
unsigned gk_alphabet_size = 256u;

void calc_positions(std::vector<long long>& count) {
	count[0] = 1;
	for (int i = 1; i < count.size(); i++)
		count[i] += count[i - 1];
}

void zero_iteration_suff_array(const std::string& str, std::vector<long long>& count,
	std::vector<long long>& prev_count, std::vector<long long>& classes, std::vector<long long>& suffs, int& classesN) {
	prev_count[0] = 0;
	for (int i = 1; i < prev_count.size(); i++)
		prev_count[i] = count[i - 1];

	for (int i = 0; i < str.size(); i++) {
		suffs[prev_count[str[i]]++] = i;
	}

	char last_char = str[str.size() - 1];

	for (int i = 0; i < suffs.size(); i++) {
		if (str[suffs[i]] != last_char) {
			last_char = str[suffs[i]];
			classesN++;
		}
		classes[suffs[i]] = classesN;
	}
}

void sort_intervals(const std::string& str, std::vector<long long>& count,
	std::vector<long long>& prev_count, const std::vector<long long>& classes, std::vector<long long>& suffs, int& cur_len) {
	std::vector<long long> sorted_by2(str.size());
	for (int i = 0; i < str.size(); i++)
		sorted_by2[i] = (suffs[i] + str.size() - cur_len) % str.size();

	std::fill(count.begin(), count.end(), 0);

	for (int i = 0; i < sorted_by2.size(); i++) {
		count[classes[sorted_by2[i]]]++;
	}

	calc_positions(count);
	prev_count[0] = 0;
	for (int i = 1; i < prev_count.size(); i++)
		prev_count[i] = count[i - 1];

	for (int i = 0; i < str.size(); i++)
		suffs[prev_count[classes[sorted_by2[i]]]++] = sorted_by2[i];

}

std::vector<long long> get_suff_array(const std::string& str) {

	std::vector<long long> count(std::max(gk_alphabet_size, unsigned(str.size())), 0);
	for (auto ch : str)
		count[ch]++;

	calc_positions(count);
	std::vector<long long> prev_count(count.size());
	std::vector<long long> classes(str.size());
	std::vector<long long> suffs(str.size());
	int classesN = 0;
	zero_iteration_suff_array(str, count, prev_count, classes, suffs, classesN);

	int cur_len = 1;
	while (cur_len <= str.size()) {
		sort_intervals(str, count, prev_count, classes, suffs, cur_len);
		std::vector<long long> new_classes(str.size());
		int classesN = 0;

		for (int i = 1; i < str.size(); i++) {
			int mid1 = (suffs[i] + cur_len) % str.size();
			int mid2 = (suffs[i - 1] + cur_len) % str.size();

			if (classes[suffs[i]] != classes[suffs[i - 1]] || classes[mid1] != classes[mid2])
				classesN++;
			new_classes[suffs[i]] = classesN;
		}
		classes = new_classes;
		cur_len *= 2;
	}
	return suffs;
}


std::vector<long long> build_lcp_array(const std::string& str, const std::vector<long long>& suffs) {
	std::vector<long long> lcp(str.size());
	std::vector<long long> pos(str.size());
	for (int i = 0; i < str.size(); i++)
		pos[suffs[i]] = i;
	int k = 0;
	for (int i = 0; i < str.size(); i++) {
		if (k > 0)
			k--;
		if (pos[i] == str.size() - 1) {
			lcp[str.size() - 1] = -1;
			k = 0;
			continue;
		}
		else {
			int j = suffs[pos[i] + 1];
			while (std::max(i + k, j + k) < str.size() && str[i + k] == str[j + k])
				k++;
			lcp[pos[i]] = k;
		}
	}
	return lcp;
}

long long get_amount_of_unique_substrings(std::string input) {
	input += '\0';
	auto suff_arr = get_suff_array(input);
	auto lcp_array = build_lcp_array(input, suff_arr);
	long long sum = 0;
	for (int i = 0; i < input.size(); i++)
		sum += (input.size() - 1) - suff_arr[i];
	for (int i = 0; i < input.size() - 1; i++)
		sum -= lcp_array[i];
	return sum;
}

int main() {
	std::string input;
	std::cin >> input;
	long long amount_of_unique_substrings = get_amount_of_unique_substrings(input);
	std::cout << amount_of_unique_substrings << std::endl;
	return 0;
}
