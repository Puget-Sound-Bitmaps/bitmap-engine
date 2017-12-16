//File for processing query bytes 
#include <stdio.h>
#include <stdlib.h>
#include "ActiveRun.h"
//#include "Core.h"
//determines the type of run we are looking at
int getHeadType(byte header){

    //printf("getHeadType\n");
	/*unsigned char h_type4 = header >> 4;
	unsigned char h_type3 = header >> 5;
	unsigned char h_type2 = header >> 6;
	unsigned char h_type1 = header >> 7;*/

	//it's a type 1 run 
	if(header >> 7){
		return 1;
	}
	//it's a type 2 run 
	else if(header >> 6){
		return 2;
	}
	//it's a type 3 run 
	else if(header >> 5){
		return 3;
	}
	//it's a type 4 run
	else if(header >> 4){
		return 4;
	}
    return -1;
}

/*
param: the current run in the form of an unsigned char *
return: the number of counter bytes in the run
*/
unsigned int counterBytes(byte * run, activeRun *curr_run){
	int pos = 0;
	unsigned int f_len; //keeping it unsigned? I think this helps
	do{
		unsigned int temp = run[pos+1] << 1;
		temp >>= 1;
		f_len = f_len + temp; //I believe we can add chars to ints
		pos++;
	}while(run[pos+1] >> 7 != 0);

    curr_run->tail_pos = curr_run->run_pos + pos +1;

    //storing the run size
    curr_run->run_size = 1 + pos;

    curr_run->counter_len = pos;

	return f_len;
}


void store_tail(activeRun *run)
{
    run->tail_store = malloc(sizeof(byte) * run->tail_len);

    int i= 0;
    int pos;

    if(run->run_type == 1 || run->run_type == 3)
    {
        for(pos = run->tail_pos; pos < run->run_size + run->tail_pos; pos++)
        {
            run->tail_store[i] = run->full_seq[pos];
            i++;
        }
    }
    //The odd byte will just be stored in a tail of size 1
    else
    {
        int val = 1;
        int i;
        for(i = 0; i < run->odd_pos; i++)
        {
            val *= 2;
        }

        if(run->fill_bit == 0)
        {
            
            run->tail_store[0] = val;
        }
        else
        {
            run->tail_store[0] = 255 - val;
        }
    }
}


//will produce a blank activeRun struct with all values set to zero
activeRun *initDefRun()
{
    activeRun *run = (activeRun*) malloc(sizeof(activeRun));

    run->header = 0;
    run->run_type = 0;
    //run->run_seq;
    //run->full_seq;
    run->tail_len = 0;
    run->fill_len = 0;
    run->run_pos = 0;
    run->tail_pos = 0;
    run->run_size = 0;
    run->tail_store = 0;


    return run;
}

