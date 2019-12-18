#include <iostream>
#include <vector>
#include <string>

const long long gk_base = 10000;

int max(int left, int right) {
	return (left > right) ? left : right;
}

int min(int left, int right) {
	return (left < right) ? left : right;
}

signed char abs(signed char val) {
	return (val < 0 ? -val : val);
}

int abs(int val) {
	return (val < 0 ? -val : val);
}


class BigInteger {
public:
	BigInteger() { _data = std::vector<signed char>(1, 0); _positive = true; }
	BigInteger(std::vector<signed char>::const_iterator left, std::vector<signed char>::const_iterator right, bool positive = true)
		{_data = std::vector<signed char>(left, right); _positive = positive; _normalize();	};
	BigInteger(const std::vector<signed char>& data, bool positive = true) { _data = data; _positive = positive; _normalize(); };
	BigInteger(const BigInteger& bigint, bool new_positive) { _data = bigint._data; _positive = new_positive; _normalize(); };
	BigInteger(const std::string& str);
	BigInteger(int val);

	BigInteger& operator++() { *this = *this + 1; return *this; }
	BigInteger operator++(int) { *this = *this + 1; return *this; }

	explicit operator bool() { return (*this == BigInteger(0) ? 0 : 1); }

	friend BigInteger operator+(const BigInteger&, const BigInteger&);
	friend BigInteger operator-(const BigInteger&, const BigInteger&);
	BigInteger operator-() const { return BigInteger(*this, !_positive); };

	BigInteger operator*(const BigInteger&) const;
	BigInteger &operator*=(const BigInteger&);

	BigInteger operator/(const BigInteger&) const;
	BigInteger &operator/=(const BigInteger&);

	BigInteger operator%(const BigInteger&) const;
	BigInteger &operator%=(const BigInteger&);

	BigInteger operator+=(const BigInteger&);
	BigInteger operator-=(const BigInteger&);


	bool operator<(const BigInteger&) const;
	bool operator>(const BigInteger&) const;
	bool operator<=(const BigInteger& other) const { return !(*this > other); }
	bool operator>=(const BigInteger& other) const { return !(*this < other); }
	bool operator==(const BigInteger& other) const { return !(*this < other) && !(*this > other); }
	bool operator!=(const BigInteger& other) const { return !(*this == other); }
	std::string toString() const;

private:
	void _normalize();
	std::vector<long long> get_number(const std::vector<signed char>&, int) const;
	void extend_vec(std::vector<long long>&, int) const;
	std::vector<long long> karatsuba_mul(const std::vector<long long>& x, const std::vector<long long>& y) const;
	void finalize(std::vector<long long>& res) const;
	bool _positive;
	std::vector<signed char> _data;
};

BigInteger::BigInteger(const std::string& str) {
	std::string const* str_ptr = &str;
	std::string null_str = "0";
	if (str.size() == 0)
		str_ptr = &null_str;
	int first_ind = 0;
	_positive = true;
	if (str_ptr->at(0) == '-') {
		_positive = false;
		++first_ind;
	}
	for (int i = first_ind; i < (int)str.size(); i++)
		_data.push_back(str_ptr->at(i) - '0');
	_normalize();
};

BigInteger BigInteger::operator%(const BigInteger& right) const {
	return *this - (*this / right) * right;
}

BigInteger &BigInteger::operator%=(const BigInteger& right) {
	*this = *this - (*this / right) * right;
	return *this;
}

