#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <queue>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <memory.h>
const int gk_english_alphabet_size = 26;

struct Node {
	Node();
	std::unordered_map<char, std::shared_ptr<Node>> son;
	std::unordered_map<char, std::shared_ptr<Node>> go;
	bool is_leaf = false;
	std::shared_ptr<Node> parent;
	std::shared_ptr<Node> up;
	std::shared_ptr<Node> suff_link;
	char char_to_parent;
	std::vector<int> id_vec;
	std::vector<int> leaf_pattern_number;

};

Node::Node() {
	is_leaf = false;
	parent = nullptr;
	up = nullptr;
	suff_link = nullptr;
	char_to_parent = gk_english_alphabet_size + 1;
	leaf_pattern_number.clear();
}

class Bohr {
public:
	Bohr();
	void init(std::string pattern);
	void process_text(std::string text);
	std::vector<int> restore_answer();

private:
	std::shared_ptr<Node> root;
	int text_size = 0;
	int subpatterns_amount = 0;
	int pattern_size = 0;
	std::vector<short> pattern_vector;

	std::shared_ptr<Node> get_suff_link(std::shared_ptr<Node> cur_node);
	std::shared_ptr<Node> get_link(std::shared_ptr<Node> cur_node, char char_to_parent);
	std::shared_ptr<Node> get_up(std::shared_ptr<Node> cur_node);
	void add_string(std::string pattern, int pattern_number, int string_id);
};

Bohr::Bohr() {
	root = std::make_shared<Node>();
	root->suff_link = root;
	text_size = 0;
}



std::shared_ptr<Node> Bohr::get_suff_link(std::shared_ptr<Node> cur_node) {
	if (cur_node->suff_link == nullptr) {
		if (cur_node == root || cur_node->parent == root)
			cur_node->suff_link = root;
		else
			cur_node->suff_link = get_link(get_suff_link(cur_node->parent), cur_node->char_to_parent);
	}
	return cur_node->suff_link;
}

std::shared_ptr<Node> Bohr::get_link(std::shared_ptr<Node> cur_node, char cur_char) {
	if (cur_node->go[cur_char] == nullptr) {
		if (cur_node->son[cur_char] != nullptr)
			cur_node->go[cur_char] = cur_node->son[cur_char];
		else
			if (cur_node == root)
				cur_node->go[cur_char] = root;
			else
				cur_node->go[cur_char] = get_link(get_suff_link(cur_node), cur_char);
	}
	return cur_node->go[cur_char];
}

std::shared_ptr<Node> Bohr::get_up(std::shared_ptr<Node> cur_node) {
	if (cur_node->up == nullptr) {
		if (get_suff_link(cur_node)->is_leaf)
			cur_node->up = get_suff_link(cur_node);
		else
			if (get_suff_link(cur_node) == root)
				cur_node->up = root;
			else
				cur_node->up = get_up(get_suff_link(cur_node));
	}
	return cur_node->up;
}

void Bohr::add_string(std::string pattern_string, int pattern_number, int string_id) {
	std::shared_ptr<Node> cur_node = root;
	pattern_vector.push_back({});
	for (unsigned int i = 0; i < pattern_string.size(); i++) {
		char cur_char = pattern_string[i] - 'a';
		if (cur_node->son[cur_char] == nullptr) {
			cur_node->son[cur_char] = std::make_shared<Node>();
			cur_node->son[cur_char]->parent = cur_node;
			cur_node->son[cur_char]->char_to_parent = cur_char;
		}
		cur_node = cur_node->son[cur_char];
	}
	cur_node->is_leaf = true;
	cur_node->id_vec.push_back(string_id);
	cur_node->leaf_pattern_number.push_back(pattern_number);
}

void Bohr::process_text(std::string text) {
	std::shared_ptr<Node> cur_node = root;
	text_size = text.size();
	pattern_vector.resize(text_size);
	for (int i = 0; i < text.size(); i++) {
		char cur_char = text[i] - 'a';
		cur_node = get_link(cur_node, cur_char);
		std::shared_ptr<Node> temp_node = cur_node;
		while (temp_node != root) {
			temp_node->up = get_up(temp_node);
			if (temp_node->is_leaf)
				for (int j = 0; j < temp_node->id_vec.size(); j++) {
					if (i - temp_node->id_vec[j] >= 0)
						++pattern_vector[i - temp_node->id_vec[j]];
				}
			temp_node = temp_node->up;
		}
	}
}

void Bohr::init(std::string pattern_string) {
	std::string temp = "";
	pattern_size = pattern_string.size();
	pattern_string += '?';
	int count = 0;
	subpatterns_amount = 0;
	for (unsigned int i = 0; i < pattern_string.size(); i++) {
		if (pattern_string[i] == '?') {
			if (temp.size() != 0) {
				add_string(temp, count++, i - 1);
				temp = "";
				++subpatterns_amount;
			}
		}
		else {
			temp += pattern_string[i];
		}
	}
}

std::vector<int> Bohr::restore_answer() {
	std::vector<int> answer;
	for (int i = 0; i < pattern_vector.size(); i++)
		if (pattern_vector[i] == subpatterns_amount && i <= text_size - pattern_size)
			answer.push_back(i);
	return answer;
}

int main() {
	std::string pattern, text;
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cin >> pattern >> text;
	Bohr bohr;
	bohr.init(pattern);
	bohr.process_text(text);

	auto answer_vec = bohr.restore_answer();
	for (int i = 0; i < answer_vec.size(); i++) {
		std::cout << answer_vec[i] << " ";
	}
	std::cout << std::endl;
}
