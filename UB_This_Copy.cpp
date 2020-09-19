/**
 * An example of an undefined behavior,
 * that occured, because "this" pointer was captured
 */
#include <iostream>

using namespace std;
class A{
public:
    int a;
    A(): a(1) {};
    // Return a printer function
    auto getPrinter(){ 
        return [=]() { std::cout << a << std::endl; };
    };
};
int main()
{
    A* obj = new A;
    auto printer_function = obj->getPrinter();
    printer_function();
	// An object is destroyed
    delete obj;
	// Yet a function captured this
    printer_function();
    return 0;
}
