#include "circuit.h"
#include "gc.h"  
#include "FileIO.hpp"
#include "nizk.h" 

int main(int argc,char **argv){  
    Circuit *circuit=new Circuit; 
    circuit->from_file("sha1.txt"); 

    FileIO *io=new FileIO("proof","rb");
    

 
    NIZK *nizk=new NIZK(BOB,io,circuit);
 
    cout<< nizk->verify() <<endl;

    return 0;
}