#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <iomanip>

std::vector<int64_t> calc_positions(std::vector<int64_t> count) {
	count[0] = 1;
	for (int i = 1; i < count.size(); i++)
		count[i] += count[i - 1];
	return count;
}

std::vector<int64_t> get_suff_array(std::string str) {

	std::vector<int64_t> count(std::max(256u, unsigned(str.size())), 0);
	for (auto ch : str)
		count[ch]++;

	count = calc_positions(count);
	std::vector<int64_t> prev_count(count.size());
	prev_count[0] = 0;
	for (int i = 1; i < prev_count.size(); i++)
		prev_count[i] = count[i - 1];
	std::vector<int64_t> suffs(str.size());
	for (int i = 0; i < str.size(); i++) {
		suffs[prev_count[str[i]]++] = i;
	}
	std::vector<int64_t> classes(str.size());
	int classesN = 0;
	char last_char = str[str.size() - 1];

	for (int i = 0; i < suffs.size(); i++) {
		if (str[suffs[i]] != last_char) {
			last_char = str[suffs[i]];
			classesN++;
		}
		classes[suffs[i]] = classesN;
	}


	int cur_len = 1;
	while (cur_len <= str.size()) {
		std::vector<int64_t> sorted_by2(str.size());
		for (int i = 0; i < str.size(); i++)
			sorted_by2[i] = (suffs[i] + str.size() - cur_len) % str.size();

		std::fill(count.begin(), count.end(), 0);

		for (int i = 0; i < sorted_by2.size(); i++) {
			count[classes[sorted_by2[i]]]++;
		}

		count = calc_positions(count);
		prev_count[0] = 0;
		for (int i = 1; i < prev_count.size(); i++)
			prev_count[i] = count[i - 1];

		for (int i = 0; i < str.size(); i++)
			suffs[prev_count[classes[sorted_by2[i]]]++] = sorted_by2[i];

		std::vector<int64_t> new_classes(str.size());
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

std::vector<int64_t> build_lcp_array(std::string str, std::vector<int64_t>  suffs) {
	std::vector<int64_t> lcp(str.size());
	std::vector<int64_t> pos(str.size());
	for (int i = 0; i < str.size(); i++)
		pos[suffs[i]] = i;
	int64_t k = 0;
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

void print_array(std::vector<int64_t> arr) {
	for (int i = 0; i < arr.size(); i++)
		std::cout << std::setw(3) << arr[i] << " ";
	std::cout << std::endl;
}

std::string get_common_substring_by_order(std::string s_string, std::string t_string, int64_t k) {
	std::string str = s_string + '\1' + t_string + '\0';
	auto suff_array = get_suff_array(str);
	auto lcp_array = build_lcp_array(str, suff_array);
	int prev_index = -1;
	int64_t count = 0, prev_count = 0;
	std::string answer = "";
	
	for (int i = 2; i < str.size() - 1 && count < k; i++) {
		if ((suff_array[i] <= s_string.size() && suff_array[i + 1] >= s_string.size())
			|| (suff_array[i] >= s_string.size() && suff_array[i + 1] <= s_string.size())) {
			if (prev_index == -1)
				prev_index = i;
			else {
				count -= *std::min_element(lcp_array.begin() + prev_index, lcp_array.begin() + i + 1);
				prev_index = i;
			}
			count += lcp_array[i];
			if (count >= k) {
				for (int j = suff_array[i]; j < suff_array[i] + lcp_array[i] - (count - k); j++)
					answer += str[j];
			}
		}
		else {
		}
	}
	return (answer == "") ? "-1" : answer;
}

int main() {
	std::string s_string, t_string;
	int64_t k;
	std::cin >> s_string;
	std::cin >> t_string;
	std::cin >> k;

	std::string k_common_string = get_common_substring_by_order(s_string, t_string, k);
	std::cout << k_common_string << std::endl;
}
