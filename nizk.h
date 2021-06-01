#ifndef NIZK_H
#define NIZK_H

#include "circuit.h"
#include "gc.h" 
#include <vector>
#include <iostream> 
#include <algorithm>
#include "FileIO.hpp"
using namespace std;
 
class NIZK{
public:
    int party;
    FileIO *io;
    Circuit *circuit; 
    vector<GarbledCircuit*> GC;

    NIZK(int party,FileIO *io, Circuit *circuit){
        this->party=party;
        this->io=io;
        this->circuit=circuit; 
    }


    void prove(vector<int>alice_inputs){
        for(int i=0;i<REP;i++){
            GarbledCircuit *gc=new GarbledCircuit();
            Garbling::Gb(gc,circuit);
            GC.push_back(gc);
            
            int cur=0;
            block c[2];
            c[0]=gc->constant[0];
            c[1]=gc->constant[1]^gc->delta;
            io->write_block(c,2); 
            for(auto &g:circuit->gates){
                if(g.type==AND_GATE)
                    io->write_vec(gc->tables[g.z]);
                if(g.type==OUT_GATE)
                    io->write_vec(gc->d[cur++]);
            }
        }

        block blk;
        io->hash.digest((char*)&blk);
        vector<int>b;
        for(int i=0;i<NUM_INT;i++)
        for(int j=0;j<32;j++)
            b.push_back(blk[i]>>j&1);
        
        for(int i=0;i<REP;i++){ 
            GarbledCircuit *gc=GC[i];

            if(b[i]){
                io->write_block(&gc->seed,1);
            }else{
                Garbling::En(gc,circuit,alice_inputs);
                io->write_vec(gc->e);
            }
        }
        io->close();
    }

    bool verify(){
        for(int i=0;i<REP;i++){
            GarbledCircuit *gc=new GarbledCircuit();
            gc->tables.resize(circuit->gates.size());
            gc->labels.resize(circuit->gates.size());
            GC.push_back(gc);
            
            int cur=0;
            io->read_block(gc->constant,2); 
            for(auto &g:circuit->gates){
                if(g.type==AND_GATE){ 
                    io->read_vec(gc->tables[g.z]);
                }
                if(g.type==OUT_GATE){
                    gc->d.push_back(GarbledTable());
                    io->read_vec(gc->d[cur++]);
                }
            }
        }

        block blk;
        io->hash.digest((char*)&blk);
        vector<int>b;
        for(int i=0;i<NUM_INT;i++)
        for(int j=0;j<32;j++)
            b.push_back(blk[i]>>j&1);
        
        vector<vector<int> >all_out;
        for(int i=0;i<REP;i++){ 
            vector<int>results;
            GarbledCircuit *gc=GC[i];

            if(b[i]){
                block s;
                io->read_block(&s,1);
                GarbledCircuit *gc2=new GarbledCircuit(s);
                Garbling::Gb(gc2,circuit);

                if(gc2->tables==gc->tables && gc2->d==gc->d){
                    //puts("yes");
                }else{
                    puts("no");
                    return false;
                } 
            }else{
                io->read_vec(gc->e);
                int cur=0;
                for(auto &g:circuit->gates){
                    if(g.type==INPUT_ALICE_GATE)
                        gc->labels[g.x]=gc->e[cur++];
                }

                Garbling::Ev(gc,circuit);
                Garbling::De(gc,circuit,results);
                all_out.push_back(results);
            }
        } 


        vector<int>outputs;
        for(int i=0;i<all_out[0].size();i++){
            vector<int>tmp;
            for(int j=0;j<all_out.size();j++){
                int x=all_out[j][i]; 
                tmp.push_back(x);
                if(x!=tmp[0]){
                    puts("CHEATING !!");
                    return false;
                }
             }
            outputs.push_back(tmp[0]);
        }

        for(auto x:outputs)
            cerr<<x;
        cerr<<endl;
         

        return true;

    } 
};

 
#endif