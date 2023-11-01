
#include <iostream>
#include <functional>
#include "series.hh"

#ifndef HOMEWORK2_RIEMANNINTEGRAL_HPP
#define HOMEWORK2_RIEMANNINTEGRAL_HPP

/*
RiemannIntegral class (inherited from Series class to calculate integral of a function
*/

class RiemannIntegral : public Series {
public:
    //function, bottom, and upper bound can be decided by the user
    RiemannIntegral(std::function<double(double)> f, double a, double b) 
        : function(f), lowerBound(a), upperBound(b) {}
    double compute(unsigned int N) override {
        return computeSeries(N);
    }

protected:
    double sumSeries(unsigned int N) override{
        // return the Reimen integral with N as the maximum iterator
        double delta_x = (upperBound - lowerBound) / N;
        double sum = 0.0;

        for (int i = 0; i < N ; i++) {
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
};

#endif //HOMEWORK2_RIEMANNINTEGRAL_HPP
