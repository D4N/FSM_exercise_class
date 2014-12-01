#-*- coding: utf-8 -*-

from __future__ import division, print_function

import numpy as np
import scipy.linalg as lin

#import matplotlib.pyplot as plt


class point(object):

	def __init__(self, position, label):
		"""
		Abstraction of a point in 2D space.

		>>> p = point([1, 1], "A")
		>>> p.label
		'A'
		>>> p.position
		array([ 1.,  1.])

		If position is not a 2d position, an exception is thrown:
		>>> p = point(1, "A")
		Traceback (most recent call last):
		 ...
		ValueError: The point should be in 2D

		The provided position has to be convertible to a numpy array:
		>>> p = point("not an array", "A")
		Traceback (most recent call last):
		 ...
		ValueError: Provided position cannot be converted to a numpy array.

		"""

		self.__label = str(label)

		self.position = position


	@property
	def position(self):
		return self.__position

	@position.setter
	def position(self, new_value):
		try:
			self.__position = np.array(new_value, dtype=float)
		except:
			raise ValueError("Provided position cannot be converted to a numpy array.")

		if not self.__position.shape == (2,):
			raise ValueError("The point should be in 2D")

	@property
	def label(self):
		"""
		Read-only identifier of the point, can be used e.g. for plotting.
		Cannot be changed once set.

		>>> p = point([1, 1], "funny point")
		>>> p.label
		'funny point'
		>>> p.label = "other"
		Traceback (most recent call last):
		 ...
		AttributeError: can't set attribute
		"""
		return self.__label


class rod(object):

	def __init__(self, point_1, point_2, Youngs_modulus = 1., radius = 1.):
		"""
		Abstraction of a rod for finite element methods in 2D.

		Parameters:
		* point_1, point_2 - members of the point class, rod is constructed between both points
		* Youngs_modulus - Young's modulus of the rod
		* radius - radius of the rod

		The supplied parameters can be accessed via properties with the same name as the respective keywords:

		>>> p1 = point([0,0], "A")
		>>> p2 = point([1,0], "B")
		>>> R = rod(p1, p2, Youngs_modulus = 10., radius = .1)
		>>> R.point_1 == p1
		True
		>>> R.point_2 == p2
		True
		>>> R.Youngs_modulus
		10.0
		>>> R.radius
		0.1

		The rod constructor automatically calculates:
		* rod length (accessible via length)
		* angle between the rod and the x axis (accessible via theta)
		* the stiffness coefficient K (accessible via stiffness_coefficient)

		>>> p1 = np.random.rand((2))
		>>> p2 = np.random.rand((2))
		>>> R = rod(point(p1, "A"), point(p2, "B"), 1, 1)
		>>> l = np.sqrt( np.sum( (p1 - p2)**2 ) )
		>>> round((l-R.length)/l, 12)
		0.0
		>>> theta = np.arctan2(p2[1]-p1[1], p2[0]-p1[0])
		>>> np.abs(round((theta-R.theta)/theta, 12))
		0.0
		>>> K = np.pi/l
		>>> np.abs(round((K - R.stiffness_coefficient)/K, 10))
		0.0

		The actually useful part of the class is the function rod_matrix_block.
		It computes the following matrix:
		K_AB = K * ( cos^2 theta		cos theta*sin theta )
		           ( cos theta*sin theta	sin^2 theta         )

		This matrix block is needed to construct the master stiffness matrix.

		>>> R = rod(point([0, 0], "A"), point([1, 0], "B"))
		>>> R.theta
		0.0
		>>> round(R.stiffness_coefficient, 5)
		3.14159
		>>> np.round(R.rod_matrix_block(), 5)
		array([[ 3.14159,  0.     ],
		       [ 0.     ,  0.     ]])

		At last you can re-compute the angle, length and stiffness coefficient or only the angle using the function recompute_rod_properties.
		"""

		self.__point_1 = point_1
		self.__point_2 = point_2

		self.__E = float(Youngs_modulus)
		self.__r = float(radius)

		self.recompute_rod_properties(initial_run = True)


	def recompute_rod_properties(self, initial_run = False):
		"""
		Recomputes the angle, length and stiffness coefficient of the rod if the positions have changed.
		If you set initial_run to False, only the angle is adjusted (this is used for the iteration of the finite element system).
		"""

		self.__theta = np.arctan2( self.__point_2.position[1] - self.__point_1.position[1], self.__point_2.position[0] - self.__point_1.position[0])

		if initial_run:
			self.__length = np.sqrt( np.sum( (self.__point_1.position - self.__point_2.position)**2 ) )
			self.__K = self.__E*np.pi*self.__r**2 / self.__length


	def rod_matrix_block(self):
		"""
		Returns the matrix with the following components:

		K_AB = K * ( cos^2 theta		cos theta*sin theta )
		           ( cos theta*sin theta	sin^2 theta         )
		"""
		mat = np.matrix(np.zeros((2,2)))

		mat[0,0] = (np.cos(self.__theta))**2
		mat[1,1] = (np.sin(self.__theta))**2
		mat[1,0] = np.cos(self.__theta) * np.sin(self.__theta)
		mat[0,1] = np.cos(self.__theta) * np.sin(self.__theta)

		mat *= self.__K

		return mat


	@property
	def Youngs_modulus(self):
		return self.__E

	@property
	def length(self):
		return self.__length

	@property
	def point_1(self):
		return self.__point_1

	@property
	def point_2(self):
		return self.__point_2

	@property
	def theta(self):
		return self.__theta

	@property
	def stiffness_coefficient(self):
		return self.__K

	@property
	def radius(self):
		return self.__r



