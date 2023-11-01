#ifndef DUMPER_HH
#define DUMPER_HH

#include <iostream>
#include "series.hh"
#include <fstream>
#include <iomanip> 

// parent class Series that set the interface
class DumperSeries{
public:
virtual void dump(std::ostream & os) = 0;
virtual void setPrecision(unsigned int precision){
    precision_ = precision;
}
// Declare the operator overload for << 
friend std::ostream & operator <<(std::ostream & stream, DumperSeries & _this);

protected:
Series & series;
unsigned int precision_; // Store precision

// Constructor to initialize the reference to a Series object
// Default precision is 6
DumperSeries(Series& s) : series(s), precision_(6) {
}
};      

// child clss WriteSeries that writes all steps before reaching maximim iterator to the defined file type
class WriteSeries : public DumperSeries{
    public:
    int maxiter;
    char separator;
    // Constructor to initialize N
    WriteSeries(int max, Series& s) : maxiter(max), DumperSeries(s) {
    }
    // Method to set the separator
    void setSeparator(char sep) {
        separator = sep;
    }
    // Method to write the file
    void dump(std::ostream &os = std::cout) override {
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
        
        if (std::isnan(series.getAnalyticPrediction())){
            // series is ComputeArthimetic
            // Write the result of every step to the file with the specified separator
            for (int i = 1; i <= maxiter; i+=1){
            double result = series.compute(i);
            outputFile << result << separator << std::endl;
        }
        outputFile.close();
        } else {
            // series is ComputePi
            for (int i = 1; i <= maxiter; i+=1){
            double result = series.compute(i);
            outputFile << result << separator << std::endl;
        }
        // write also the anlytical value at the end of the file
        outputFile << series.getAnalyticPrediction() << std::endl;
        outputFile.close();
    }
    }
};

// child class that prints the result to screen at defined frequency until reaching maximum iterator
class PrintSeries : public DumperSeries {
    public:
    int frequency;
    int maxiter;
    // Constructor to initialize N
    PrintSeries(int freq, int max, Series& s) : frequency(freq), maxiter(max), DumperSeries(s) {
    }
    
    // Method to print the result to screen
    void dump(std::ostream &os = std::cout) override {
        if (std::isnan(series.getAnalyticPrediction())){
            // series is ComputeArthimetic
            for (int i = 1; i <= maxiter; i+=frequency){
            os << std::setprecision(precision_);
            os << "At " << i << " th step, result is " << series.compute(i) << std::endl;
        }
        } else {
            // series is ComputePi
            for (int i = 1; i <= maxiter; i+=frequency){
            os << std::setprecision(precision_);
            os << "At " << i << " th step, result is " << series.compute(i) << ". Convergence = " << series.getAnalyticPrediction()-series.compute(i) << std::endl;
        }
    }
};
};
#endif // DUMPER_HH