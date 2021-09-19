#include "tests.h"

void test_initialization()
{
	/* test constructor */
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
	heart::Cardioid cardioid(27.5);
	double res = cardioid.polar_distance(2.355);
	assert(round(res * 10000) / 10000 == 16.1556);
	assert(cardioid.polar_distance(0) == 110);
	assert(round(cardioid.polar_distance(M_PI)) == 0);
	assert(round(cardioid.polar_distance(2 * M_PI)) == 110);
}

void test_most_distant_points()
{
	heart::Cardioid cardioid(3.50);
	heart::MostDistantPoints points = cardioid.most_distant_points();
	assert(points.point1.x == (3 * 3.50) / 2);
	assert(points.point1.y == 0);
	assert(points.point2.x == - (3 * 3.50) / 2);
	assert(points.point2.y == 0);
}

void test_r_of_curvature()
{
	heart::Cardioid cardioid(10);
	heart::Radius *radii = cardioid.r_of_curvature();
	assert(radii[0].angle == 0);
	assert(radii[0].r == 0);
	assert(radii[1].angle == M_PI / 6);
	assert(radii[1].r == (8 / 3) * 10 * sin(M_PI / 12));
	assert(radii[2].angle == M_PI / 4);
	assert(radii[2].r == (8 / 3) * 10 * sin(M_PI / 8));
	assert(radii[3].angle == M_PI / 3);
	assert(radii[3].r == (8 / 3) * 10 * sin(M_PI / 6));
	delete [] radii;
}

void test_area()
{
	heart::Cardioid cardioid;
	assert(round(cardioid.area() * 1000) / 1000 == 18.850);
	cardioid.set_r(15.01);
	assert(round(cardioid.area() * 1000) / 1000 == 282.932);

	heart::Cardioid cardioids[3];
	cardioids[0].set_r(3.333);
	cardioids[1].set_r(4.4444);
	cardioids[2].set_r(5.55555);
	assert(round(cardioids[0].area() * 100) / 100 == 62.83);
	assert(round(cardioids[1].area() * 100) / 100 == 83.77);
	assert(round(cardioids[2].area() * 100) / 100 == 104.72);
}

void test_polar_arc_lenght()
{
	heart::Cardioid cardioid(0.99);
	double res;
	res = cardioid.polar_arc_lenght(M_PI / 4);
	assert(round(res * 10000) / 10000 == 1.3589);
	res = cardioid.polar_arc_lenght((5 * M_PI) / 6);
	assert(round(res * 10000) / 10000 == 55.1557);
	res = cardioid.polar_arc_lenght(M_PI / 2);
	assert(round(res * 10000) / 10000 == 15.8400);
}

void call_tests()
{
	test_initialization();
	test_polar_distance();
	test_most_distant_points();
	test_r_of_curvature();
	test_area();
	test_polar_arc_lenght();
	std::cout << "\nNo errors found...\n" << std::endl;
}
