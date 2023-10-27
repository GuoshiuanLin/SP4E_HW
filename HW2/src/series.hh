#ifndef SERIES_HH
#define SERIES_HH

#include <iostream>
#include <cmath>

// parent class
class Series{
public:
virtual double compute(unsigned int N) = 0;
virtual double getAnalyticPrediction() {
        return nan("");
    }
};

// child class that computes the sum of 1,2,..., N
class ComputeArithmetic : public Series {
public:
    // Constructor to initialize N
    ComputeArithmetic() {
    }

    // Method to calculate and return the sum of numbers from 1 to N
    double compute(unsigned int N) override {
        double sum = 0.0;
        for (unsigned int i = 1; i <= N; i++) {
            sum += i;
        }
        return sum;
    }
};

// child class that computes the convergence of Pi
class ComputePi : public Series {
public:
    // Constructor to initialize N
    ComputePi() {
    }

    // Method to calculate and return Pi convergence using integers from 1 to N
    double compute(unsigned int N) override {
        float pi = 0.;
        for (unsigned int i = 1; i <= N; i++) {
             pi += 1./(1.*i*i);
        }
        pi *= 6.;
        pi = sqrt(pi);
        return pi;
    }
    double getAnalyticPrediction() override {
        return M_PI; 
    }
};

#endif // SERIES_HH