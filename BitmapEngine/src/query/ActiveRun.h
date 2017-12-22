//defining the activeByte struct

#ifndef ACTIVERUN_H_
#define ACTIVERUN_H_

typedef unsigned char byte;

typedef struct activeRun{
	
	//The header byte of the current run
	byte header;

	//The type of the run, can be 1, 2, 3 or 4
	unsigned int run_type;

	//Pointer to the compressed sequnce of bytes
	byte *run_seq;
	byte *full_seq;

	//The length of the tail in bytes
	unsigned int tail_len; //only used for type 1 and 3, if of type 2 or 4 then value will always be 1 (note in this case there is no physical tail but a logical tail of 1)

	//The location of the odd bit in the current run
	unsigned int odd_pos;  //only used for type 2 and 4 (!!! will be non-initalized if type 1 or 3 !!!) 

	//The number of counter bytes used in the current run
	unsigned int counter_len; //only used for type 3 and 4, will be 0 for type 1 and 2

	//The fill bit corresponds to the bit being stored in the fill, storing a 1 means a 1-fill, stroing a 0 means a 0-fill
	unsigned int fill_bit;

	//The length of the fill in bytes
	unsigned int fill_len; 	

	//the position of  the run starting at the header.  Based off the over all compressed bit sequence.
	int run_pos;	

	//the poisiton of the tail bytes based from run_pos.	
	int tail_pos;

	//how many bytes are in the run			
	unsigned int run_size;	

	//The literal tail bytes of the current run, (!!! will be non-initalized if type 2 or 4 !!!)
	byte *tail_store;
}activeRun;

int getHeadType(byte header);

unsigned int counterBytes(byte * run, activeRun *curr_run);

activeRun *initDefRun();

activeRun *initActiveRun(byte *run, int seq_size, int run_start);

void printActiveRun(activeRun * param);

void store_tail(activeRun *run);

#endif /* ACTIVERUN_H_ */