class finite_element_system(object):

	def __init__(self, rods):
		"""
		Abstraction of a finite difference system. You just have to supply a list of rods that connect some points.

		Important! The points have to be the same python objects, not just having the same coordinates.

		>>> p1 = point([0,0], "A")
		>>> p2 = point([1,0], "B")
		>>> p3 = point([0,1], "C")
		>>> rod1 = rod(p1, p2)
		>>> rod2 = rod(p2, p3)
		>>> rod3 = rod(p3, p1)
		>>> F = finite_element_system([rod1, rod2, rod3])
		>>> for point in F.points:
		...	print(point.label)
		A
		B
		C
		"""

		self.__rods = rods

		self.__points = []

		for rod in rods:
			tmp1 = rod.point_1
			tmp2 = rod.point_2

			if not tmp1 in self.__points:
				self.__points.append(tmp1)

			if not tmp2 in self.__points:
				self.__points.append(tmp2)

		self.__mat = None
		self.__forces = np.zeros( (2*len(self.__points)) )
		self.__new_x = np.zeros( (2*len(self.__points)) )
		self.__last_x = None

		self.__convergence = None

		self.__boundary_conditions = []


	def __find_rod_2_points(self, point1, point2):
		"""
		Returns the rod which is between point1 and point2.
		"""

		for rod in self.__rods:
			if (point1 == rod.point_1) and (point2 == rod.point_2):
				return rod
			elif (point1 == rod.point_2) and (point2 == rod.point_1):
				return rod


	def __find_rod_1_point(self, point):
		"""
		Returns the list of rods which are connected to point.
		"""

		result = []

		for rod in self.__rods:
			if (point == rod.point_1) or (point == rod.point_2):
				result.append(rod)

		return result


	def calc_stiffness_matrix(self):
		"""
		Recalculate the master stiffness matrix.
		"""

		n = len(self.__points)

		self.__mat = np.matrix(np.zeros((2*n,2*n)))

		for i in xrange(n):

			for j in xrange(n):
				tmp_mat = np.matrix(np.zeros((2,2)))
				if i==j:
					for rod in self.__find_rod_1_point(self.__points[i]):
						tmp_mat += rod.rod_matrix_block()

				else:
					current_rod = self.__find_rod_2_points(self.__points[i], self.__points[j])
					if not current_rod == None:
						tmp_mat = -1.*current_rod.rod_matrix_block()

				self.__mat[2*i,   2*j]   = tmp_mat[0,0]
				self.__mat[2*i+1, 2*j+1] = tmp_mat[1,1]
				self.__mat[2*i+1, 2*j]   = tmp_mat[1,0]
				self.__mat[2*i,   2*j+1] = tmp_mat[0,1]

			if 2*i in self.__boundary_conditions:
				self.__mat[2*i,:] = 0.
				self.__mat[2*i,2*i] = 1.

			if 2*i+1 in self.__boundary_conditions:
				self.__mat[2*i+1,:] = 0.
				self.__mat[2*i+1,2*i+1] = 1.




	def set_boundary_condition(self, fix_point, fix_coordinates):
		"""
		Abstraction of boundary conditions applied to the finite difference problem.

		The line corresponding to the given point and the specified coordinate(s) is zeroed.

		fix_point - the point which should not move any more, a point object is expected
		fix_coordinates - string, either "x", "y" or "xy"
		"""

		coordinate_dict = {"x": [0], "y": [1], "xy": [0, 1]}

		tmp = str(fix_coordinates)
		if not (tmp in coordinate_dict.keys()):
			raise ValueError("Invalid coordinate to fix specified, possible values are: x, y, xy. You specified: " + tmp)

		n = self.__points.index(fix_point)

		for coord in coordinate_dict[tmp]:
			if not (2*n + coord) in self.__boundary_conditions:
				self.__boundary_conditions.append(2*n + coord)

			self.__forces[2*n + coord] = 0


	def fix_force(self, point, coordinate, force_magnitude):
		"""
		Sets the force at the given point in the direction of coordinate to the value given in force_magnitude.
		"""
		coordinate_dict = {"x": 0, "y": 1}

		tmp = str(coordinate)
		if not (tmp in coordinate_dict.keys()):
			raise ValueError("Invalid coordinate to fix the force specified, possible values are: x and y. You specified: " + tmp)

		n = self.__points.index(point)

		self.__forces[2*n + coordinate_dict[tmp]] = force_magnitude


	def iteration_step(self):
		"""
		Perform an iteration step.
		"""

		self.calc_stiffness_matrix()

		# solve the equations using the lu-decomposition
		lu, piv = lin.lu_factor(self.__mat)

		self.__last_x = self.__new_x
		self.__new_x = lin.lu_solve((lu, piv), self.__forces)

		# update point positions to the new d_x
		for i in xrange(len(self.__points)):
			pos_difference_new = np.array([self.__new_x[2*i], self.__new_x[2*i+1]])
			pos_difference_last = np.array([self.__last_x[2*i], self.__last_x[2*i+1]])

			self.__points[i].position -= pos_difference_last
			self.__points[i].position += pos_difference_new


		# update rod properties
		for rod in self.__rods:
			rod.recompute_rod_properties()



	def convergence_reached(self):
		"""
		Returns True if the maximal normalized difference of the displacement between two steps: max(|last - new/new|) is smaller than the class property convergence_threshold.
		"""
		# catch the first run
		if self.__last_x == None:
			return False

		if self.__convergence == None:
			raise ValueError("You have to provide a convergence threshold.")

		diff = np.abs( (self.__last_x - self.__new_x)/self.__new_x )
		if np.max(diff[np.isfinite(diff)]) < self.__convergence:
			return True
		else:
			return False




	def iteration(self):

		n = 0
		while self.convergence_reached():

			self.iteration_step()
			n += 1

		return n


	@property
	def points(self):
		return self.__points

	@property
	def rods(self):
		return self.__rods

	@property
	def master_stiffness_matrix(self):
		return self.__mat

	@property
	def force_vector(self):
		return self.__forces

	@property
	def displacement_vector(self):
		return self.__new_x

	@property
	def convergence_threshold(self):
		return self.__convergence

	@convergence_threshold.setter
	def convergence_threshold(self, new_value):
		try:
			self.__convergence = float(new_value)
		except:
			raise ValueError("The convergence threshold has to be a float.")



