# SP4E_HW
## Introduction 
This is the homework for SP4E 2023 Assignment 2

### Authors
- Jakov Oreb: exercise 5 & 6
- Guo-Shiuan Lin: exercise 1 to 4 \
The best way to divide the work is to have the files for exercises 1-4 ready (especially exercise 1-2) because they are needed for exercises 5 and 6.\
At first, Guo-Shiuan finished and pushed exercises 1-2 very quickly so Jakov could continue with exercise 6 without a long wait.\
Exercise 5 was done at the end because exercise 1-4 has to be done before that.
 
### Prerequisites
- C++ Compiler
- python
- matplotlib
  
### Installation
`git clone https://github.com/GuoshiuanLin/SP4E_HW.git`\
`cd HW2`: enter the main directory\
`mkdir build`\
`cd build`\
`ccmake ..`\
`make`

### Getting started
`./src/main arg1 arg2 arg3 arg4`
- arg1: int, the maximum iterator
- arg2: string, 'ComputeArithmetic', 'ComputePi', or 'RiemannIntegral' 
- arg3: int, frequnecy to output results to the screen
- arg4: string, 'print':print result to the screen with the frequency (arg3) or 'write': write and save to file (every iteration step)\
For example,\
`./src/main 20 'ComputeArithmetic' 2 'print'` would print the iteration of ComputeArithmetic to the screen with frequency 2 up to maximum iterator 20\
`./src/main 30 'ComputePi' 5 'write'` would write the iteration up to 30 of ComputePi to output file. the frequency argument does not matter in this case, because every step is written.
`./src/main 100 'RiemannIntegral' 1 'print'` would run the RiemannIntegral with 100 iterations, arg3 and arg4 are null when arg2 is `'RiemannIntegral'`.

For `RiemannIntegral` after entering `./src/main arg1 arg2 arg3 arg4` in the command terminal, follow the interactive instruction below.
- ```Enter the lower bound (a)```: lower limit of the definite integral.
- ```Enter the upper bound (b)```: upper limit of the definite integral.
- ```Choose the functionType```: type of the function inside the integral.
    - "cos" for cosine
    - "sin" for sine
    - "cube" for cubic function.

### Configuration
- dumper.hh
  - `setPrecision()`: set the precision of the numerical output to the screen
- main.cc
  - setSeparator(): choose the seperator: `,`, `|`, or ` ` for the output file type `.csv`, `.psv`, or `.txt`   

### Plotting
Run in the command terminal: `python plot.py`

### Main Classes
#### `ComputeArithmetic`
compute the sum of 1,2,..., N
#### `ComputePi`
compute approximation of Pi using integers from 1, 2, ..., to N
#### `RiemannIntegral`
compute Riemann integral of one of the three functions
#### `WriteSeries`
write results at every step to file output.fileType
#### `PrintSeries`
print results at defined frequency to screen and save in print_output.txt
