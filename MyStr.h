#ifndef MYSTR_H
#define MYSTR_H

#include <iostream>
using namespace std;

class Mystr
{
    int len;
    int cap;
    char* str;

    int length(const char* a) const;

    void regrow();
    void itos(int num);

public:
    Mystr();
    Mystr(const char* p);
    Mystr(char ch, int n);
    Mystr(int num);
    Mystr(const Mystr& other);
    Mystr& operator=(const Mystr& s2);
    ~Mystr();

    void print() const;

    Mystr& insert_at(int num, char ch);
    Mystr& insert_at(int n, const Mystr& a);

    int getlength() const { return len; }
    int getcapacity() const { return cap; }
    const char* getstring() const { return str; }
};

#endif
