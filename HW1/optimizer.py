import numpy as np
from scipy.sparse import linalg
from scipy.optimize import minimize

# define the matrix
A = np.array([[8,1],[1,3]])
b = np.array([[2], [4]])

# define the quadratic function
def quadratic(x):
    """
    Args:
        x (ndarray): array of shape 2*1
    """
    S = 0.5 * x.T @ A @ x - x.T @ b
    return S

# define the optimizer
def optimizer(func, A, b, method):
    """
    Use 2 different optimization methods to optimize the cuntion

    Args:
        func: function to be optimized
        A (ndarray): LHS matrix 
        b (ndarray): RHS matrix
        method (string): optimization method chosen from Scipy

    Returns:
        x, y, z: parameters (x, y) that optimize the value (z) of the function 
    """
    if method=='BFGS':
        result = minimize(func, [3,-3], method='BFGS')
        x, y = result.x
        z = result.fun
    elif method=='GMRES':
        result = linalg.lgmres(A, b, [3,-3])
        x, y = result [0]
        z = quadratic(np.array([x,y]))[0]
    return x, y, z

print(f"optimization by BFGS: {optimizer(quadratic, A, b, 'BFGS')}")
print(f"optimization by GMRES: {optimizer(quadratic, A, b, 'GMRES')}")
