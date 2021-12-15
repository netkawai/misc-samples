'''
sine
'''

import numpy as np
import matplotlib.pyplot as plt
z = np.arange(-8, 8, .1)
t = np.sin(z)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(z,t)
ax.set_ylim([-1.0,1.0])
ax.set_xlim([-8,8])
ax.grid(True)
ax.set_xlabel('z')
ax.set_title('sine function')

plt.show()
