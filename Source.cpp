#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

class point {
public:
	point(double x, double y) : _x(x), _y(y) {};
	point() = default;
	double get_x() const { return _x; }
	double get_y() const { return _y; }
	bool operator<(point right) const;
private:
	double _x;
	double _y;
};

bool point::operator<(point right) const {
	if (_y < right.get_y() || _y == right.get_y() && _x < right.get_x())
		return true;
	return false;
}

class point_vector {
public:
	point_vector(point pnt1, point pnt2) : _pnt1(pnt1), _pnt2(pnt2) {}
	point_vector() = default;
	point first() const { return _pnt1; }
	point second() const { return _pnt2; }
private:
	point _pnt1;
	point _pnt2;
};

double scalar_mul(const point_vector& left, const point_vector& right) {
	double first_x = left.second().get_x() - left.first().get_x();
	double first_y = left.second().get_y() - left.first().get_y();
	double second_x = right.second().get_x() - right.first().get_x();
	double second_y = right.second().get_y() - right.first().get_y();
	double scalar = first_x * second_x + first_y * second_y;
	return scalar;
}

int vector_mul_sign(const point_vector& left, const point_vector& right) {
	double first_x = left.second().get_x() - left.first().get_x();
	double first_y = left.second().get_y() - left.first().get_y();
	double second_x = right.second().get_x() - right.first().get_x();
	double second_y = right.second().get_y() - right.first().get_y();
	double new_x = first_x * second_y - second_x * first_y;
	if (new_x > 0)
		return 1;
	if (new_x < 0)
		return -1;
	return 0;
}

class polygon {
public:
	polygon(const std::vector<point> &points) : _points(points) {}
	polygon() = default;
	void add_point(double x, double y) { _points.push_back(point(x, y)); }
	void add_point(point pnt) { _points.push_back(pnt); }
	int size() const { return _points.size(); }
	point get_point(int ind) const { return _points[ind]; }
private:
	std::vector<point> _points;
};

double polar_angle_cos(const point_vector &first, const point_vector &second) {
	double first_abs = sqrt(scalar_mul(first, first));
	double second_abs = sqrt(scalar_mul(second, second));
	double cos = scalar_mul(first, second) / (first_abs * second_abs);
	return cos;
}

void set_counters(const polygon& A, const polygon& B, int &ind_A, int &ind_B) {
	ind_A = 0;
	ind_B = 0;
	point min = A.get_point(0);
	for (int i = 1; i < A.size(); i++) {
		if (A.get_point(i) < min) {
			min = A.get_point(i);
			ind_A = i;
		}
	}
	min = B.get_point(0);
	for (int i = 1; i < B.size(); i++) {
		if (B.get_point(i) < min) {
			min = B.get_point(i);
			ind_B = i;
		}
	}
}

polygon minkowski_sum(const polygon& A, const polygon& B, int ind_A, int ind_B) {
	polygon sum;
	int ind_A_counter = 0;
	int ind_B_counter = 0;
	point_vector direction({ 0.0, 0.0 }, { 1.0, 0.0 });

	while (ind_A_counter < A.size() || ind_B_counter < B.size()) {
		double new_x = A.get_point(ind_A % A.size()).get_x() + B.get_point(ind_B % B.size()).get_x();
		double new_y = A.get_point(ind_A % A.size()).get_y() + B.get_point(ind_B % B.size()).get_y();
		sum.add_point(new_x, new_y);
		double angle_1 = polar_angle_cos(direction, { A.get_point(ind_A % A.size()), A.get_point((ind_A + 1) % A.size()) });
		double angle_2 = polar_angle_cos(direction, { B.get_point(ind_B % B.size()), B.get_point((ind_B + 1) % B.size()) });

		if (angle_1 > angle_2) {
			direction = point_vector({ A.get_point(ind_A % A.size()), A.get_point((ind_A + 1) % A.size()) });
			++ind_A;
			++ind_A_counter;
		}
		else if (angle_1 < angle_2) {
			direction = point_vector({ B.get_point(ind_B % B.size()), B.get_point((ind_B + 1) % B.size()) });
			++ind_B;
			++ind_B_counter;
		}
		else {
			direction = point_vector({ A.get_point(ind_A % A.size()), A.get_point((ind_A + 1) % A.size()) });
			++ind_A;
			++ind_B;
			++ind_A_counter;
			++ind_B_counter;
		}
	}
	return sum;
}

bool check_if_point_in_segment(const point_vector& segment, const point& pnt) {
	point first = segment.first();
	point second = segment.second();
	if (!(pnt.get_x() >= first.get_x() && pnt.get_x() <= second.get_x()
		|| pnt.get_x() <= first.get_x() && pnt.get_x() >= second.get_x()))
		return false;
	if (!(pnt.get_y() >= first.get_y() && pnt.get_y() <= second.get_y()
		|| pnt.get_y() <= first.get_y() && pnt.get_y() >= second.get_y()))
		return false;
	return true;	
}

bool check_if_point_inside(const polygon& poly, const point& pnt) {
	int sign = -2;
	for (int i = 0; i < poly.size(); i++) {
		point_vector pnt_vec = point_vector({ poly.get_point(i), pnt });
		point_vector poly_vec = point_vector({ poly.get_point(i), poly.get_point((i + 1) % poly.size()) });
		int cur_sign = vector_mul_sign(poly_vec, pnt_vec);
		if (cur_sign == 0) {
			if (!check_if_point_in_segment(poly_vec, pnt))
				return false;
		}
		else if (sign == -2)
				sign = cur_sign;
			else if (cur_sign != sign)
					return false;
	}
	return true;
}

bool check_if_intersect(const polygon &A, const polygon &B) {
	polygon minkowski_poly;
	polygon B_inv;
	for (int i = 0; i < B.size(); i++)
		B_inv.add_point(-B.get_point(i).get_x(), -B.get_point(i).get_y());
	int ind_A = 0, ind_B = 0;
	set_counters(A, B_inv, ind_A, ind_B);
	minkowski_poly = minkowski_sum(A, B_inv, ind_A, ind_B);

	
	return check_if_point_inside(minkowski_poly, { 0.0, 0.0 });
}

int main() {
	int n;
	std::cin >> n;
	std::vector<point> temp_vec(n);
	for (int i = n-1; i >= 0; i--) {
		double x, y;
		std::cin >> x >> y;
		temp_vec[i] = point(x, y);
	}
	polygon A(temp_vec);
	temp_vec.clear();

	int m;
	std::cin >> m;
	temp_vec.resize(m);
	for (int i = m-1; i >= 0; i--) {
		double x, y;
		std::cin >> x >> y;
		temp_vec[i] = point(x, y);
	}
	polygon B(temp_vec);
	temp_vec.clear();

	bool intersection = check_if_intersect(A, B);
	if (intersection == true)
		std::cout << "YES" << std::endl;
	else
		std::cout << "NO" << std::endl;
	return 0;
}