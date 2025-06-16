#ifndef SAFEBOX_H
#define SAFEBOX_H

#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class SafeValue
{
    T value;
public:
    SafeValue(T& v) : value(v) {}
    SafeValue(const T& v) : value(v) {}
    void set(const T& v) 
    {
        try
        {
            value = v;
        }
        catch (const exception& e) 
        {
            cerr << "Exception in set(): " << e.what() << endl;
        }
    
	}
    T get() const 
    {
        try
        {
            return value;
        }
        catch (const exception& e) 
        {
            cerr << "Exception in get(): " << e.what() << endl;
            return T{};
        }
    }
};
#endif