BigInteger BigInteger::operator/(const BigInteger& right) const {
	BigInteger new_left = *this;
	BigInteger new_right = right;
	new_right._positive = true;
	new_left._positive = true;
	BigInteger abs_right = BigInteger(right, true);
	BigInteger abs_left = BigInteger(*this, true);
	BigInteger temp;

	temp._data.clear();
	if (BigInteger(new_left, true) < BigInteger(new_right, true))
		return BigInteger(0);
	while (new_right <= new_left)
		new_right._data.push_back(0);
	while (new_right > abs_right) {
		temp._data.push_back(0);
		
		
		new_right._data.pop_back();

		long long temp_dig = 0;
		while (new_left >= new_right) {
			new_left -= new_right;
			++temp_dig;
		}
		temp._data[temp._data.size() - 1] = (signed char)temp_dig;
	}

	if (this->_positive != right._positive)
		temp._positive = false;
	else
		temp._positive = true;
	return temp;
}


std::ostream& operator<<(std::ostream& os, const BigInteger& bigint) {
	os << bigint.toString();
	return os;
}
std::istream& operator>>(std::istream& is, BigInteger& bigint) {
	std::string input;
	is >> input;
	bigint = BigInteger(input);
	return is;
}

void BigInteger::_normalize() {
	size_t first_ind = 0;
	while (first_ind < _data.size() && _data[first_ind] == 0)
		++first_ind;
	if (first_ind >= _data.size())
		_data = { 0 };
	else
	if (first_ind != 0) {
		for (size_t i = 0; i < _data.size() - first_ind; i++)
			_data[i] = _data[i + first_ind];
		_data.resize(_data.size() - first_ind);
	}
}

BigInteger &BigInteger::operator/=(const BigInteger& right) {
	*this = *this / right;
	return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger& right) {
	*this = *this * right;
	return *this;
}

