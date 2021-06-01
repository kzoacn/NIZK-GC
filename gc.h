#ifndef GC__H
#define GC__H
#include <iostream>
#include "circuit.h"
#include "block.h"
#include <vector>
#include "hash.h"
#include "prg.h" 
typedef std::vector<block> GarbledTable;

class GarbledCircuit{
public:
    std::vector<GarbledTable>tables;
    std::vector<block>labels;
    std::vector<block>e;
    std::vector<GarbledTable>d;

    block seed;
    block delta;
    block constant[2];
    PRG prg;

    GarbledCircuit(){
        PRG().random_block(&seed,1);  
        prg.reseed(&seed);

        
        prg.random_block(&constant[0],1); 
        prg.random_block(&constant[1],1);
        constant[0][NUM_INT-1]=0;
        constant[1][NUM_INT-1]=0;
        delta=new_label();
    }

    GarbledCircuit(block seed){
        this->seed=seed; 
        prg.reseed(&seed);
        
        prg.random_block(&constant[0],1); 
        prg.random_block(&constant[1],1);
        constant[0][NUM_INT-1]=0;
        constant[1][NUM_INT-1]=0;
        delta=new_label();
    }

    block new_label(){
        block tmp;
        prg.random_block(&tmp,1);
        tmp[NUM_INT-1]=0;
        return tmp;
    }
};

class Garbling{
public:

    static void Gb(GarbledCircuit *gc,Circuit *circuit){
        gc->tables.resize(circuit->gates.size());
        gc->labels.resize(circuit->gates.size());
        GarbledTable tb; 
        for(auto &g:circuit->gates){
            switch (g.type)
            {
            case INPUT_ALICE_GATE:
            case INPUT_BOB_GATE: 
                gc->labels[g.x]=gc->new_label(); 
                break;
            case CONST_GATE:
                gc->labels[g.y]=gc->constant[g.x];
                break;
            case XOR_GATE:
                gc->labels[g.z]=gc->labels[g.x]^gc->labels[g.y]; 
                break;
            case AND_GATE:
                gc->labels[g.z]=gc->new_label();

                for(int i=0;i<2;i++)
                for(int j=0;j<2;j++){
                    
                    block tmp;
                    block h[3];
                    h[0]=gc->labels[g.x]^(i?gc->delta:zero_block);
                    h[1]=gc->labels[g.y]^(j?gc->delta:zero_block);
                    h[2]=gc->labels[g.z]^((i&j)?gc->delta:zero_block);

                    tmp=Hash::hash_for_block(Hash::hash_for_block(h[0])^h[1])^h[2]; 

                    gc->tables[g.z].push_back(tmp);
                }
                break;
                //TODO random shuffle

            case OUT_GATE:
                tb.clear();
                for(int i=0;i<2;i++){
                    block tmp=gc->labels[g.x]^(i?gc->delta:zero_block);  
                    tmp=Hash::hash_for_block(tmp);
                    tb.push_back(tmp);
                } 
                gc->d.push_back(tb);
            
                break;

            default:
                break;
            }
        } 
    }


    static void Ev(GarbledCircuit *gc,Circuit *circuit){
        
        block key,tmp;
        for(auto &g:circuit->gates){
            switch (g.type)
            {
            case INPUT_ALICE_GATE:
            case INPUT_BOB_GATE:  
                break;
            case CONST_GATE:
                gc->labels[g.y]=gc->constant[g.x];
                break;
            case XOR_GATE:
                gc->labels[g.z]=gc->labels[g.x]^gc->labels[g.y]; 
                break;
            case AND_GATE:
                block h[2];
                h[0] = gc->labels[g.x];
                h[1] = gc->labels[g.y];
                block H=Hash::hash_for_block(Hash::hash_for_block(h[0])^h[1]);
                for(int i=0;i<4;i++){
                    tmp=gc->tables[g.z][i];
                    tmp=tmp^H; 
                    if(tmp[NUM_INT-1]==0){
                        gc->labels[g.z]=tmp;
                        break;
                    }
                }
                break; 
            }
            
        }

    }

    static void En(GarbledCircuit *gc,Circuit *circuit,std::vector<int>inputs){
        int cur=0;
        for(auto &g:circuit->gates){
            if(g.type==INPUT_ALICE_GATE){
                int val=inputs[cur++]; 
                gc->e.push_back(gc->labels[g.x]^(val?gc->delta:zero_block));
            }
        }
    }

    static void De(GarbledCircuit *gc,Circuit *circuit,std::vector<int>&outputs){
        outputs.clear();
        int cur=0;
        for(auto &g:circuit->gates){
            if(g.type==OUT_GATE){
                block h=Hash::hash_for_block(gc->labels[g.x]);
                for(int i=0;i<2;i++){
                    if(h==gc->d[cur][i]){
                        outputs.push_back(i);
                        break;
                    }
                } 
                cur++;
            }
        }
    }
}; 

#endif