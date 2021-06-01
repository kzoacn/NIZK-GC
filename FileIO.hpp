#ifndef RECORD_IO_CHANNEL
#define RECORD_IO_CHANNEL

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector> 
using std::string;
using std::vector;  
#include "block.h" 
#include "hash.h"

class FileIO {  
public:
	FILE *fp;

	FileIO(const char *file,const char *mode) {
		fp=fopen(file,mode);	
	}

	Hash hash;


	void write_data(const void * data, int len) { 
		fwrite(data,1,len,fp);
		hash.put(data,len);
	}

	void read_data(void  * data, int len) { 
		fread(data,1,len,fp);
		hash.put(data,len); 
	}

	template<class T>
	void write_vec(std::vector<T> &x){
		int sz=x.size();
		write_data(&sz,sizeof(sz));
		write_data(x.data(),sizeof(T)*sz);
	}
	template<class T>
	void read_vec(std::vector<T> &x){
		int sz;
		read_data(&sz,sizeof(sz));  
		x.resize(sz); 
		read_data(x.data(),sizeof(T)*sz); 	
	}

	void write_block(block *data,int len){
		write_data(data,sizeof(block)*len);
	}
	void read_block(block *data,int len){
		read_data(data,sizeof(block)*len);
	}

	void close(){
		fclose(fp);
	}
}; 
 
#endif  //NETWORK_IO_CHANNEL
