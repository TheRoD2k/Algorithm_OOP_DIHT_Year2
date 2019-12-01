#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <float.h>

class point {
public:
	point(double x, double y, double z) : _x(x), _y(y), _z(z) {};
	point() = default;
	double get_x() { return _x; }
	double get_y() { return _y; }
	double get_z() { return _z; }
private:
	double _x;
	double _y;
	double _z;
};

class segment {
public:
	double size() { return _sz; }
	segment(point point1, point point2) : _p1(point1), _p2(point2) { _calculate_size(); };
	point get_first_point() { return _p1; }
	point get_second_point() { return _p2; }
private:
	void _calculate_size();
	double _sz;
	point _p1;
	point _p2;
};

void segment::_calculate_size() {
	double dx = _p2.get_x() - _p1.get_x();
	double dy = _p2.get_y() - _p1.get_y();
	double dz = _p2.get_z() - _p1.get_z();
	_sz = sqrt(dx * dx + dy * dy + dz * dz);
}

point get_parameterized_point(segment seg, double param) {
	double x1 = seg.get_first_point().get_x();
	double x2 = seg.get_second_point().get_x();
	double y1 = seg.get_first_point().get_y();
	double y2 = seg.get_second_point().get_y();
	double z1 = seg.get_first_point().get_z();
	double z2 = seg.get_second_point().get_z();

	double diff_x = x2 - x1;
	double diff_y = y2 - y1;
	double diff_z = z2 - z1;
	double new_x = x1 + param * diff_x;
	double new_y = y1 + param * diff_y;
	double new_z = z1 + param * diff_z;
	return point(new_x, new_y, new_z);
}

double get_distance(point point1, point point2) {
	double dx = point2.get_x() - point1.get_x();
	double dy = point2.get_y() - point1.get_y();
	double dz = point2.get_z() - point1.get_z();
	return sqrt(dx * dx + dy * dy + dz * dz);
}

template <typename T, typename U, typename FUNC>
double ternar_search(T left, U right, bool min_search, FUNC function) {
	double sigma = 0.00000001;
	double left_param = 0;
	double right_param = 1;
	while (right_param - left_param > sigma) {
		bool first_better;
		if (min_search)
			first_better = function(left_param) < function(right_param);
		else
			first_better = function(left_param) > function(right_param);

		if (first_better)
			right_param = right_param - (right_param - left_param) / 3;
		else 
			left_param = left_param + (right_param - left_param) / 3;
	}
	return left_param;
}

double get_minimum_distance(segment seg1, segment seg2) {
	double min_val = DBL_MAX;
	auto calculate_distance_segments = [&seg1, &seg2, &min_val](double param) -> double {
		point pnt = get_parameterized_point(seg1, param);
		double distance;
		auto calculate_distance_segment_point = [&pnt, &seg2, &min_val, &distance](double inner_param) -> double {
			point inner_pnt = get_parameterized_point(seg2, inner_param);
			distance = get_distance(pnt, inner_pnt);
			if (distance < min_val)
				min_val = distance;
			return distance;
		};
		ternar_search(pnt, seg2, true, calculate_distance_segment_point);
		return distance;
	};

	ternar_search(seg1, seg2, true, calculate_distance_segments);
	return min_val;
}

int main() {
	double x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3,
		x4, y4, z4;
	std::cin >> x1 >> y1 >> z1
		>> x2 >> y2 >> z2
		>> x3 >> y3 >> z3
		>> x4 >> y4 >> z4;
	segment seg1(point(x1, y1, z1), point(x2, y2, z2));
	segment seg2(point(x3, y3, z3), point(x4, y4, z4));
	std::cout << std::fixed << std::setprecision(7) << get_minimum_distance(seg1, seg2) << std::endl;
}