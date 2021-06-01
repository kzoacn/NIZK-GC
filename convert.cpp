 
#include <stdio.h>
#include <fstream>
#include <vector>
#include  "circuit.h"
using std::vector;
 
#define AND_GATE 0
#define XOR_GATE 1
#define NOT_GATE 2

Circuit *circuit;
class BristolFormat { public:
	int num_gate, num_wire, n1, n2, n3;
	vector<int> gates; 
	std::ofstream fout;
    vector<int>wires;
	BristolFormat(int num_gate, int num_wire, int n1, int n2, int n3, int * gate_arr) {
		this->num_gate = num_gate;
		this->num_wire = num_wire;
		this->n1 = n1;
		this->n2 = n2;
		this->n3 = n3;
		gates.resize(num_gate*4); 
	}

	BristolFormat(FILE * file) {
		this->from_file(file);
	}

	BristolFormat(const char * file) {
		this->from_file(file);
	}

 

	void from_file(FILE * f) {
		int tmp;
		(void)fscanf(f, "%d%d\n", &num_gate, &num_wire);
		(void)fscanf(f, "%d%d%d\n", &n1, &n2, &n3);
		(void)fscanf(f, "\n");
		char str[10];
		gates.resize(num_gate*4); 
        wires.resize(num_wire);
		for(int i = 0; i < num_gate; ++i) {
			(void)fscanf(f, "%d", &tmp);
			if (tmp == 2) {
				(void)fscanf(f, "%d%d%d%d%s", &tmp, &gates[4*i], &gates[4*i+1], &gates[4*i+2], str);
				if (str[0] == 'A') gates[4*i+3] = AND_GATE;
				else if (str[0] == 'X') gates[4*i+3] = XOR_GATE;
			}
			else if (tmp == 1) {
				(void)fscanf(f, "%d%d%d%s", &tmp, &gates[4*i], &gates[4*i+2], str);
				gates[4*i+3] = NOT_GATE;
			}
		}
	}

	void from_file(const char * file) {
		FILE * f = fopen(file, "r");
		this->from_file(f);
		fclose(f);
	} 
    void compute() {
        for(int i=0;i<n1;i++)
            wires[i]=circuit->input_alice();
        
        int one=circuit->input_const(1);

		for(int i = 0; i < num_gate; ++i) {
			if(gates[4*i+3] == AND_GATE) {
				wires[gates[4*i+2]] = circuit->And(wires[gates[4*i]], wires[gates[4*i+1]]);
			}
			else if (gates[4*i+3] == XOR_GATE) {
				wires[gates[4*i+2]] = circuit->Xor(wires[gates[4*i]], wires[gates[4*i+1]]);
			}
			else{  
				wires[gates[4*i+2]] = circuit->Xor(wires[gates[4*i]], one);
            }
		}

        for(int i=0;i<n3;i++)
            wires[i]=circuit->out(num_wire-n3+i);
	}
};
   

int main(){
    circuit=new Circuit();

    BristolFormat bf("sha-256.txt");
    bf.compute();

    circuit->to_file("sha2.txt");
    return 0;
}