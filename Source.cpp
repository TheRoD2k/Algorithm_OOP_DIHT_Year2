#include "biginteger.h"
#include <cassert>

int main() {
	std::string first, second;
	std::cin >> first >> second;
	BigInteger f(first), s(second);
	std::cout << "A + B = " << f + s << std::endl;
	std::cout << "A - B = " << f - s << std::endl;
	std::cout << "A * B = " << f * s << std::endl;
	if (s != 0) {
		std::cout << "A // B = " << f / s << std::endl;
		std::cout << "A % B = " << f % s << std::endl;
	}
	else
		std::cout << "No division - B == 0! No division by zero exception throw as exception.h is unavailable" << std::endl;
	std::cout << "bool(A), bool(B) == " << bool(f) << ", " << bool(s) << std::endl;
}