class iteration_controller(object):

	def __init__(self, finite_difference_system):

		self.__system = finite_difference_system

	def iterate_until_convergence(self, convergence_threshold = 1e-6, max_step_count = 1000, plot_steps = True):

		if plot_steps:
			import matplotlib.pyplot as plt

		self.__system.convergence_threshold = convergence_threshold
		step_count = 0

		self.plot_current_state(step = step_count)

		while not self.__system.convergence_reached():

			if step_count == max_step_count - 1:
				break


			self.__system.iteration_step()
			step_count += 1

			if plot_steps:
				self.plot_current_state(step = step_count)

		if plot_steps:
			plt.legend()
			plt.show()
			plt.close()

		return step_count

	def plot_current_state(self, step):

		import matplotlib.pyplot as plt

		line = plt.plot([self.__system.rods[0].point_1.position[0], self.__system.rods[0].point_2.position[0]], [self.__system.rods[0].point_1.position[1], self.__system.rods[0].point_2.position[1]], '-', label = "step "+str(step))
		color = line[0].get_color()

		for rod in self.__system.rods:
			plt.plot([rod.point_1.position[0], rod.point_2.position[0]], [rod.point_1.position[1], rod.point_2.position[1]], color + '-')



def wheel():
	middle = point([0, 0], "mid")

	outer = []
	for phi in np.linspace(0, 2*np.pi, 12, endpoint = False):
		outer.append(point([np.cos(phi), np.sin(phi)], "%(number).2f" % {"number": phi}))

	rods = []

	kwargs = { "Youngs_modulus": 1e3, "radius": 0.1 }

	for i in xrange(len(outer)-1):
		rods.append(rod(middle, outer[i], **kwargs))
		rods.append(rod(outer[i], outer[i+1], **kwargs))

	rods.append(rod(outer[0], outer[-1], **kwargs))
	rods.append(rod(outer[-1], middle, **kwargs))

	F = finite_element_system(rods)

	np.set_printoptions(threshold = 100000, linewidth = 200, suppress = False)
	F.calc_stiffness_matrix()

	print(F.master_stiffness_matrix)
	print("non-zero elements:", np.count_nonzero(F.master_stiffness_matrix))
	(m,n) = F.master_stiffness_matrix.shape
	print("element count:", m*n)


	for current_point in [middle]+outer:
		#F.set_boundary_condition(current_point, "x")
		F.fix_force(current_point, "y", -1.)

	F.set_boundary_condition(outer[-2], "xy")
	F.set_boundary_condition(outer[-3], "xy")
	F.set_boundary_condition(outer[-4], "xy")

	print("boundary conditions applied")

	F.calc_stiffness_matrix()

	print("non-zero elements:", np.count_nonzero(F.master_stiffness_matrix))
	(m,n) = F.master_stiffness_matrix.shape
	print("element count:", m*n)


	controller = iteration_controller(F)
	controller.iterate_until_convergence(max_step_count = 2)



