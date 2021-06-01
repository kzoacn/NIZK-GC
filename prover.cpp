#include "circuit.h"
#include "gc.h"  
#include "FileIO.hpp"
#include "nizk.h" 

int main(int argc,char **argv){  
    Circuit *circuit=new Circuit; 
    circuit->from_file("sha1.txt"); 

    FileIO *io=new FileIO("proof","wb");
    

 
    NIZK *nizk=new NIZK(ALICE,io,circuit);
 
    vector<int> alice_in;

    for(int i=0;i<512;i++)
        alice_in.push_back(0);
    

    nizk->prove(alice_in);

    return 0;
}