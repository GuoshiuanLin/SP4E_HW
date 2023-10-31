#ifndef SERIES_HH
#define SERIES_HH

#include <iostream>
#include <cmath>

// parent class
class Series {
public:
    unsigned int current_index;
    double current_value;

    Series() : current_index(0), current_value(0.0) { }

    virtual double compute(unsigned int N) = 0;
    virtual double getAnalyticPrediction() {
        return nan("");
    }

    double computeSeries(unsigned int N) {
        if (current_index == N) {
            // If the series is already computed up to N, return the stored value
            return current_value;
        } else {
            // Otherwise, compute the series and memorize the result
            current_index = N;
            current_value = calculateSeries(N);
            return current_value;
        }
    }

protected:
    virtual double calculateSeries(unsigned int N) = 0;
};

// child class that computes the sum of 1,2,..., N
class ComputeArithmetic : public Series {
protected:
    double calculateSeries(unsigned int N) override {
        double sum = 0.0;
        for (unsigned int i = 1; i <= N; i++) {
            sum += i;
        }
        return sum;
    }

public:
    double compute(unsigned int N) override {
        return computeSeries(N);
    }
};

// Child class that computes the convergence of Pi
class ComputePi : public Series {
protected:
    double calculateSeries(unsigned int N) override {
        float pi = 0.;
        for (unsigned int i = 1; i <= N; i++) {
            pi += 1. / (1. * i * i);
        }
        pi *= 6.;
        pi = sqrt(pi);
        return pi;
    }

public:
    double compute(unsigned int N) override {
        return computeSeries(N);
    }

    double getAnalyticPrediction() override {
        return M_PI;
    }
};

#endif // SERIES_HH