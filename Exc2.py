
import numpy as np
import argparse

from optimizer import optimizer
from GMRes_1 import GMRes

def arg_paser ():
    
    parser = argparse.ArgumentParser(description = 'input of the coefficinets A and b')
    parser.add_argument('--matrix_A', action='store', type=float, nargs= '+', help='matrix A elements')
    parser.add_argument('--nrows_A', action = 'store', type = int, help ='matrix A - number of rows')
    parser.add_argument('--vector_b', action = 'store', type = float, nargs='+', help = 'elements of vector b')
    parser.add_argument('--minimizer', action = 'store', type = str, help = 'Minimizer specification: GMRes (custom), BFGS, GMRES')
    parser.add_argument('--plot', action = 'store_true', default = False, help='Reusults plot')
    parser.add_argument('--export', action = 'store', type = str, default = None, help = 'save the exported figures')
    
    return parser.parse_args()

def matrix_from_argparse (matrix_A, nrows_A):
    return np.array(matrix_A, nrows_A).reshape((nrows_A, len(matrix_A) // nrows_A))

if __name__=='__main__':
    
    print('Exc2')
    print('___')
    
    args = arg_paser()
    
    A = matrix_from_argparse(args.matrix_A, args.nrows_A)
    b = np.array(args.vector_b)
    x0 = np.zeros_like(b)+10
    
    method = args.minimizer
    plot_axis_offset = 3
    
    if method == 'GMRes (custom)':
        print ('Selected method is custom GMRes, ...')
        x, _, x_interations = GMRes(A, b, x0, 200, 1e-9)
        print ('Solution is {x}')
    
    elif method in ['BFGS', 'GMRES']:
        print ('Selected method is  {method}')
        x = optimizer(x, A, b, method)
        print ('Solution x is {x}')
    else:
        raise NameError(f'{method} method is invalid')


        
            
    
        
    