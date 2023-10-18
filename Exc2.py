
import numpy as np
from matplotlib import pyplot as plt
from optimizer import quadratic, optimizer
from scipy.sparse import linalg
import argparse
from GMRes_1 import gmres

def arg_paser ():
    
    parser = argparse.ArgumentParser(description = 'input of the coefficinets A and b')
    parser.add_argument('--matrix_A', action='store', type=float, nargs= '+', help='matrix A elements')
    parser.add_argument('--nrows_A', action = 'store', type = int, help ='matrix A - number of rows')
    parser.add_argument('--vector_b', action = 'store', type = float, nargs='+', help = 'elements of vector b')
    parser.add_argument('--minimizer', action = 'store', type = str, help = 'Minimizer specification: GMRES_custom, GMRES')
    parser.add_argument('--plot', action = 'store_true', help='Reusults plot')
    
    return parser.parse_args()

def matrix_from_argparse (matrix_A, nrows_A):
    """Return matrix A with number of rows equal nrow_A
    """
    return np.array(matrix_A).reshape((nrows_A, len(matrix_A) // nrows_A))

if __name__=='__main__':
    
    print('Exercise 2')
    print('___')
    
    args = arg_paser()
    
    A = matrix_from_argparse(args.matrix_A, args.nrows_A)
    print(f'A is:\n {A}')
    b = np.array(args.vector_b)
    # b = np.array(args.vector_b)[np.newaxis].T
    print(f'b is:\n {b}')
    # initialization
    x0 = np.zeros_like(b)+3
    
    method = args.minimizer
    plot_axis_offset = 3
    
    if method == 'GMRES_custom': # the GMRES we developed ourselves
        print ('Selected method is the custom GMRes, ...')
        solution_history = gmres(A, b, x0, 1e-9, 200)
        print (f'Solution is {solution_history[-1]}')
    
    elif method =='GMRES': # the GMRES from scipy
        print (f'Selected method is  {method}')
        x_history = []
        y_history = []
        z_history = []
        def callback_function(params):
            x_history.append(params[0]), y_history.append(params[1]), z_history.append(quadratic(params)[0])
        result = linalg.lgmres(A, b, [3,3], callback=callback_function)
        print(f'Solution is ({x_history[-1]}, {y_history[-1]})')
    else:
        raise NameError(f'{method} method is invalid')


    if args.plot:
        # use the same plotting routine from the previous exercise
        # initialize x, y, and z for plotting the background quadratic function
        x_axis = np.linspace(-3,3,100)
        y_axis = np.linspace(-3,3,100)
        z_axis = np.zeros((len(x_axis), len(y_axis)))
        for i, x in enumerate(x_axis):
            for j, y in enumerate(y_axis):
                z_axis[i, j] = quadratic(np.array([x,y]))

        # create 2-D coordinates and plot the quadratic function
        x_axis, y_axis = np.meshgrid(x_axis, y_axis, indexing='ij')
        fig = plt.figure(figsize=plt.figaspect(0.5))
        ax = fig.add_subplot(1,1,1,projection='3d')
        ax.plot_surface(x_axis, y_axis, z_axis, cmap='viridis', alpha=0.4) 
        ax.contour(x_axis, y_axis, z_axis, colors='k')  
        ax.set_xlabel('x'), ax.set_ylabel('y'), ax.set_zlabel('S(x)')
        ax.view_init(azim=130, elev=50)
        
        # the plotting routines are slightly different for the two methods due to shape of the solutions stored
        if method == 'GMRES_custom':
            # unpack the history solutions during iteration into x and y seperately 
            x_history, y_history = zip(*[(x, y) for x, y in solution_history])
            # calculate the z values for each iteration 
            z_history = [quadratic(np.array(i))[0] for i in solution_history]
        
        # plot the history solutions and the corresponding values
        ax.plot(x_history, y_history, z_history, 'r-')
        ax.scatter(x_history, y_history, z_history, color='r', marker='o')
        ax.set_title(f'{method} (x,y)=({x_history[-1]:.3f},{y_history[-1]:.3f}), z={z_history[-1]:.3f}')
    else:
        print("Plotting is disabled.")
        
            
    
        
    
