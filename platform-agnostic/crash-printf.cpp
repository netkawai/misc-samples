#include <string> 
using namespace std;  

int main(void)
{ 
    string test; 
    test = "Hello string!!"; 
    printf("%s\n", test); 
    // Result 
    // (NULL) or crash. 
    //printf does not concern the type of pointer. 
    // because string class is not equal to char array

    return(0); 
} 

 

