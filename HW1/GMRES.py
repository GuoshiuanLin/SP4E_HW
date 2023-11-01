# -*- coding: utf-8 -*-
"""
Created on Tue Oct 17 10:35:24 2023

@author: Jakov Oreb & Guo-Shiuan Lin 
"""

'''
#This code is a code for the Generalized minimal residual method implementation according to the 
wikipedia page: https://en.wikipedia.org/wiki/Generalized_minimal_residual_method
'''

import numpy as np

    
def gmres(A, b, x0, tol=1e-6, max_iter: int=100):
    """Using GMRES method to solve Ax = b

    Args:
        A (array): LHS matrix
        b (array): RHS vector
        x0 (array): initial guess
        tol (float, optional): tolerence for the residual. Defaults to 1e-6.
        max_iter (int, optional): number of itration. Defaults to 100.

    Returns:
        solutions: list of arrays of history solutions during the iteration
    """
    n = len(b)
    # H: to store the upper Hessenberg matrix generated during the iterations
    H = np.zeros((max_iter + 1, max_iter))
    # Q: to store the vector during the iteration
    Q = np.zeros((n, max_iter + 1))
    # r: residual between b and the LHS
    r = b - np.einsum('ij,j->i', A, x0)
    beta = np.linalg.norm(r)
    Q[:, 0] = r / beta
    x = x0
    # List to store history solutions at each iteration
    solutions = [x.copy()]  
    
    # start iteration
    for k in range(max_iter):
        v = np.einsum('ij,j->i', A, Q[:, k])
        for j in range(k + 1):
            H[j, k] = np.inner(Q[:, j], v)
            v -= H[j, k] * Q[:, j]
        
        # construct the vectors for the Krylov
        H[k + 1, k] = np.linalg.norm(v)
        if H[k + 1, k] != 0:
            Q[:, k + 1] = v / H[k + 1, k]
            
        # e1 is used as a basis vector to find the coefficients y in the least squares problem
        e1 = np.zeros(k + 1)
        e1[0] = 1
        y, _, _, _ = np.linalg.lstsq(H[:k + 1, :k + 1], beta * e1, rcond=None)
        
        # update the approximate solution
        x = x0 + np.einsum('ij,j->i', Q[:, :k + 1], y)
        solutions.append(x.copy())
        
        # calculate the residual
        r = b - np.einsum('ij,j->i', A, x)
        residual_norm = np.linalg.norm(r)
        print(f"Iteration {k + 1}: Residual Norm = {residual_norm}")

        # stop when residual is small enough
        if residual_norm < tol:
            break  
        
    return solutions   
