from matplotlib import pyplot as plt
import pandas as pd
import os

# check the existent file, read by pandas, and plot the numerical results on y axis with the iteration on x axis
if os.path.isfile('output.csv'):
    file = pd.read_csv('output.csv', index_col=False, names=['numbers'])
elif os.path.isfile('output.txt'):
    file = pd.read_csv('output.txt', index_col=False, names=['numbers'])    
else:
    file = pd.read_csv('output.psv',index_col=False, names=['numbers'],sep='|')
    
plt.figure()
plt.plot(file.index[:-1], file.numbers[:-1], '-^')
plt.xlabel('iterations')
plt.ylabel('sereis results')
plt.show()