BigInteger operator-(const  BigInteger& left, const BigInteger& right) {
	return left + (-right);
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {
	BigInteger const* new_left;
	BigInteger const* new_right;
	if (BigInteger(left, true) < BigInteger(right, true)) {
		new_left = &right;
		new_right = &left;
	}
	else {
		new_left = &left;
		new_right = &right;
	}
	int max_size = max(left._data.size(), right._data.size());
	int left_size = new_left->_data.size();
	int right_size = new_right->_data.size();
	std::vector<signed char> new_data(max_size + 1, 0);

	for (int i = max_size; i - (max_size + 1 - left_size) >= 0; i--) {
		new_data[i] = new_left->_data[i - (max_size + 1 - left_size)];
	}

	if ((new_left->_positive && new_right->_positive) || (!new_left->_positive && !new_right->_positive)) {
		for (int i = max_size; i > 0; i--) {
			if (i - (max_size + 1 - right_size) >= 0)
				new_data[i] += new_right->_data[i - (max_size + 1 - right_size)];
			new_data[i - 1] += new_data[i] / 10;
			new_data[i] %= 10;
		}
	}
	else {
		for (int i = max_size; i > 0; i--) {
			if (new_data[i] < 0) {
				new_data[i] = 9;
				--new_data[i - 1];
			}
			if (i - (max_size + 1 - right_size) >= 0)
				new_data[i] -= new_right->_data[i - (max_size + 1 - right_size)];
			if (new_data[i] < 0) {
				new_data[i] = 10 + new_data[i];
				--new_data[i - 1];
			}
		}

	}

	int first_ind = 0;
	while (first_ind < max_size && new_data[first_ind] == 0)
		++first_ind;
	bool new_positive = false;
	if (left._positive && right._positive)
		new_positive = true;
	if (left._positive && !right._positive && new_left == &left)
		new_positive = true;
	if (!left._positive && right._positive && new_right == &left)
		new_positive = true;
	return (first_ind <= max_size) ? BigInteger(new_data.begin() + first_ind, new_data.end(), new_positive) : BigInteger();
}


std::vector<long long> BigInteger::get_number(const std::vector<signed char>& num, int base) const {
	std::vector<long long> new_number;
	
	long long temp = 0;
	long long dig = 1;
	for (int i = num.size() - 1; i >= 0; i--) {
		temp += static_cast<long long>(num[i]) * dig;
		dig *= 10;
		if (dig == base) {
			new_number.push_back(temp);
			temp = 0;
			dig = 1;
		}
	}

	if (temp != 0) {
		new_number.push_back(temp);
	}
	return new_number;
}

void BigInteger::extend_vec(std::vector<long long>& vec, int min_size) const {
	while (min_size & (min_size - 1))
		++min_size;
	vec.resize(min_size);
}

void BigInteger::finalize(std::vector<long long>& res) const {
	for (int i = 0; i < static_cast<int>(res.size()) - 1; ++i) {
		res[i + 1] += res[i] / gk_base;
		res[i] %= gk_base;
	}
}

BigInteger BigInteger::operator*(const BigInteger &right) const {
	const BigInteger& left = *this;
	std::vector<long long> first = get_number(left._data, gk_base);
	std::vector<long long> second = get_number(right._data, gk_base);
	size_t min_size = max(first.size(), second.size());

	extend_vec(first, min_size);
	extend_vec(second, min_size); 

	std::vector<long long> new_res = karatsuba_mul(first, second);
	finalize(new_res);
	

	std::vector<signed char> new_val;

	auto cur_num_iterator = new_res.crbegin();
	while (cur_num_iterator != new_res.crend() && !*cur_num_iterator) {
		++cur_num_iterator;
	}
	if (cur_num_iterator == new_res.crend())
		return BigInteger();

	while (cur_num_iterator != new_res.crend()) {
		int zero_amount = 0;
		long long num = *cur_num_iterator;

		if (num == 0) {
			num += 1;
		}
		if (num < gk_base) {
			while ((num *= 10) < gk_base) {
				++zero_amount;
			}
		}
		while (zero_amount > 0) {
			--zero_amount;
			new_val.push_back(0);
		}
		long long temp_num = *cur_num_iterator++;
		std::string temp_str = std::to_string(temp_num);
		for (size_t i = 0; i < temp_str.size(); i++)
			new_val.push_back(temp_str[i] - '0');
	}

	size_t first_index = 0;
	for (size_t i = 0; i < new_val.size() && new_val[first_index] == 0; i++)
		++first_index;
	int new_positive = false;
	if ((left._positive && right._positive) || (!left._positive && !right._positive))
		new_positive = true;
	return (first_index < new_val.size()) ? BigInteger(new_val.begin() + first_index, new_val.end(), new_positive) : BigInteger();
}



std::vector<long long> BigInteger::karatsuba_mul(const std::vector<long long>& x, const std::vector<long long>& y) const {
	int len = x.size();
	std::vector<long long> res(2 * len, 0);


	if (len <= 1) {
		if (x.size() > 0 || y.size() > 0)
			res[0] += x[0] * y[0];
		return res;
	}

	int temp_len = len / 2;

	std::vector<long long> first_right{ x.begin(), x.begin() + temp_len };
	std::vector<long long> first_left{ x.begin() + temp_len, x.end() };
	std::vector<long long> second_right{ y.begin(), y.begin() + temp_len };
	std::vector<long long> second_left{ y.begin() + temp_len, y.end() };


	std::vector<long long> first_sum(temp_len);
	std::vector<long long> second_sum(temp_len);

	for (int i = 0; i < temp_len; ++i) {
		first_sum[i] = first_left[i] + first_right[i];
		second_sum[i] = second_left[i] + second_right[i];
	}

	std::vector<long long> P1 = karatsuba_mul(first_left, second_left);
	std::vector<long long> P2 = karatsuba_mul(first_right, second_right);
	std::vector<long long> P3 = karatsuba_mul(first_sum, second_sum);

	for (auto i = 0; i < len; ++i)
		P3[i] -= P2[i] + P1[i];

	for (auto i = 0; i < len; ++i)
		res[i] = P2[i];

	for (auto i = len; i < 2 * len; ++i)
		res[i] = P1[i - len];

	for (auto i = temp_len; i < len + temp_len; ++i)
		res[i] += P3[i - temp_len];

	return res;
}

bool BigInteger::operator<(const BigInteger& right) const {
	const BigInteger& left = *this;
	bool first_equals_zero = true, second_equals_zero = true;
	for (size_t i = 0; i < right._data.size(); i++)
		if (right._data[i] != 0)
			second_equals_zero = false;
	for (size_t i = 0; i < left._data.size(); i++)
		if (left._data[i] != 0)
			first_equals_zero = false;
	if (first_equals_zero && second_equals_zero)
		return false;
	if (first_equals_zero) {
		if (right._positive)
			return true;
		else
			return false;
	}

	if (second_equals_zero) {
		if (left._positive)
			return false;
		else
			return true;
	}
	
	if (left._positive && right._positive) {
		if (left._data.size() < right._data.size())
			return true;
		if (left._data.size() > right._data.size())
			return false;
		for (size_t i = 0; i < left._data.size(); i++) {
			if (left._data[i] < right._data[i])
				return true;
			if (left._data[i] > right._data[i])
				return false;
		}
		return false;
	}

	if (!left._positive && !right._positive) {
		if (left._data.size() > right._data.size())
			return true;
		if (left._data.size() < right._data.size())
			return false;
		for (size_t i = 0; i < left._data.size(); i++) {
			if (left._data[i] > right._data[i])
				return true;
			if (left._data[i] < right._data[i])
				return false;
		}
		return false;
	}
	if (!left._positive && right._positive)
		return true;
	return false;
}

bool BigInteger::operator>(const BigInteger& right) const {
	const BigInteger& left = *this;
	bool first_equals_zero = true, second_equals_zero = true;
	for (size_t i = 0; i < right._data.size(); i++)
		if (right._data[i] != 0)
			second_equals_zero = false;
	for (size_t i = 0; i < left._data.size(); i++)
		if (left._data[i] != 0)
			first_equals_zero = false;
	if (first_equals_zero && second_equals_zero)
		return false;

	if (first_equals_zero) {
		if (right._positive)
			return false;
		else
			return true;
	}

	if (second_equals_zero) {
		if (left._positive)
			return true;
		else
			return false;
	}

	if (left._positive && right._positive) {
		if (left._data.size() < right._data.size())
			return false;
		if (left._data.size() > right._data.size())
			return true;
		for (size_t i = 0; i < left._data.size(); i++) {
			if (left._data[i] < right._data[i])
				return false;
			if (left._data[i] > right._data[i])
				return true;
		}
		return false;
	}

	if (!left._positive && !right._positive) {
		if (left._data.size() > right._data.size())
			return false;
		if (left._data.size() < right._data.size())
			return true;
		for (size_t i = 0; i < left._data.size(); i++) {
			if (left._data[i] > right._data[i])
				return false;
			if (left._data[i] < right._data[i])
				return true;
		}
		return false;
	}
	if (!left._positive && right._positive)
		return false;
	return true;
}

BigInteger::BigInteger(int val) {
	if (val == 0) {
		_data = std::vector<signed char>(1, 0);
		_positive = true;
		return;
	}
	_positive = (val < 0 ? false : true);
	val = abs(val);
	int temp_val = val;
	int new_size = 0;
	while (temp_val > 0) {
		temp_val /= 10;
		++new_size;
	}
	_data = std::vector<signed char>(new_size, 0);
	for (int i = new_size - 1; i >= 0; i--) {
		_data[i] = val % 10;
		val /= 10;
	}
}



std::string BigInteger::toString() const {
	std::string num_string;
	if (*this != BigInteger() && !_positive)
		num_string = '-';
	for (size_t i = 0; i < _data.size(); i++)
		num_string += _data[i] + '0';
	return num_string;
}




BigInteger BigInteger::operator+=(const BigInteger& right) {
	*this = *this + right;
	return *this;
}

BigInteger BigInteger::operator-=(const BigInteger& right) {
	*this = *this + (-right);
	return *this;
}
