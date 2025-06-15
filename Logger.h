#ifndef LOGGER_H
#define LOGGER_H

#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include "MyStr.h"
#include <fstream>
#include <iostream>

using namespace std;

class Logger
{
private:
    ofstream file;
    Logger() {}
    ~Logger() {}
public:

    static Logger& getinstance();
    void initialize(const string& filename);
    void cleanup(const string& filename);
    void writeerror(const string& error_message);
};
#endif