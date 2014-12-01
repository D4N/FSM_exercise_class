#include "n-body.hpp"

#include <random>

void one_leapfrog() {
	leapfrog_n_body solver (0., std::string("1_a.dat"));

	solver.add_object(body(cartesian_vector(0., 0., 0.), cartesian_vector(0., 0., 0.), 1.));
	solver.add_object(body(cartesian_vector(1., 0., 0.), cartesian_vector(0., 0.5, 0.), 1e-3));

	solver.simulate(50, 0.01, 0, false);

}

void one_rk() {
	rk2_n_body solver (0., std::string("1_c.dat"));

	solver.add_object(body(cartesian_vector(0., 0., 0.), cartesian_vector(0., 0., 0.), 1.));
	solver.add_object(body(cartesian_vector(1., 0., 0.), cartesian_vector(0., 0.5, 0.), 1e-3));

	solver.simulate(500, 0.001, 0.1, false);

}

void task_b() {
	leapfrog_n_body test_system (0., std::string("task_b.dat"));

	test_system.add_object(body(cartesian_vector(-.5, 0., 0.), cartesian_vector(0., -.5, 0.), 1.));
	test_system.add_object(body(cartesian_vector(.5, 0., 0.), cartesian_vector(0., .5, 0.), 1.));

	test_system.simulate(10, 0.1, 0, false);
}

void task_b_rk2() {
	rk2_n_body test_system (0., std::string("task_b_rk2.dat"));

	test_system.add_object(body(cartesian_vector(-.5, 0., 0.), cartesian_vector(0., -.5, 0.), 1.));
	test_system.add_object(body(cartesian_vector(.5, 0., 0.), cartesian_vector(0., .5, 0.), 1.));

	test_system.simulate(10, 0.001, 0.1, true);
}



void task_c() {
	leapfrog_n_body test_system (0., std::string("task_c.dat"));

	test_system.add_object(body(cartesian_vector(-.5, 0., 0.), cartesian_vector(0., -.5, 0.), 1.));
	test_system.add_object(body(cartesian_vector(.5, 0., 0.), cartesian_vector(0., .5, 0.), 1.));
	test_system.add_object(body(cartesian_vector(1., 6., 2.), cartesian_vector(0., 0., 0.), .1));

	test_system.simulate(50, 0.001, 0.1, false);
}

void task_e() {
	std::random_device rand;

	leapfrog_n_body test_system (0., std::string("task_e.dat"));

	int i = 0;
	while (i<5) {
		cartesian_vector pos = cartesian_vector(rand(), rand(), rand());
		pos = 2. * pos / (double) rand.max() - cartesian_vector(1., 1., 1.);

		// reject points outside the unit sphere
		if (pos.norm_squared() > 1.) { continue; }

		cartesian_vector vel = cartesian_vector(rand(), rand(), rand());
		vel = vel / (5.*rand.max()) - cartesian_vector(.1, .1, .1) ;

		test_system.add_object(body(pos, vel, .1));
		i++;
	}
	test_system.simulate(1000., 0.001, 0.1, true);

}

int main() {

	//one_leapfrog();
	one_rk();

	//task_b_rk2();
	//task_b();
	//task_c();
	//task_e();

	return 0;
}
