#include <iostream>
#include <cstdlib>
 
int main()
{
    if(const char* env_p = std::getenv("LD_LIBRARY_PATH"))
        std::cout << "Your LD_LIBRARY_PATH is: " << env_p << '\n';
}