
#include <iostream>
#include <functional>
#include "series.hh"

#ifndef HOMEWORK2_RIEMANNINTEGRAL_HPP
#define HOMEWORK2_RIEMANNINTEGRAL_HPP

/*
RiemannIntegral class to calculate integral of a function
*/

class RiemannIntegral {
public:
    RiemannIntegral(std::function<double(double)> f, double a, double b, int N) 
        : function(f), lowerBound(a), upperBound(b), numIntervals(N) {}

    double CalculateRiemannSum() {
        double delta_x = (upperBound - lowerBound) / numIntervals;
        double sum = 0.0;

        for (int i = 0; i < numIntervals; i++) {
            double x_i = lowerBound + i * delta_x;
            double f_x_i = function(x_i);
            sum += f_x_i * delta_x;
        }

        return sum;
    }

private:
    std::function<double(double)> function;
    double lowerBound;
    double upperBound;
    int numIntervals;
};

#endif //HOMEWORK2_RIEMANNINTEGRAL_HPP
