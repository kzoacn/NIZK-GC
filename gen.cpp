#include "circuit.h"
#include "gc.h"   

void adder_full(Circuit *circuit) {

    int op1[32],op2[32],dest[32],carry;

    for(int i=0;i<32;i++)
        op1[i]=circuit->input_alice();
    for(int i=0;i<32;i++)
        op2[i]=circuit->input_const(12>>i&1);
    carry=circuit->input_const(0);

	for(int i=0;i<32;i++){ 
		int axc = circuit->Xor(op1[i],carry);
		int bxc = circuit->Xor(op2[i],carry);
		dest[i] = circuit->Xor(op1[i],bxc);
		int t = circuit->And(axc,bxc);
		carry =circuit->Xor(carry,t);
	}
    for(int i=0;i<32;i++)
        circuit->out(dest[i]);
}
 
int main(int argc,char **argv){ 
 
    Circuit *circuit=new Circuit; 
    adder_full(circuit);     
    circuit->to_file("cir.txt");  
    return 0;
}