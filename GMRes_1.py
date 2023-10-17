# -*- coding: utf-8 -*-
"""
Created on Tue Oct 17 10:35:24 2023

@author: oreb
"""

'''
#This code is a code for the Generalized minimal residual method implementation according to the 
wikipedia page: https://en.wikipedia.org/wiki/Generalized_minimal_residual_method
'''

'''
First step is to import Numpy
'''

import numpy as np

'''
Definition of module to perform different matrix and vectopr operations independent of their size

'''
#Matrix multiplication

def matrix_multiplication (matrix_1, matrix_2):
    return np.einsum("ij, jk -> ik", matrix_1, matrix_2)

#Matrix and vector multiplication

def matrix_vector_mult (matrix_1, matrix_2):
    if matrix_1.ndim == 1:
        return np.einsum ("ij, j -> i", matrix_1, matrix_2)
    elif matrix_2.ndim == 1:
        return np.einsum ("i, ij -> j", matrix_1, matrix_2)
    else:
        raise ValueError("First or second matrix should be 1D - vector")
        
#Transpose matrix with einsum

def matrix_trnasp (matrix):
    return np.einsum("ij->ji", matrix)

#Dot product of 2 vectors
    
def dot_product (vector_1, vector_2):
    return np.einsum("i, i->", vector_1, vector_2)

#Vector norm

def vect_norm (vector):
    return np.sqrt(np.einsum("k,k->", vector, vector))

#Arnoldi function definition 

def arnoldi_f (A,Q,k):
    K_v = Q[:, k - 1] #Krylov Vector
    
    q = matrix_vector_mult(A, K_v) 
    
    h = np.zeros(k + 1)
    
    for i in range (0, k): 
        h[i] = dot_product (q.T, Q[:,i])
        q = q - h[i] * Q[:, i]
        
    h [k] = vect_norm(q)
    q = q / h [k]
    
    return h, q

def Givens_rotation_application (h, cs, sn, k):
    # apply for i th column
    for i in range (0, k - 1):
        temp = cs[i] * h[i] + sn [i] * h [i + 1]
        h[i + 1] = -sn[i]*h[i]+cs[i] * h [i + 1] 
        h [i] = temp
    
    #update the next cos, sin values for rotation 
    
    cs_k, sn_k = Givens_rot (h[k], h[k+1])
    
    #eliminate H(i-1, i)
    h [k] = cs_k * h [k-1] + sn_k * h[k]
    h [k]=0.0
    return h, cs_k, sn_k

def Givens_rot (v1, v2):
    if v1 == 0 and v2 == 0:
        cs, sn = 0.0
    
    else:
        t = np.sqrt(v1**2 + v2**2)
        cs, sn = v1/t, v2/t
    
    return cs, sn

def GMRes (A: np.ndarray, b: np.ndarray, x: np.ndarray, max_iter: int= 1e2, 
           treshhold: float= 1e-9):
    
    x_interations = [x]
    
    r = b - matrix_vector_mult(A,x)
    b_norm = vect_norm(b)
   
    error = vect_norm(r)/ b_norm
    
    #initialize the 1D vector
    
    sn = np.zeros ((max_iter, 1))
    cs = np.zeros ((max_iter, 1))
    
    # e1 = zeros (n,1)
    
    e1 = np.zeros((max_iter + 1, 1))
    
    e1 [0] = e1
    e = [error]
    
    r_norm = vect_norm(r)
    
    Q = np.zeros ((r.shape [0], max_iter + 1))
    H = np.zeros ((max_iter + 2, max_iter))
    
    Q [:, 0] = r/r_norm
    beta = r_norm * e1
    
    #run Arnoldi
    for k in range (0, max_iter):
        H[k+2, k], Q[:, k+1] = arnoldi_f(A, Q, k+1)
    
    #eliminate the last element in Hth row and update the rotation matrix 
    
        H [:k+2, k], cs[k], sn [k] =Givens_rotation_application(H[:k+2, k], cs, sn, k+1)
    
    #update the residual vector 
        beta [k+1] = -sn[k]*beta [k]
        beta [k] = cs [k]* beta [k]
    
        error = abs(beta [k+1])/ b_norm
    
        e.append(error)
        if error <= treshhold:
            break
   
        H_inv = np.linalg.inv(H[:k + 1, :k + 1])
        y = matrix_multiplication(H_inv, beta[:k + 1])
        x_current = x + matrix_multiplication(Q[:, k+1], y).reshape(x.shape)
        x.iterations.append(x_current)
    
    H_inv = np.linalg.inv(H[:k + 1, :k+1])
    y = matrix_multiplication(H_inv, beta[:k + 1])
    x = x + matrix_multiplication(Q[:,:k+1], y).reshape(x.shape)
    x_interations.append(x)
    
    return x, e, x_interations

A = np.array([[8, 1], [1, 3]])
b = np.array([1, 2])
x0 = np.array([5, 5])

GMRes(A, b, x0)
    
    
    


    
    



        
        
