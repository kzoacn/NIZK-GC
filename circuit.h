#ifndef CIR__
#define CIR__
#include <vector>
#include <cstdio>
#include "constants.h"
/*


IA x 
IB x

CONST y x
AND z x y
XOR z x y
OUT in 

*/ 
const int INPUT_ALICE_GATE=0;
const int INPUT_BOB_GATE=1;
const int CONST_GATE=2;
const int XOR_GATE=3;
const int AND_GATE=5;
const int OUT_GATE=6;

class Gate{
public:
    int type;
    int x,y,z; 
    Gate(int _t=0,int _x=0,int _y=0,int _z=0){
        type=_t;
        x=_x;
        y=_y;
        z=_z;
    }
};

class Circuit{
public:
    int num_gate;

    std::vector<Gate>gates;

    Circuit(){
        num_gate=0;
    }

    int input_alice(){
        gates.push_back(Gate(INPUT_ALICE_GATE,num_gate));
        return num_gate++;
    }
    int input_bob(){
        gates.push_back(Gate(INPUT_BOB_GATE,num_gate));
        return num_gate++;
    }
    int input_const(int x){
        gates.push_back(Gate(CONST_GATE,x,num_gate));
        return num_gate++;
    }

    int Xor(int x,int y){
        gates.push_back(Gate(XOR_GATE,x,y,num_gate));
        return num_gate++;
    }
    int And(int x,int y){
        gates.push_back(Gate(AND_GATE,x,y,num_gate));
        return num_gate++;
    }
    int out(int x){
        gates.push_back(Gate(OUT_GATE,x));
        return x;
    }

    void from_file(const char *name){
        FILE *fp=fopen(name,"r");
        gates.clear();
        int num;
        fscanf(fp,"%d\n",&num);
        gates.resize(num);
        char op[10];
        for(auto &g:gates){
            fscanf(fp,"%s",op);
            
            if(op[0]=='I' && op[1]=='A'){
                g.type=INPUT_ALICE_GATE;
                fscanf(fp,"%d\n",&g.x);
            }
            if(op[0]=='I' && op[1]=='B'){
                g.type=INPUT_BOB_GATE;
                fscanf(fp,"%d\n",&g.x);
            }
            if(op[0]=='C'){
                g.type=CONST_GATE;
                fscanf(fp,"%d %d\n",&g.y,&g.x);
            }
            if(op[0]=='X'){
                g.type=XOR_GATE;
                fscanf(fp,"%d %d %d\n",&g.z,&g.x,&g.y);
            }
            if(op[0]=='A'){
                g.type=AND_GATE;
                fscanf(fp,"%d %d %d\n",&g.z,&g.x,&g.y);
            }
            if(op[0]=='O'){
                g.type=OUT_GATE;
                fscanf(fp,"%d\n",&g.x);
            }
                
        }

        fclose(fp);
    }
    void to_file(const char *name){
        FILE *fp=fopen(name,"w");
        fprintf(fp,"%d\n",(int)gates.size());
        for(auto &g : gates){
            switch (g.type)
            {
            case INPUT_ALICE_GATE:
                fprintf(fp,"IA %d\n",g.x);
                break;
            case INPUT_BOB_GATE:
                fprintf(fp,"IB %d\n",g.x);
                break;
            case CONST_GATE:
                fprintf(fp,"CONST %d %d\n",g.y,g.x);
                break;
            case XOR_GATE:
                fprintf(fp,"XOR %d %d %d\n",g.z,g.x,g.y);
                break;
            case AND_GATE:
                fprintf(fp,"AND %d %d %d\n",g.z,g.x,g.y);
                break;
            case OUT_GATE:
                fprintf(fp,"OUT %d\n",g.x);
                break;

            default:
                break;
            }
        }
        fclose(fp);
    }

};
 

#endif