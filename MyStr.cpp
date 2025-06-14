#include "Mystr.h"
#include <cstring>
#include <stdexcept>

int Mystr::length(const char* a) const
{
    int c = 0;
    while (a && a[c] != '\0')
    {
        c++;
    }
    return c;
}

Mystr::Mystr()
    : len(0), cap(1), str(nullptr)
{}

Mystr::Mystr(const char* p)
{
    len = length(p);
    cap = len ? len : 1;
    str = new char[cap];
    for (int i = 0; i < len; i++)
    {
        str[i] = p[i];
    }
}

Mystr::Mystr(char ch, int n) : len(n), cap(n)
{
    str = new char[cap];
    for (int i = 0; i < n; i++)
    {
        str[i] = ch;
    }
}

void Mystr::itos(int num)
{
    delete[] str;
    int temp = num;
    int total = 0;
    do 
    {
        temp /= 10;
        total++;
    } while (temp != 0);

    len = total;
    cap = total;
    str = new char[cap];

    for (int i = total - 1; i >= 0; i--)
    {
        str[i] = (num % 10) + '0';
        num /= 10;
    }
}

Mystr::Mystr(int num) : str(nullptr)
{
    itos(num);
}

Mystr::Mystr(const Mystr& other) : len(other.len), cap(other.cap)
{
    str = (cap > 0) ? new char[cap] : nullptr;
    for (int i = 0; i < len; i++)
    {
        str[i] = other.str[i];
    }
}

Mystr& Mystr::operator=(const Mystr& s2)
{
    if (this != &s2)
    {
        delete[] str;
        len = s2.len;
        cap = s2.cap;
        str = (cap > 0) ? new char[cap] : nullptr;
        for (int i = 0; i < len; i++)
        {
            str[i] = s2.str[i];
        }
    }
    return *this;
}

Mystr::~Mystr()
{
    delete[] str;
}

void Mystr::print() const
{
    for (int i = 0; i < len; i++)
    {
        cout << str[i];
    }
	cout << endl;

	if (len == 0)
	{
		cout << "Empty string" << endl;
	}
	else
	{
		cout << "Length: " << len << ", Capacity: " << cap << endl;
	}
}

void Mystr::regrow()
{
    int new_cap = cap * 2 + 1;
    char* new_str = new char[new_cap];

	if (!new_str)
	{
		throw runtime_error("Memory allocation failed");
	}

    for (int i = 0; i < len; i++)
    {
        new_str[i] = str[i];
    }

    delete[] str;
    str = new_str;
    cap = new_cap;
}

Mystr& Mystr::insert_at(int num, char ch)
{
    if (len >= cap)
    {
        regrow();
    }
    for (int i = len; i > num; i--)
    {
        str[i] = str[i - 1];
    }
    str[num] = ch;
    len++;

    return *this;
}

Mystr& Mystr::insert_at(int n, const Mystr& a)
{
    for (int i = 0; i < a.len; i++)
    {
        insert_at(n + i, a.str[i]);
    }
    return *this;
}

//also complete the powerup class and it's code in the main.cpp wherever else it is needed.
// please make three powerup derived classes ie maxhealth, 2x speed and 2x size and make them so that
// composition can be demonstrated.
// also add a few random cases of exception handling.
//please implement a template class and functions, and please write the code in the exact same style
// i have written it,
// i would really appreciate it. also note that im using namespace std.