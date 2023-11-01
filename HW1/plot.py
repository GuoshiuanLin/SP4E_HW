import numpy as np
from scipy.sparse import linalg
from scipy.optimize import minimize
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import axes3d
from optimizer import quadratic, A, b

# initialize x, y, and z for plotting the background quadratic function
x_axis = np.linspace(-3,3,100)
y_axis = np.linspace(-3,3,100)
z_axis = np.zeros((len(x_axis), len(y_axis)))
for i, x in enumerate(x_axis):
    for j, y in enumerate(y_axis):
        z_axis[i, j] = quadratic(np.array([x,y]))

# create 2-D coordinates  
x_axis, y_axis = np.meshgrid(x_axis, y_axis, indexing='ij')

# initialize lists to store the parameters x, y and result z during the optimization (BFGS)
x = []
y = []
z = []
# define a function to store the parameters and the result during the optimization
def callback_function(params):
    x.append(params[0]), y.append(params[1]), z.append(quadratic(params)[0])

# running the scipy BFGS slover with initial guess [3, -3]    
result = minimize(quadratic, [3,-3], method='BFGS', callback=callback_function)
# plotting
fig = plt.figure(figsize=plt.figaspect(0.5))
# first plot (BFGS)
ax = fig.add_subplot(1,2,1,projection='3d')
ax.plot_surface(x_axis, y_axis, z_axis, cmap='viridis', alpha=0.4) 
ax.contour(x_axis, y_axis, z_axis, colors='k')  
ax.set_xlabel('x'), ax.set_ylabel('y'), ax.set_zlabel('S(x)')
ax.view_init(azim=130, elev=50)
ax.plot(x, y, z, 'r-')
ax.scatter(x, y, z, color='r', marker='o')
ax.scatter(x[-1], y[-1], z[-1], color='r', marker='o')
ax.set_title(f'BFGS (x,y)=({x[-1]:.3f},{y[-1]:.3f}), z={z[-1]:.3f}')


# initialize lists to store the parameters during the optimization (LGMRES)
x = []
y = []
z = []
# running the scipy LGMRES slover with initial guess [3, -3]    
result = linalg.lgmres(A, b, [3,-3], callback=callback_function)
# second plot
ax = fig.add_subplot(1,2,2,projection='3d')
ax.plot_surface(x_axis, y_axis, z_axis, cmap='viridis', alpha=0.4) 
ax.contour(x_axis, y_axis, z_axis, colors='k')  
ax.set_xlabel('x'), ax.set_ylabel('y'), ax.set_zlabel('S(x)')
ax.view_init(azim=130, elev=50)
ax.plot(x, y, z, 'r-')
ax.scatter(x, y, z, color='r', marker='o')
ax.scatter(x[-1], y[-1], z[-1], color='r', marker='o')
ax.set_title(f'GMRES (x,y)=({x[-1]:.3f},{y[-1]:.3f}), z={z[-1]:.3f}')
plt.show()
 
