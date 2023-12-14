### Homework 4
## Introduction 
This is the homework for SP4E 2023 Assignment 4

### Authors
- Jakov Oreb
- Guo-Shiuan Lin

### Textual Answers
1.2. In class ParticlesFactory, createSimulation function has been overloaded to take functor as one of its argument. Comment on what this function is doing?

It means that `createSimulation` can accept a callable object or function with a specific signature. The purpose of providing this overloaded version is to allow users to customize or extend the behavior of the createSimulation function by changing the arguments in `createComputes`.
In this case, users can choose between "planet" and 'material_point'.

2.2. How will you ensure that references to Compute objects type are correctly managed in the pythonbindings?

One can use the py::return_value_policy when binding methods that return references or pointers to Compute objects. The correct policy depends on the ownership and lifetime management strategy of the underlying C++ objects.
