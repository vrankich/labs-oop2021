#include "tests.h"

bool compare(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void test_struct_Point()
{
	/* test constructor */
	heart::Point p1;
	assert(p1.x == 0);
	assert(p1.y == 0);
	heart::Point p2(2.7, -7.2);
	assert(p2.x == 2.7);
	assert(p2.y == -7.2);
	heart::Point p3(-10, 0.01);
	assert(p3.x == -10);
	assert(p3.y == 0.01);
}

void test_initialization()
{
	/* test constructor */
	heart::Cardioid cardioid;
	assert(cardioid.get_r() == 1);
	heart::Cardioid cardioid1(5.123);
	assert(cardioid1.get_r() == 5.123);

	/* test exceptions */
	bool is_thrown = false;
	try {
		cardioid1.set_r(-5.123);
	} catch(std::exception &e) {
		is_thrown = true;
	}
	assert(is_thrown);
	is_thrown = false;
	try {
		heart::Cardioid cardioid2(-10);
	} catch(std::exception &e) {
		is_thrown = true;
	}
	assert(is_thrown);

	/* array of cardioids */
	heart::Cardioid cardioids[5];
	cardioids[0].set_r(10.10);
	cardioids[1].set_r(111);
	cardioids[2].set_r(3);
	cardioids[3].set_r(55.99);
	assert(cardioids[0].get_r() == 10.10);
	assert(cardioids[1].get_r() == 111);
	assert(cardioids[2].get_r() == 3);
	assert(cardioids[3].get_r() == 55.99);
	assert(cardioids[4].get_r() == 1);
} 

void test_polar_distance()
{
	heart::Cardioid cardioid1;
	assert(compare(cardioid1.polar_distance(0), 4, 	EPSILON));
	assert(compare(cardioid1.polar_distance(PI / 6), 3.7320508, 	EPSILON));
	assert(compare(cardioid1.polar_distance(PI / 2), 2, 	EPSILON));
	
	heart::Cardioid cardioid2(27.5);
	assert(compare(cardioid2.polar_distance(0), 110, 	EPSILON));
	assert(compare(cardioid2.polar_distance(PI / 6), 102.6313972, EPSILON));
	assert(compare(cardioid2.polar_distance(PI / 2), 55, EPSILON));

	heart::Cardioid arr[3];
	arr[1].set_r(3);
	arr[2].set_r(0.0001);
	assert(compare(arr[0].polar_distance(PI / 3), 3, 	EPSILON));
	assert(compare(arr[1].polar_distance(PI / 3), 9, 	EPSILON));
	assert(compare(arr[2].polar_distance(PI / 3), 0.0003, EPSILON));
}

void test_most_distant_points()
{
	heart::Point point1, point2;
	double x, y;

	heart::Cardioid cardioid(0.00002);
	cardioid.most_distant_points(point1, point2);
	x = (3.0 / 4) * 0.00002;
	assert(point1.x == x);
	assert(point2.x == x);
	y = 2.5981E-5;
	assert(compare(point1.y, y, EPSILON));
	assert(compare(point2.y, -y, EPSILON));
	
	heart::Cardioid arr[3];
	arr[1].set_r(0.0123);
	arr[2].set_r(150);
	arr[0].most_distant_points(point1, point2);
	x = 3.0 / 4;
	assert(point1.x == x);
	assert(point2.x == x);
	y = 1.2990381;
	assert(compare(point1.y, y, EPSILON));
	assert(compare(point2.y, -y, EPSILON));
	arr[1].most_distant_points(point1, point2);
	x = (3.0 / 4) * 0.0123;
	assert(point1.x == x);
	assert(point2.x == x);
	y = 0.01597817;
	assert(compare(point1.y, y, EPSILON));
	assert(compare(point2.y, -y, EPSILON));
	arr[2].most_distant_points(point1, point2);
	x = (3.0 / 4) * 150;
	assert(point1.x == x);
	assert(point2.x == x);
	y = 194.85571585;
	assert(compare(point1.y, y, EPSILON));
	assert(compare(point2.y, -y, EPSILON));
}

void test_radii_of_curvature()
{
	double rad1, rad2, rad3;
	
	heart::Cardioid cardioid(0.01);
	cardioid.radii_of_curvature(rad1, rad2, rad3);
	assert(compare(rad1, (4.0 / 3) * 0.01, EPSILON));
	assert(rad2 == 0);
	assert(rad3 == 0);

	cardioid.set_r(80.5);
	cardioid.radii_of_curvature(rad1, rad2, rad3);
	assert(compare(rad1, (4.0 / 3) * 80.5, EPSILON));
	assert(rad2 == 0);
	assert(rad3 == 0);

	heart::Cardioid arr[2];
	arr[0].set_r(5.99);
	arr[1].set_r(390);
	arr[0].radii_of_curvature(rad1, rad2, rad3);
	assert(compare(rad1, (4.0 / 3) * 5.99, EPSILON));
	assert(rad2 == 0);
	assert(rad3 == 0);
	arr[1].radii_of_curvature(rad1, rad2, rad3);
	assert(compare(rad1, (4.0 / 3) * 390, EPSILON));
	assert(rad2 == 0);
	assert(rad3 == 0);
}

void test_area()
{
	heart::Cardioid cardioid;
	cardioid.set_r(0.00027);
	assert(compare(cardioid.area(), 3.43533E-07, EPSILON));
	cardioid.set_r(15.01);
	assert(compare(cardioid.area(), 1061.7017, EPSILON_BIG));

	heart::Cardioid arr[3];
	arr[0].set_r(0.0033);
	arr[1].set_r(4.44);
	arr[2].set_r(555);
	assert(compare(arr[0].area(), 5.13179E-05, EPSILON));
	assert(compare(arr[1].area(), 92.8981514, EPSILON));
	assert(compare(arr[2].area(), 1451533.6156, EPSILON_BIG));
}

void test_polar_arc_lenght()
{
	double res;

	heart::Cardioid cardioid(0.0099);
	res = cardioid.arc_lenght(PI / 4);
	
	res = cardioid.arc_lenght((5 * PI) / 6);
	
	res = cardioid.arc_lenght(PI / 2);

}

int call_tests()
{
	test_struct_Point();
	test_initialization();
	test_polar_distance();
	test_most_distant_points();
	test_radii_of_curvature();
	test_area();
	test_polar_arc_lenght();
	std::cout << "\nNo errors found...\n" << std::endl;
	return 0;
}
