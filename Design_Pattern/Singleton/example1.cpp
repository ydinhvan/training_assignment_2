#include <iostream>

using namespace std;

class Singleton
{
private:
    // constructor
    Singleton(const string value): value_(value)
    {
        // :value_(value) <=> this->value_ = value;
    }

    // static variable: sharing between class
    static Singleton* singleton_;
    
    string value_;
public:

    Singleton(Singleton &other) = delete;
    void operator = (const Singleton &) = delete;
    
    // static method
    static Singleton *GetInstance(const string& value);
    
    string getvalue() const
    {
        return value_;
    }

    string getvalue(string s)
    {
        return s;
    }
};

// Static method should be defined outside the class
Singleton *Singleton::singleton_ = 0;
// Define singleton
Singleton *Singleton::GetInstance(const string& value)
{
    if(singleton_ == nullptr)
    {  
        singleton_ = new Singleton(value);
    }
    return singleton_;
}

int main()
{
    // create object
    Singleton* singleton1 = Singleton::GetInstance("BAR");
    Singleton* singleton2 = Singleton::GetInstance("FOR");
    cout << "singleton1: "<< singleton1->getvalue() << endl; 
    // stdout: BAR
    cout << "singleton2: "<< singleton2->getvalue() << endl;
    // stdout: BAR
    
    // Explain: After create singleton1 object, 
    // static variable: singleton_ = BAR 
    // when create singleton2 object -> singleton_ !=  nullptr
    // -> return singleton_ = BAR 

    // copy constructor: creat new instance  -> disable
    Singleton* singleton3 = singleton1;
    singleton3 = Singleton::GetInstance("BOR");
    cout << "singleton2: "<< singleton3->getvalue() << endl;

    return 0;
}
