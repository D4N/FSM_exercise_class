#-*- coding: utf-8 -*-

from __future__ import division, print_function

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


class simulation_output(object):

	def __init__(self, filename):

		self.__filename = str(filename)

		self.get_data_from_file()


	def get_data_from_file(self):

		self.__data_count = sum(1 for line in open(self.__filename))

		self.__time = np.zeros((self.__data_count))
		self.__energy = np.zeros((self.__data_count))

		with open(self.__filename, 'r') as data:
			first_line = data.readline()

			tmp = first_line.split(' ')
			self.__object_count = int((len(tmp) - 2)/6)

		self.__x = np.zeros((self.__object_count, self.__data_count))
		self.__y = np.zeros((self.__object_count, self.__data_count))
		self.__z = np.zeros((self.__object_count, self.__data_count))


		with open(self.__filename, 'r') as data:
			j = 0
			for line in data:
				tmp = line.split(' ')

				self.__time[j] = float(tmp[0])
				self.__energy[j] = float(tmp[1])

				for i in xrange(self.__object_count):
					self.__x[i,j] = float(tmp[2+6*i])
					self.__y[i,j] = float(tmp[3+6*i])
					self.__z[i,j] = float(tmp[4+6*i])

				j += 1


	def plot_data(self, plot_type = "xy"):

		if not plot_type in ["xy", "yz", "xz", "xyz", "energy"]:
			raise ValueError("Possible values for the plot_type are: xy, yz, xz, xyz and energy")

		self.fig = plt.figure()
		if plot_type == "xyz":
			self.ax = self.fig.add_subplot(111, projection='3d')
		else:
			self.ax = self.fig.add_subplot(111)


		if plot_type == "xy":
			for i in xrange(self.__object_count):
				self.ax.plot(self.__x[i], self.__y[i])
		elif plot_type == "yz":
			for i in xrange(self.__object_count):
				self.ax.plot(self.__y[i], self.__z[i])
		elif plot_type == "xz":
			for i in xrange(self.__object_count):
				self.ax.plot(self.__x[i], self.__z[i])
		elif plot_type == "xyz":
			for i in xrange(self.__object_count):
				self.ax.plot(self.__x[i], self.__y[i], self.__z[i])

		elif plot_type == "energy":
			self.ax.plot(self.__time, self.__energy)


		self.ax.set_xlabel(plot_type[0])
		self.ax.set_ylabel(plot_type[1])

		if plot_type == "xyz":
			self.ax.set_zlabel("z")
		elif plot_type == "energy":
			self.ax.set_xlabel("t")
			self.ax.set_ylabel(r"$E_{tot}$")

		if not plot_type == "xyz":
			plt.grid()

		plt.show()
		plt.close()


if __name__ == '__main__':

	import sys

	S = simulation_output(sys.argv[1])
	S.plot_data(sys.argv[2])
