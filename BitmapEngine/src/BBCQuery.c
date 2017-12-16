#include <stdio.h>
#include <stdlib.h>
#include "Core.h"
#include "ActiveRun.h"

int OR_BBC(byte * ret, byte * col1, unsigned int size1, byte * col2, unsigned int size2)
{
    //will tell Query_BBC to perform OR operation on the two compressed seqs
    return Query_BBC(ret, col1, size1, col2, size2, 0);
}

int AND_BBC(byte * ret, byte * col1, unsigned int size1, byte * col2, unsigned int size2)
{   
    //will tell Query_BBC to perform AND operation on the two compressed seqs
    return Query_BBC(ret, col1, size1, col2, size2, 1);
}

int Query_BBC(byte * ret, byte * col1, unsigned int size1, byte * col2, unsigned int size2, int type)
{
    printf("-----------QUERY IN PROCESS-------------\n");
    printf("\n");
    //The header position in col1 of run1
    int pos1 = 0;

    //The header position in col2 of run2
    int pos2 = 0;

    //the position we are in in our output array
    int out_pos = 0;
    
    //The first active run
    activeRun *run1 = initDefRun();
    /*run1 = initActiveRun(col1, size1, pos1);
    pos1++;*/

    //The second active run
    activeRun *run2 = initDefRun();
    /*run2 = initActiveRun(col2, size2, pos2);
    pos2++;*/

    //int k = 0;
    while(pos1 < size1 && pos2 < size2)
    {
        printf("\n----------------------\n");
        printf("new run is starting\n");
        printf("----------------------\n");
        printf("pos1: %u pos2: %u\n", pos1, pos2);

        //If run1 is empty than initiate the next run from col1
        if(run1->tail_len == 0 && run1->fill_len == 0)
        {
            free(run1);
            run1 = initActiveRun(col1, size1, pos1);
            printf("For run1:\n");
            printActiveRun(run1);
            //eat up the header byte in col1
            pos1++;
        }
        //If run2 is empty thatn intiate the next run form col2
        if(run2->tail_len == 0 && run2->fill_len == 0)
        {
            free(run2);
            run2 = initActiveRun(col2, size2, pos2);
            printf("For run2:\n");
            printActiveRun(run2);
            //eat up the header byte in col2
            pos2++;
        }
        int tail_pos1 = 0;
        int tail_pos2 = 0;


        //While run1 and run2 are not empty keep operating
        while((run1->tail_len != 0 || run1->fill_len != 0) && (run2->tail_len != 0 || run2->fill_len != 0))
        {
            printf("\n");
            printf("run1 fill_len: %u tail_len: %u\n", run1->fill_len, run1->tail_len);
            printf("run2 fill_len: %u tail_len: %u\n", run2->fill_len, run2->tail_len);
            printf("pos1: %u pos2: %u\n", pos1, pos2);
            //fill OR fill
            if(run1->fill_len > 0 && run2->fill_len > 0){
                int lookAhead = 0;
                if(run1->fill_len > run2->fill_len)
                {
                    lookAhead = run2->fill_len;
                }
                else
                {
                    lookAhead = run1->fill_len;
                }

                int i;
                //0 fill OR 0 fill
                //0 fill AND 0 fill
                if(run1->fill_bit == 0 && run2->fill_bit == 0){
                   
                    printf("0 fill OR 0 fill\n");
                    
                    for(i = 0; i < lookAhead; i++)
                    {
                        //ERROR here, need to mallloc the *ret variable to the size
                        //of the column(s) we are compressing. This should be done 
                        //a tthe top of this file. 
                        ret[out_pos + i] = 0b00000000;
                        printf("or op is: %u\n", ret[out_pos + i]);
                    }
                }
                //1 fill OR 1 fill
                //1 fill AND 1 fill
                else if(run1->fill_bit == 1 && run2->fill_bit ==1)
                {
                    printf("1 fill OR 1 fill\n");
                    for(i = 0; i < lookAhead; i++)
                    {
                        ret[out_pos + i] = 0b11111111;
                        printf("or op is: %u\n", ret[out_pos + i]);
                    }
                }
                //0 fill OR 1 fill
                //0 fill AND 1 fill
                else
                {
                    printf("0 fill OR 1 fill\n");
                    for(i = 0; i < lookAhead; i++)
                    {
                        if(type == 0)
                        {
                            ret[out_pos + i] = 0b11111111;
                            printf("or op is: %u\n", ret[out_pos + i]);
                        }
                        else if(type == 1)
                        {
                            ret[out_pos + i] = 0b00000000;
                        }
                    }
                }
                out_pos += i;
                //pos1 += i;
                //pos2 += i;
                run1->fill_len -= lookAhead;
                run2->fill_len -= lookAhead;
            }
            //fill OR messy
            else if(run1->fill_len > 0 && run2->fill_len == 0){
                //0 fill and messy
                printf("fill OR messy\n");
                if(run1->fill_bit == 0){
                    if(type == 0)
                    {
                        ret[out_pos] = 0b00000000 | run2->tail_store[tail_pos2]; 
                        printf("run1 is 0 run2 is %u result of or op is: %u\n", run2->tail_store[tail_pos2], ret[out_pos]);
                    }
                    else if(type == 1)
                    {
                        ret[out_pos] = 0b00000000 & run2->tail_store[tail_pos2];
                    }                
                }
                //1 fill and messy
                else{
                    if(type == 0)
                    {
                        ret[out_pos] = 0b11111111 | run2->tail_store[tail_pos2];
                        printf("run1 is 1 run2 is %u result of or op is: %u\n", run2->tail_store[tail_pos2], ret[out_pos]);
                    }
                    else if(type == 1)
                    {
                        ret[out_pos] = 0b11111111 & run2->tail_store[tail_pos2];
                    }
                }
                out_pos++;
                //pos1++;
                if(run2->run_type == 1 || run2->run_type == 3)
                {
                    pos2++;
                }
                tail_pos2++;
                run1->fill_len--;
                run2->tail_len--;
            }
            //messy OR fill
            else if(run2->fill_len > 0 && run1->fill_len == 0){
                printf("messy OR fill\n");
                //0 fill and messy
                if(run1->fill_bit == 0){
                    if(type == 0)
                    {
                        ret[out_pos] = 0b00000000 | run1->tail_store[tail_pos1];
                        printf("run2 is 0 run1 is %u result of or op is: %u\n", run1->tail_store[tail_pos1], ret[out_pos]);        
                    }
                    else if(type == 1)
                    {
                        ret[out_pos] = 0b00000000 & run1->tail_store[tail_pos1]; 
                    }         
                }
                //1 fill and messy
                else{
                    if(type == 0)
                    {
                        ret[out_pos] = 0b11111111 | run1->tail_store[tail_pos1];
                        printf("run1 is 1 run2 is %u result of or op is: %u\n", run2->tail_store[tail_pos1], ret[out_pos]);
                    }
                    else if(type == 1)
                    {
                        ret[out_pos] = 0b11111111 & run1->tail_store[tail_pos1];
                    }
                }
                out_pos++;
                
                if(run1->run_type == 1 || run1->run_type == 3)
                {
                    pos1++;
                }
                //pos2++;
                tail_pos1++;
                run2->fill_len--;
                run1->tail_len--;
            }
            //messy OR messy
            else{
                printf("messy OR messy\n");
                if(type == 0)
                {
                    ret[out_pos] = run1->tail_store[tail_pos1] | run2->tail_store[tail_pos2];
                    printf("run1 is %u run2 is %u result of or op is: %u\n", run1->tail_store[tail_pos1], run2->tail_store[tail_pos2], ret[out_pos]);
                }
                else if(type == 1)
                {
                    ret[out_pos] = run1->tail_store[tail_pos1] & run2->tail_store[tail_pos2];
                }
                out_pos++;

                if(run1->run_type == 1 || run1->run_type == 3)
                {
                    pos1++;
                }
                if(run2->run_type == 1 || run2->run_type == 3)
                {
                    pos2++;
                }

                tail_pos1++;
                tail_pos2++;
                run1->tail_len--;
                run2->tail_len--;
            }

            //increment pos1 if there are counter bytes
            if((run1->run_type == 3 || run1->run_type == 4) && run1->fill_len <= 0 && run1->tail_len <= 0)
            {
                pos1 += run1->counter_len;
            }
            //increment pos2 if there are counter bytes
            if((run2->run_type == 3 || run2->run_type == 4) && run2->fill_len <= 0 && run2->tail_len <= 0)
            {
                pos2 += run2->counter_len;
            }

            printf("pos1: %u pos2: %u\n", pos1, pos2);
        }

        //If run1 is empty than update pos1 to the header postion of the next run

        /*if(run1->tail_len == 0 && run1->fill_len == 0)
        {
            free(run1);
        }
        
        //If run2 is empty than update pos2 to the header positon of the next run

        if(run2->tail_len == 0 && run2->fill_len == 0)
        {
            free(run2);
        }*/
        
    //If on success than return 1
    }

    printf("------------QUERY DONE----------\n");
    return out_pos;
}

/*int AND_BBC(byte * ret, byte * col1, unsigned int size1, byte * col2, unsigned int size2)
{
    return 1;
}*/
