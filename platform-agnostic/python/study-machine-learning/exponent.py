'''
exponent
'''

import numpy as np
import matplotlib.pyplot as plt
z = np.arange(-5, 5, .1)
t = np.exp(z)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(z,t)
ax.set_ylim([0,5.0])
ax.set_xlim([-5,5])
ax.grid(True)
ax.set_xlabel('z')
ax.set_title('exponent function')

plt.show()
