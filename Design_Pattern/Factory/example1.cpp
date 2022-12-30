#include <iostream>

/**
 * The Product interface declares the operations that all concrete products must
 * implement.
 */
// Interface 
class Product {
public:
    // Constructor
    Product()
    {
        std::cout << "Constructor Product class" << std::endl;
    }

    // Destructor
    virtual ~Product() 
    {
        std::cout << "Destructor Product class" << std::endl;
    }

    // Abstract method
    virtual std::string Operation() const = 0;
};

/**
 * Concrete Products provide various implementations of the Product interface.
 */
class ConcreteProduct1 : public Product {
public:
    //  Constructor
    ConcreteProduct1()
    {
        std::cout << "Constructor ConcreteProduct1 class" << std::endl;
    };

    // Destructor
    virtual ~ConcreteProduct1() 
    {
        std::cout << "Destructor ConcreteProduct1 class" << std::endl;
    }

    std::string Operation() const override 
    {
        return "{Result of the ConcreteProduct1}";
    }
};
class ConcreteProduct2 : public Product 
{
public:
    // Constructor
    ConcreteProduct2()
    {
        std::cout << "Constructor ConcreteProduct2 class" << std::endl;
    };

    // Destructor
    virtual ~ConcreteProduct2() 
    {
        std::cout << "Destructor ConcreteProduct2 class" << std::endl;
    }

    std::string Operation() const override 
    {
        return "{Result of the ConcreteProduct2}";
    }
};

/**
 * The Creator class declares the factory method that is supposed to return an
 * object of a Product class. The Creator's subclasses usually provide the
 * implementation of this method.
 */
class Creator           // Abstract class
{
    /**
    * Note that the Creator may also provide some default implementation of the
    * factory method.
    */
public:
    // Constructor
    Creator()
    {
        std::cout << "Constructor Creator class" << std::endl;
    }
    
    // Destructor
    virtual ~Creator()
    {
        std::cout << "Destructor Creator class" << std::endl;
    };

    virtual Product* FactoryMethod() const = 0;     // Abstract method
    /**
    * Also note that, despite its name, the Creator's primary responsibility is
    * not creating products. Usually, it contains some core business logic that
    * relies on Product objects, returned by the factory method. Subclasses can
    * indirectly change that business logic by overriding the factory method and
    * returning a different type of product from it.
    */
    std::string SomeOperation() const 
    {
        // Call the factory method to create a Product object.
        Product* product = this->FactoryMethod();
        // Now, use the product.
        std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
        delete product;
        return result;
    }
};

/**
 * Concrete Creators override the factory method in order to change the
 * resulting product's type.
 */
class ConcreteCreator1 : public Creator {
    /**
    * Note that the signature of the method still uses the abstract product type,
    * even though the concrete product is actually returned from the method. This
    * way the Creator can stay independent of concrete product classes.
    */
public:
    // Constructor
    ConcreteCreator1()
    {
        std::cout << "Constructor ConcreteCreator1 class" << std::endl;
    };

    // Destructor
    virtual ~ConcreteCreator1() 
    {
        std::cout << "Destructor ConcreteCreator1 class" << std::endl;
    }
    
    Product* FactoryMethod() const override 
    {
        return new ConcreteProduct1();
    }
};

class ConcreteCreator2 : public Creator 
{
public:
    // Constructor
    ConcreteCreator2()
    {
        std::cout << "Constructor ConcreteCreator2 class" << std::endl;
    };

    // Destructor
    virtual ~ConcreteCreator2() 
    {
        std::cout << "Destructor ConcreteCreator2 class" << std::endl;
    }

    Product* FactoryMethod() const override 
    {
        return new ConcreteProduct2();
    }
};

/**
 * The client code works with an instance of a concrete creator, albeit through
 * its base interface. As long as the client keeps working with the creator via
 * the base interface, you can pass it any creator's subclass.
 */
void ClientCode(const Creator& creator) 
{
    // ...
    std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
            << creator.SomeOperation() << std::endl;
    // ...
}

/**
 * The Application picks a creator's type depending on the configuration or
 * environment.
 */
int main() 
{
    std::cout << "App: Launched with the ConcreteCreator1.\n";
    Creator* creator = new ConcreteCreator1();
    ClientCode(*creator);
    std::cout << std::endl;
    std::cout << "App: Launched with the ConcreteCreator2.\n";
    Creator* creator2 = new ConcreteCreator2();
    ClientCode(*creator2);
    delete creator;
    delete creator2;
    return 0;
}