# -*- coding: utf-8 -*-

from __future__ import division, print_function

import numpy as np


columns = np.random.randint(5, 15)

means = -10. * np.random.random((columns)) + 5.

samples = np.random.randint(1000, 2000)

values = np.random.normal(loc = means, size = (samples, columns))

print("#\t"+''.join([str(mean) + '\t' for mean in means]))

for i in xrange(samples):
	print(str(i) + '\t', end='')
	print(''.join([ str(value) + '\t' for value in values[i]]))
