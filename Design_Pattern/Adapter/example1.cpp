#include <algorithm>    // reverse function
#include <iostream>

/**
 * The Target defines the domain-specific interface used by the client code.
 */
class Target {
public:
    // Constructor
    Target()
    {
        std::cout << "Constructor Target class" << std::endl;
    }

    // Destructor
    virtual ~Target()
    {
        std::cout << "Destructor Target class" << std::endl;
    }

    virtual std::string Request() const 
    {
        return "Target: The default target's behavior.";
    }
};

/**
 * The Adaptee contains some useful behavior, but its interface is incompatible
 * with the existing client code. The Adaptee needs some adaptation before the
 * client code can use it.
 */
class Adaptee {
public:
    // Constuctor
    Adaptee()
    {
        std::cout << "Constructor Adaptee class" << std::endl;
    }
    
    // Destructor
    virtual ~Adaptee()
    {
        std::cout << "Destructor Adaptee class" << std::endl;
    }

    std::string SpecificRequest() const 
    {
        return ".eetpadA eht fo roivaheb laicepS";
    }
};

/**
 * The Adapter makes the Adaptee's interface compatible with the Target's
 * interface.
 */
class Adapter : public Target 
{
private:
    Adaptee *adaptee_;
public:
    // Constuctor
    Adapter()
    {
        std::cout << "Constructor Adapter class" << std::endl;
    }
    
    // Destructor
    virtual ~Adapter()
    {
        std::cout << "Destructor Adapter class" << std::endl;
    }

    // Overide constructor
    Adapter(Adaptee *adaptee) : adaptee_(adaptee) {}
    
    
    std::string Request() const override 
    {
        std::string to_reverse = this->adaptee_->SpecificRequest();
        std::reverse(to_reverse.begin(), to_reverse.end());
        return "Adapter: (TRANSLATED) " + to_reverse;
    }
};

/**
 * The client code supports all classes that follow the Target interface.
 */
void ClientCode(const Target *target) 
{
  std::cout << target->Request();
}

int main() {
    std::cout << "Client: I can work just fine with the Target objects:\n";
    Target *target = new Target;
    ClientCode(target);
    std::cout << "\n\n";
    Adaptee *adaptee = new Adaptee;
    std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
    std::cout << "Adaptee: " << adaptee->SpecificRequest();
    std::cout << "\n\n";
    std::cout << "Client: But I can work with it via the Adapter:\n";
    Adapter *adapter = new Adapter(adaptee);
    ClientCode(adapter);
    std::cout << "\n";

    delete target;
    delete adaptee;
    delete adapter;
    return 0;
}