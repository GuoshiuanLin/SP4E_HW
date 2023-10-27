#ifndef DUMPER_HH
#define DUMPER_HH

#include <iostream>
#include "series.hh"
#include <fstream>

class DumperSeries{
public:
virtual void dump() = 0;
protected:
Series & series;
    // Constructor to initialize the reference to a Series object
    DumperSeries(Series& s) : series(s) {
    }
};      

class WriteSeries : public DumperSeries{
    public:
    int maxiter;
    char separator;
    // Constructor to initialize N
    WriteSeries(int max, Series& s) : maxiter(max), DumperSeries(s) {
    }
    // Function to set the separator
    void setSeparator(char sep) {
        separator = sep;
    }
    void dump() override {
        std::string fileExtension;
        
        // Determine the file extension based on the separator
        if (separator == ',') {
            fileExtension = ".csv";
        } else if (separator == '|') {
            fileExtension = ".psv";
        } else {
            fileExtension = ".txt";
        }
        // Generate the output file name based on separator
        std::string fileName = "output" + fileExtension;
        // Open a file for writing
        std::ofstream outputFile(fileName);
        // if series is ComputeArthimetic, else series is ComputePi
        if (std::isnan(series.getAnalyticPrediction())){
            for (int i = 1; i <= maxiter; i+=1){
            double result = series.compute(i);
            // Write the result to the file with the specified separator
            outputFile << result << separator << std::endl;
        }
        outputFile.close();
        } else {
            for (int i = 1; i <= maxiter; i+=1){
            double result = series.compute(i);
            // Write the result to the file with the specified separator
            outputFile << result << separator << std::endl;
        }
        // write the anlytical value at the end of the file
        outputFile << series.getAnalyticPrediction() << std::endl;
        outputFile.close();
    }
    }
};

class PrintSeries : public DumperSeries {
    public:
    int frequency;
    int maxiter;
    // Constructor to initialize N
    PrintSeries(int freq, int max, Series& s) : frequency(freq), maxiter(max), DumperSeries(s) {
    }
    
    // Override the Method dump
    void dump() override {
        std::cout << "frequency is " << frequency << " maxiter is " << maxiter << std::endl;
        if (std::isnan(series.getAnalyticPrediction())){
            for (int i = 1; i <= maxiter; i+=frequency){
            std::cout << "at " << i << "th step, result is " << series.compute(i) << std::endl;
        }
        } else {
            for (int i = 1; i <= maxiter; i+=frequency){
            std::cout << "at " << i << "th step, result is " << series.compute(i) << ". Convergence = " << series.getAnalyticPrediction()-series.compute(i) << std::endl;
        }
    }
};
};
#endif // DUMPER_HH