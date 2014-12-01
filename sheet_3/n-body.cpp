#include "n-body.hpp"
#include <cmath>
#include <algorithm>
#include <iomanip>


generic_n_body::generic_n_body (double initial_time, std::string out_file_name) {

	time = initial_time;

	// save the file name and clear the output file
	output_file_name = out_file_name;
	output_file.open(output_file_name, std::ios::out | std::ios::trunc);

	output_file << std::setprecision(14);
	output_file.close();
}


void generic_n_body::calculate_total_energy() {
	/*
	 * Total energy calculator. The total energy is stored in the class member total_energy.
	 * The previously calculated total energy is stored in the class member total_energy_previous (this is used for adaptive time stepping).
	 *
	 * E_tot = E_kin + E_pot =
	 *         sum_i=0^N (m_i/2*v_i^2
	 *       - sum_j=0^i m_i*m_i / |x_i - x_j|
	 */

	total_energy_previous = total_energy;

	double E_kin = 0., E_pot = 0.;
	int n = body_list.size();

	// iteration through all bodies, the inner loop only iterates until j=i-1,
	// thereby no potential energies are accounted twice and no division by zero occurs (would occur in the inner loop if i = j)
	for (int i=0; i<n; i++) {
		E_kin += .5*body_list[i].mass* body_list[i].velocity*body_list[i].velocity;

		for (int j=0; j<i; j++) {
			E_pot -= body_list[i].mass * body_list[j].mass / ( body_list[i].position - body_list[j].position).norm();
		}
	}

	total_energy = E_kin + E_pot;

}

void generic_n_body::write_state() {
	/*
	 * Writes the current state to the output file in the following way:
	 * time total energy position1 velocity1 position2 velocity2 ... endl
	 *
	 * The values are separated by spaces.
	 */

	output_file << time << ' ' << total_energy << ' ';

	// fancy way of looping through the body_list vector using an iterator
	for (std::vector<body>::iterator iter = body_list.begin(); iter != body_list.end(); ++iter) {
		output_file << iter->position << ' ' << iter->velocity << ' ';
	}

	output_file << std::endl;
}


void generic_n_body::add_object (body object) {
	/*
	 * Adds a new body to the vector storing all objects.
	 */
	body_list.push_back(object);
}


double generic_n_body::time_step_correction_factor() {
	/*
	 * Returns the correction factor for the time step to adjust it each step.
	 */
	const double relative_threshold = 1e-10;

	calculate_total_energy();


	if ( std::abs( (total_energy - total_energy_previous)/total_energy_previous ) <= relative_threshold ) {
		return 2.;
	}
	else {
		return 0.5;
	}
}


void generic_n_body::calculate_accelerations() {
	/*
	 * Calculates the acceleration due to gravity for each body and stores the vectorial acceleration in a std::vector container.
	 * The accelerations are accessible via the last_acceleration class member
	 */

	// adjust the size to account for recent body additions
	int n = body_list.size();
	last_acceleration.resize(n);


	// double loop over all bodies to calculate the acceleration (i.e. the force)
	for (int i=0; i<n; i++) {
		last_acceleration[i] = cartesian_vector(0, 0, 0);

		for (int j=0; j<n; j++) {

			// ignore this case, as it would cause a division by 0
			if (i==j) { continue; }

			// a_i = sum_j=0^N  ( m_j * (x_j - x_i)/|x_j - x_i|^3 )
			cartesian_vector distance = body_list[j].position - body_list[i].position;
			last_acceleration[i] += (body_list[j].mass/pow(distance.norm_squared(), 1.5)) * distance;

		}
	}
}



void generic_n_body::simulate(double final_time, double time_step, double output_time, bool adaptive_steps) {
	/*
	 * Simulate the system until final_time.
	 * Use time_step as the constant time step if adaptive_steps is false or as the initial guess for the time step if adaptive_steps is true.
	 *
	 * If output_time is non zero, the current state is only written to the file every output_time (the time step is adjusted if necessary).
	 */

	// reopen output file in append mode
	output_file.open(output_file_name, std::ios::out | std::ios::app);
	//std::cout << output_file_name << std::endl;

	calculate_accelerations();

	unsigned int output_counter = 1;
	double dt = time_step; // this is a copy of the time_step, it is required to keep one, so that we can go back to the original time step if it was changed by the if clause that makes sure, that no output steps are missed


	while (time < final_time) {


		// adjust the time step (this implicitly calculates the total energy, therefore it is calculated in the static case explicitly), if necessary
		// for adaptive time steps: limit the time step in the range [1e-10:1]
		if (adaptive_steps) { time_step = std::min(std::max(time_step_correction_factor()*time_step, 1e-10), 1.); }
		else { calculate_total_energy(); time_step = dt; }


		// dump output if the correct time is reached
		if (output_time != 0) {
			// perform the output
			if ( time == output_counter * output_time ) {
				write_state();
				output_counter ++;
			}

			// adjust the time step if necessary, so that no output step is missed
			if ( time + time_step > output_counter * output_time ) { time_step = (output_counter * output_time) - time; }

		}
		else { write_state(); }

		// perform the integration here
		step(time_step);

		time += time_step;
	}

	output_file.close();

}




void leapfrog_n_body::step(double time_step) {

	//std::cout << time_step << std::endl;

	// update positions and first half of velocities
	for (unsigned int i=0; i<body_list.size(); i++) {
		body_list[i].position += body_list[i].velocity * time_step + .5*time_step*time_step * last_acceleration[i];
		body_list[i].velocity += .5*time_step * last_acceleration[i];
	}

	calculate_accelerations();

	// update second half of the velocities
	for (unsigned int i=0; i<body_list.size(); i++) {
		body_list[i].velocity += .5*time_step * last_acceleration[i];
	}
	calculate_accelerations();




}


void rk2_n_body::step(double time_step) {

	unsigned int n = body_list.size();

	//std::cout << time_step << std::endl;

	std::vector<cartesian_vector> temp_position_change; temp_position_change.resize(n);
	std::vector<cartesian_vector> final_position_change; final_position_change.resize(n);

	// update positions
	for (unsigned int i=0; i < n; i++) {

		temp_position_change[i] = .5*time_step * body_list[i].velocity;
		final_position_change[i] = body_list[i].velocity * time_step + .5*time_step*time_step * last_acceleration[i];

		body_list[i].position += temp_position_change[i];

	}

	calculate_accelerations();

	// update second half of the velocities
	for (unsigned int i=0; i < n; i++) {
		body_list[i].velocity += time_step * last_acceleration[i];

		body_list[i].position -= temp_position_change[i];
		body_list[i].position += final_position_change[i];
	}
	calculate_accelerations();
}
