#include "circuit.h"
#include "gc.h"  
#include "FileIO.hpp"
#include "nizk.h" 

int main(int argc,char **argv){  
    Circuit *circuit=new Circuit; 
    circuit->from_file("cir.txt"); 

    FileIO *io=new FileIO("proof","wb");
    

 
    NIZK *nizk=new NIZK(ALICE,io,circuit);
 
    vector<int> alice_in;

    for(int i=0;i<8;i++)
        alice_in.push_back(9>>i&1);
    

    nizk->prove(alice_in);

    return 0;
}