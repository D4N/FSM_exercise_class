/* FILE N_BODY.HPP */
#ifndef FILE_N_BODY_HPP
#define FILE_N_BODY_HPP

#include "vector.hpp"
#include "body.hpp"

#include <iostream>
#include <fstream>
#include <vector>

/*
 * Abstract base class for n-body integration.
 *
 * Objects are abstracted via the body class and stored in a std::vector container. You can add new objects using the add_object(body) function
 *
 */
class generic_n_body {
	public:
		// constructor
		generic_n_body (double time, std::string out_file_name);

		void add_object(body object);
		void simulate(double final_time, double time_step, double output_time, bool adaptive_steps);
		void calculate_total_energy();


	protected:
		double time, total_energy = 0., total_energy_previous = 0.;
		std::ofstream output_file;
		std::string output_file_name;
		std::vector<body> body_list;
		std::vector<cartesian_vector> last_acceleration;

		double time_step_correction_factor();
		virtual void step(double time_step) { return; }
		void write_state();
		void calculate_accelerations();
};

/*
 * n-body implementation of the leap frog.
 */
class leapfrog_n_body : public generic_n_body {
	public:
		leapfrog_n_body (double time, std::string out_file_name) : generic_n_body (time, out_file_name) { }

	protected:
		void step(double time_step);
};

/*
 * n-body implementation using rk2.
 */
class rk2_n_body : public generic_n_body {
	public:
		rk2_n_body (double time, std::string out_file_name) : generic_n_body (time, out_file_name) { }

	protected:
		void step(double time_step);
};

/*
 * n-body implementation using rk4.
 */
class rk4_n_body : public generic_n_body {
	public:
		rk4_n_body (double time, std::string out_file_name) : generic_n_body (time, out_file_name) { }

	protected:
		void step(double time_step);
};


#endif /* FILE_N_BODY_HPP */
