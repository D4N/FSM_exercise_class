#-*- coding: utf-8 -*-

from __future__ import division, print_function

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

from mpl_toolkits.mplot3d import Axes3D



class simulation_output(object):

	def __init__(self, filename, update_scale_time = 0, frame_rate = 25):
		"""
		Animation of the output of a n-body simulation.

		Requires the output file to be formated in the following way:

		time total_energy object_1_x object_1_y object_1_z object_1_vx object_1_vy object_1_vz object_2_x ... endl

		Each value has to be interpretable as a float by python and the values have to split by spaces.
		Each line corresponds to a new time step.


		The constructor reads your output file and stores it internally.
		It creates a matplotlib.animation.FuncAnimation and gives you access via the class property animation.
		You can play your animation using the standard matplotlib window via:

		>>> plt.show()

		after constructing a simulation_output object,

		or:
		>>> S = simulation_output('test.file')
		>>> S.animation.save('movie.mkv')

		to directly save the animation as a movie.


		You can specify your class to rescale the 3d plot every n seconds.
		This might be useful to always be able to view the objects at full zoom, if some objects get ejected.

		The time between scale updates is specified in the variable update_scale_time:
			0 - no update, the scale is chosen on construction and is set so that the objects are visible at all times
			n - update every n seconds, where n is a positive integer
			-1 - scale is chosen to be twice the initial space occupied by the particles and stays the same
			-n - raises a ValueError for negative values (beside -1)

			if you specify a float, it will be cast to an integer (and thus truncated)

		"""

		self.__filename = filename
		self.__current_step = 0

		self.__frame_rate = int(frame_rate)


		self.__scale_update = False
		self.__scale_initially = False

		if update_scale_time > 0:
			self.__scale_update = True
			self.__update_frames = self.__frame_rate * int(update_scale_time)

		elif update_scale_time < 0 and not update_scale_time == -1:
			raise ValueError("Update time of the scale has to be positive")

		elif update_scale_time == -1:
			self.__scale_initially = True

		self.get_data_from_file()

		self.fig = plt.figure()
		self.ax = self.fig.add_subplot(111, projection='3d')


		#self.animation = animation.FuncAnimation(self.fig, self.update, frames = len(self.__time) - 2, interval = self.__frame_rate, init_func = self.setup_plot)
		self.animation = animation.FuncAnimation(self.fig, self.update, frames = len(self.__time) - 1, interval = 1000./self.__frame_rate, init_func = self.setup_plot)



	def setup_plot(self):

		self.lines = self.ax.plot(self.__x[0], self.__y[0], self.__z[0], 'o', color = 'green' )
		self.ax.set_xlabel('x')
		self.ax.set_ylabel('y')
		self.ax.set_zlabel('z')

		self.text = self.fig.text(.1, .1, "t = " + str(self.__time[self.__current_step]) + ", energy: " + str(self.__energy[self.__current_step]))

		self.scale_update()

		return self.lines


	def scale_update(self):
		if self.__scale_update:

			current_step = int(self.__current_step/self.__update_frames)

			self.ax.set_xlim3d(np.min(np.array(self.__x[current_step*self.__update_frames:(current_step+1)*self.__update_frames])), np.max(np.array(self.__x[current_step*self.__update_frames:(current_step+1)*self.__update_frames])))
			self.ax.set_ylim3d(np.min(np.array(self.__y[current_step*self.__update_frames:(current_step+1)*self.__update_frames])), np.max(np.array(self.__y[current_step*self.__update_frames:(current_step+1)*self.__update_frames])))
			self.ax.set_zlim3d(np.min(np.array(self.__z[current_step*self.__update_frames:(current_step+1)*self.__update_frames])), np.max(np.array(self.__z[current_step*self.__update_frames:(current_step+1)*self.__update_frames])))

		else:
			if self.__scale_initially:
				self.ax.set_xlim3d(np.min(np.array(self.__x[0])), np.max(np.array(self.__x[0])))
				self.ax.set_ylim3d(np.min(np.array(self.__y[0])), np.max(np.array(self.__y[0])))
				self.ax.set_zlim3d(np.min(np.array(self.__z[0])), np.max(np.array(self.__z[0])))
			else:
				self.ax.set_xlim3d(np.min(np.array(self.__x)), np.max(np.array(self.__x)))
				self.ax.set_ylim3d(np.min(np.array(self.__y)), np.max(np.array(self.__y)))
				self.ax.set_zlim3d(np.min(np.array(self.__z)), np.max(np.array(self.__z)))





	def get_data_from_file(self):

		time = []
		energy = []
		x = []
		y = []
		z = []

		#with open(self.__filename, 'r') as data:
		#

		with open(self.__filename, 'r') as data:
			for line in data:
				tmp = line.split(' ')

				time.append(float(tmp[0]))
				energy.append(float(tmp[1]))

				obj_count = int((len(tmp) - 2)/6)
				x_tmp = np.zeros((obj_count))
				y_tmp = np.zeros((obj_count))
				z_tmp = np.zeros((obj_count))

				for i in xrange(obj_count):
					x_tmp[i] = tmp[2+6*i]
					y_tmp[i] = tmp[3+6*i]
					z_tmp[i] = tmp[4+6*i]

				x.append(x_tmp)
				y.append(y_tmp)
				z.append(z_tmp)

		self.__time = time
		self.__energy = energy
		self.__x = x
		self.__y = y
		self.__z = z


	def update(self, i):
		self.__current_step += 1

		#print(self.__current_step, self.__update_frames,  self.__current_step % self.__update_frames)

		# nasty way to avoid an overflow
		if self.__current_step == len(self.__time):
			exit(0)

		self.lines.pop(0).remove()
		self.lines = self.ax.plot(self.__x[self.__current_step], self.__y[self.__current_step], self.__z[self.__current_step], 'o', color = 'green')

		self.scale_update()

		self.text.set_text("t = " + str(self.__time[self.__current_step]) + ", energy: " + str(self.__energy[self.__current_step]))

		plt.draw()

		return self.lines


if __name__ == '__main__':

	import sys

	try:
		data_file_name = sys.argv[1]
		rescale_behavior = sys.argv[2]
	except:
		raise ValueError("Usage:\npython2.7 plotter.py data_file rescale_behavior (rescale_time) movie_output_name")

	update_time = 0

	if rescale_behavior == "initially":
		update_time = -1
	elif rescale_behavior == "repeat":
		update_time = sys.argv[3]
	elif rescale_behavior == "global":
		update_time = 0
	else:
		raise ValueError("Wrong scalling behavior specified, possibilities are: initially, repeat, global")

	try:
		if rescale_behavior == "repeat":
			movie = str(sys.argv[4])
		else:
			movie = str(sys.argv[3])
	except:
		movie = False


	anim = simulation_output(data_file_name, update_scale_time = update_time)

	if bool(movie):
		anim.animation.save(movie, fps=25)
	else:
		plt.show()