/*
    Param:
        byte *run:  The sequnce of compressed bytes to store in the active run
        int seq_size:   The size of run
        int run_start:  The position of the header for the current run
    return:
        returns an activeRun * 
*/
activeRun *initActiveRun(byte *run, int seq_size, int run_start){



    //need to allocate memory for the active run on the stack
    activeRun *curr_run = (activeRun*) malloc(sizeof(activeRun));

    //store the run_pos
    curr_run->run_pos = run_start;
    curr_run->header = run[run_start];

    //storing the run in the acitve run
    curr_run->full_seq = malloc(sizeof(byte) * seq_size);  //allocate memroy for the sequence
    curr_run->full_seq = run;

    //finding the type for the run
    curr_run->run_type = getHeadType(run[run_start]);


    //init counter_len to zero
    curr_run->counter_len = 0;

    //if the type is 3 or 4 then counter bytes follow the header byte.
    //need to read the counter bytes
    
    //printf("here\n");
    if(curr_run->run_type == 1)
    {
    	//getting the fill_bit for type 1
    	byte bit_temp = run[run_start] << 1;
    	bit_temp >>=7;
    	curr_run->fill_bit = bit_temp;

    	//getting fill_len for type 1
    	byte fill_temp = run[run_start] << 2;
    	fill_temp >>= 6;
    	curr_run->fill_len = fill_temp;

    	//getting tail_len for type 1
    	byte tail_temp = run[run_start] << 4;
    	tail_temp >>= 4;
    	curr_run->tail_len = tail_temp;

        //getting the physical run size
        curr_run->run_size = 1 + curr_run->tail_len;

        //getting tail_pos for type 1
        curr_run->tail_pos = curr_run->run_pos+1;

        store_tail(curr_run);

    }
    else if(curr_run->run_type == 2)
    {
    	//getting the fill_bit for type 2
    	byte bit_temp = run[run_start] << 2;
    	bit_temp >>=7;
    	curr_run->fill_bit = bit_temp;

    	//getting fill_len for type 2
    	byte fill_temp = run[run_start] << 3;
    	fill_temp >>= 6;
    	curr_run->fill_len = fill_temp;

    	//getting odd_pos for type 1
    	byte odd_temp = run[run_start] << 5;
    	odd_temp >>= 5;
    	curr_run->odd_pos = odd_temp;

        //setting tail length to 1 due to odd byte
        curr_run->tail_len = 1;

        //getting tail_pos for type 2
        curr_run->tail_pos = curr_run->run_pos+1;

        //run_size is made 2 because the odd byte has to be made a tail to work for the query engine
        //so even though it is just a header a single tail byte is added
        curr_run->run_size = 2;

        store_tail(curr_run);

    }
    else if(curr_run->run_type == 3)
    {
    	//getting fill_bit for type 3
    	byte bit_temp = run[run_start] << 3;
    	bit_temp >>= 7;
    	curr_run->fill_bit = bit_temp;



    	//getting fill_len for type 3
        curr_run->fill_len = counterBytes(run, curr_run);

     	//getting tail_len for type 3
    	byte tail_temp = run[run_start] << 4;
    	tail_temp >>= 4;
    	curr_run->tail_len = tail_temp;   

        //adding the tail len to run size
        curr_run->run_size += curr_run->tail_len;

        store_tail(curr_run);
    }
    else if(curr_run->run_type == 4)
    {
    	//getting fill_bit for type 4
		byte bit_temp = run[run_start] << 4;
    	bit_temp >>= 7;
    	curr_run->fill_bit = bit_temp;

    	//getting fill_len for type 4
        curr_run->fill_len = counterBytes(run, curr_run);

		//getting odd_pos for type 4
    	byte odd_temp = run[run_start] << 5;
    	odd_temp >>= 5;
    	curr_run->odd_pos = odd_temp;    

        //setting the tail len to 1 due to odd byte
        curr_run->tail_len =1;

        //increase run_size by 1 for the tail storing the odd byte
        //like type 2 "should" be no tail but need to add odd byte to tail to work with query engine
        curr_run->run_size++; 

        store_tail(curr_run);   
    }
    
    return curr_run;

}



void printActiveRun(activeRun * run)
{
	printf("The contents of the active run \n");
	printf("header: %u\n",run->header);
	printf("run type: %u\n", run->run_type);
	printf("fill bit: %u\n", run->fill_bit);
	printf("fill length: %u\n", run->fill_len);
    printf("tail length: %u\n", run->tail_len);
	if(run->run_type == 1 || run->run_type == 3)
	{
		
        printf("tail sequnce is: [ ");
        int i;
        for(i = 0; i < run->tail_len; i++)
        {
            printf("%d: (%u)", i, run->tail_store[i]);
            if(i < run->tail_len-1)
            {
                printf(", ");
            }
        }
        printf(" ]\n");
	}
	else if(run->run_type == 2 || run->run_type == 4)
	{
		printf("odd position: %u\n", run->odd_pos);
        printf("odd byte value (stored in tail): %u\n", run->tail_store[0]);
	}

    if(run->run_type == 3 || run->run_type == 4)
    {
        printf("Counter Byte length is: %u\n", run->counter_len);
    }

    printf("\n");
}