def bridge():

	pillar_1 = []
	pillar_2 = []

	for i in xrange(-1, 3):
		pillar_1.append(point([2, i], "pillar1"))
		pillar_2.append(point([8, i], "pillar2"))

	street = []
	l = range(11)
	l.remove(2)
	l.remove(8)

	for i in l:
		street.append(point([i, 0], "street"))


	street_kwargs = {"radius": 0.1, "Youngs_modulus": 1e4}
	concrete_kwargs = {"radius": 0.1, "Youngs_modulus": 1e4}
	rope_kwargs = {"radius": 0.1, "Youngs_modulus": 1e3}


	rods = []
	for j in xrange(len(street)):

		for i in [2, 3]:
			if j < len(street) - 2:
				rods.append(rod(street[j], pillar_1[i], **rope_kwargs))
			if j > 1:
				rods.append(rod(street[j], pillar_2[i], **rope_kwargs))

	for i in xrange(len(pillar_1)-1):
		rods.append(rod(pillar_1[i], pillar_1[i+1], **concrete_kwargs))
		rods.append(rod(pillar_2[i], pillar_2[i+1], **concrete_kwargs))

	for i in xrange(2, 6):
		rods.append(rod(street[i], street[i+1], **street_kwargs))

	rods.append(rod(street[0], street[1], **street_kwargs))
	rods.append(rod(street[-1], street[-2], **street_kwargs))

	rods.append(rod(street[1], pillar_1[1], **street_kwargs))
	rods.append(rod(street[-2], pillar_2[1], **street_kwargs))

	rods.append(rod(pillar_1[1], street[2], **street_kwargs))
	rods.append(rod(pillar_2[1], street[-3], **street_kwargs))


	F = finite_element_system(rods)

	for current_point in street + pillar_1 + pillar_2:
		F.fix_force(current_point, "y", -1.)

	for fixed_point in [street[0], street[-1], pillar_1[0], pillar_2[0]]:
		F.set_boundary_condition(fixed_point, "xy")

	F.calc_stiffness_matrix()

	print("non-zero elements:", np.count_nonzero(F.master_stiffness_matrix))
	(m,n) = F.master_stiffness_matrix.shape
	print("element count:", m*n)


	controller = iteration_controller(F)
	controller.iterate_until_convergence(max_step_count = 2)


def long_bridge(length = 100):

	upper = []
	lower = []

	for i in xrange(length):
		upper.append(point([i, 1], "A"))
		lower.append(point([i, 0], "A"))

	rods = []

	for i in xrange(length-1):
		rods.append(rod(lower[i], upper[i]))
		rods.append(rod(upper[i], upper[i]))
		rods.append(rod(lower[i], lower[i]))

	F = finite_element_system(rods)
	F.calc_stiffness_matrix()

	print("non-zero elements:", np.count_nonzero(F.master_stiffness_matrix))
	(m,n) = F.master_stiffness_matrix.shape
	print("element count:", m*n)



def task_1():
	p1 = point([0.,0.], "A")
	p2 = point([1.,0.], "B")
	p3 = point([0.,.5], "C")
	rod1 = rod(p1, p2, radius = 0.1, Youngs_modulus = 1e3)
	rod2 = rod(p2, p3, radius = 0.1, Youngs_modulus = 1e3)
	rod3 = rod(p3, p1, radius = 0.1, Youngs_modulus = 1e3)

	F = finite_element_system([rod1, rod2, rod3])

	F.set_boundary_condition(p1, "xy")
	F.set_boundary_condition(p3, "x")

	F.fix_force(p2, "y", -1.)

	F.calc_stiffness_matrix()
	print(F.master_stiffness_matrix/rod1.stiffness_coefficient)

	F.iteration_step()
	print(F.displacement_vector)

	#controller = iteration_controller(F)
	#controller.iterate_until_convergence(plot_steps = False)

	for pt in F.points:
		print(pt.position)


if __name__ == '__main__':

	task_1()

	#wheel()

	#bridge()

	#long_bridge(500)

