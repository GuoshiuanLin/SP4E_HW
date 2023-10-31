# SP4E_HW
## homeworks for SP4E 2023
## Assignment 2
### Authors
- Jakov Oreb: exercise 5 & 6
- Guo-Shiuan Lin: exercise 1 to 4 \
The best way to divide the work is to have the files for exercises 1-4 ready (especially exercise 1-2) because they are needed for exercises 5 and 6.\
At first, Guo-Shiuan finished and pushed exercises 1-2 very quickly so Jakov could continue with exercise 6 without a long wait.\
Exercise 5 was done at the end because exercise 1-4 has to be done before that.
 
### Required Python packages
- matplotlib
  
### Configure and build the project 
In the main directory HW2/\
`mkdir build`\
`cd build`\
`ccmake ..`\
`make`

### Run the program
`./src/main arg1 arg2 arg3 arg4`
- arg1: int, the maximum iterator
- arg2: int, 0 or 1. 0: Instantiate ComputeArithmetic. 1: Instantiate ComputePi
- arg3: int, frequnecy to output results to the screen
- arg4: string, 'print':print result to the screen with the frequency (arg3) or 'write': write and save to file (every iteration step)\
For example,\
`./src/main 20 0 2 'print'` would print the iteration of ComputeArithmetic to the screen with frequency 2 up to maximum iterator 20\
`./src/main 30 1 5 'write'` would write the iteration up to 30 of ComputePi to output file. the frequency argument does not matter in this case, because every step is written.

### Configuration
- dumper.hh
  - `setPrecision()`: set the precision of the numerical output to the screen
- main.cc
  - setSeparator(): choose the seperator: `,`, `|`, or ` ` for the output file type `.csv`, `.psv`, or `.txt`   

### plotting the results of interation 
- plot.ipynb

### Excercise 6: Integral: Riemann Integral - 

To run the integral operations (using RiemannIntegral.cc) the arguments needed are the following:

- ```Enter the lower bound (a)```: lower limit of the definite integral.
- ```Enter the upper bound (b)```: upper limit of the definite integral.
- ```Choose the functionType```: type of the function inside the integral.
    - "cos" for cosine
    - "sin" for sine
    - "cube" for cubic function.
- ```Enter the number of intervals```: number of used intervals.   

An example set of program arguments for taking the integral of a cubic function from 0 to 1.
```g++
./RiemannIntegral 0 1 cube 100 
```
An example set of program arguments for taking the integral of a cosine function from 0 to pi.
```g++
./RiemannIntegral 0 3.141592 cos 1000
```
An example set of program arguments for taking the integral of a sine function from 0 to pi/2.
```g++
./RiemannIntegral 0 1.570796  sin 100
```
