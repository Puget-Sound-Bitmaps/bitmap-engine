//BBCUtil.c

//#include "BBCUtil.h"
#include "../Core.h"


//transates a byte with a single '1' into a 3-bit representation of that bit's position
//inserts those three bits into the appropriate header
void placeOddBit(struct blockSeg *param) {
    //next_byte is one of eight bytes
    byte the_byte = param->next_byte;
    unsigned int i = 0;
    unsigned int pos = 0;
    //shift right until the number == 1
    if(param->fill_match == 0) {
        for(i = 0; i < 8; i++) {
            if(the_byte == 1) {
                pos = i;
                break;
            }
            else {
                the_byte >>= 1;
            }
        }
    }
    else {
        for(i = 0; i < 8; i++) {
            if(the_byte%2==0) {
                pos = i;
                break;
            }
            else {
                the_byte >>= 1;
            }
        }
    }
    //printf("current header (about to place odd bit) %x\n", param->curr_run[0]);
    param->curr_run[0] |= pos;
    //param->header = 0; //setting the header to 0 ensures a new run will on the start next loop of BBCCompressor.c
    //return param->header;
}

//gets the type of a RAW byte
void getByteType(struct blockSeg *param) {
    byte b = param->next_byte;
    if(param->next_byte == 0) {
        param->byte_type = ZERO_BYTE;
        param->fill_match = 0;
    }
    else if(param->next_byte == 255) {
        param->byte_type = ONE_BYTE;
        param->fill_match = 1;
        //param->byte_type = 1;
    }
    //if fill bit == 0, then we can use the ODD OBE BYTE. if the fill bit == 1, then we can use the ODD ZERO BYTE.
    //what if we are starting a new run? (NO FILL BIT DEFINED YET).
    //then we can attach either type of ODD BYTE to the end of a 1-bit-long type 2 run, and adjust the fill bit accordingly.
    //checks to see if there is
    //if(param->fill_bit == 0){
    else if(b == 1 || b == 2 || b == 4 || b == 8 || b == 16 || b == 32 || b == 64 || b == 128) {
        param->byte_type =  ODD_BYTE;
        param->fill_match = 0;
    }
    //}
    else if(b == 254 || b == 253 || b == 251 || b ==247 || b ==239 || b ==223 || b == 191 || b == 127) {
        param->byte_type = ODD_BYTE;
        param->fill_match = 1;
    }

    else {
        param->byte_type = MESSY_BYTE;
    }
}

//This function starts a new run based on the type of byte we have

void startNewRun(struct blockSeg *param) {

    //first we should write out the current array of chars to file,
    //and also free the memory from that array
    //printf("curr_size = %x in startnewrun\n", param->curr_size);
    //printf("current header %x in startnewrun\n", param->curr_run[0]);
    //This ensures that we aren't starting from the very first byte of the block
    //otherwise this would write a 0-byte to the file before anything else.
    if(param->curr_run[0] != 0) {
        printf("***************WRITING OUT**************** (startnewrun)\n");
        fwrite(param->curr_run, sizeof(byte), param->curr_size+1, param->colFile);
    }
    //free(param->curr_run);
    param->curr_size = 0;
    //fwrite(param->curr_run, sizeof(char), param->curr_size, param->colFile);
    //ZERO FILL
    //there's only one possible byte we should produce TYPE_1
    if(param->byte_type == ZERO_BYTE)
    {
        param->header = 0b10010000;
        param->fill_len = 1;
        param->tail_len = 0;
        param->run_type = TYPE_1;
        param->fill_bit = 0;
        param->curr_run[0] = param->header;
    }
    //ONE_FILL
    //there's only one possible byte we should produce TYPE_1
    if(param->byte_type == ONE_BYTE)
    {
        param->header = 0b11010000;
        param->fill_len = 1;
        param->tail_len = 0;
        param->run_type = TYPE_1;
        param->fill_bit = 1;
        param->curr_run[0] = param->header;
    }
    //ODD BYTE
    if(param->byte_type == ODD_BYTE)
    {
        //make (and end) type 2 run with the odd bit stored in the header
        if(param->fill_match == 0)
            param->header = 0b01000000;
        else {
            param->header = 0b01100000;
        }
        param->curr_run[0] = param->header;
        //here we decide the last three bits of the above binary number
        placeOddBit(param);
    }
    //MESSY BYTE... for example, 01101010
    if(param->byte_type == MESSY_BYTE)
    {
        //start off with a type 1 run, fill_length = 0.
        param->header = 0b10000001;
        //store the header byte and the literal in the run
        param->curr_run[0] = param->header;
        param->curr_size = 1;
        param->curr_run[param->curr_size] = param->next_byte; //storing the literal
        param->fill_len = 0;
        param->tail_len = 1;
        param->run_type = TYPE_1;
    }
}

//changes the run type (either type 1 to 2, 3 to 4, or 1 to 3)
void changeRunType(unsigned int run_type, struct blockSeg *param) {
    //printf("changing run type\n");
    /*this only happens if we are already a TYPE_1 run AND the
    tail length is 0 (i.e. going from type 1 to type 2)*/
    if(run_type == TYPE_2) {
        //we want to preserve: fill bit and the fill_length.
        //placeOddBit is called in BBCompressor.c
        //shift right to fit TYPE_2 format
        param->curr_run[0] >>= 1;
        param->run_type = TYPE_2; //do we even need this?
    }
    // if a type 1 ever max's out the fill_length then it must be changed to a type 3
    else if(run_type == TYPE_3)
    {
        //right now: header is: 0b1X110000
        //fill length is now '4'
        byte temp_bit = param->fill_bit;
        temp_bit <<= 4;
        param->header = 0b00100000; //'001' + fill_bit + tail_length //counter bytes will follow
        param->header |= temp_bit; //set the fill bit in the new header
        param->curr_run[0] = param->header;
        //update the struct
        param->run_type = TYPE_3;
        //param->fill_len = 4;
        param->curr_size++; //increments size of byte array to allow for first counter byte
        param->curr_run[param->curr_size] = (byte)param->fill_len; //sets counter byte to fill_len
    }

    /*this only happens if we are already a TYPE_3 run AND the
    tail length is 0 (i.e. going from type 3 to type 4)*/
    else if(run_type == TYPE_4) {
        //we want to preserve: fill bytes and the fill_length.
        //001X0000
        param->run_type = TYPE_4; //do we even need this?
        param->curr_run[0] >>= 1;
        //param->header = '0001' + fill_bit + odd_bit_pos  //counter bytes will follow
    }

}

//increments the fill length in the header
//increments the fill length in a type 1 run
//increments the counter bytes in a type 3 run
void incrementFill(struct blockSeg *param) {
    //increments fill
    byte newhead;
    //printf("incrementing fill\n");
    if(param->run_type == TYPE_1) {
        param->fill_len++;
        byte temp_fill = (byte)param->fill_len;
        temp_fill <<= 4;
        //temp >>= 6;
        //temp = temp + 1;
        //temp <<= 6;
        if(param->fill_bit == 0)
            newhead = 0b10000000;
        else
            newhead = 0b11000000;
        newhead |= temp_fill;
        param->header = newhead;
        param->curr_run[0] = newhead;
    }

    //increments counter bytes
    if(param->run_type == TYPE_3) {
        //continue incrementing current counter byte
        if(param->curr_run[param->curr_size] < 127) {
            //printf("incrementing counter byte\n");
            param->curr_run[param->curr_size]++;
            //printf("param->curr_run[param->curr_size] = %x\n", param->curr_run[param->curr_size]);
            //printf("param_curr_run[0] = %x\n", param->curr_run[0]);
            //printf("param_curr_run[1] = %x\n", param->curr_run[1]);
            //printf("param->curr_size = %d\n", param->curr_size);
            param->fill_len++;
        }
        //make new counter byte
        else {
            //add a 1 to the counter byte to keep with proper BBC format
            byte help = 0b10000000;
            param->curr_run[param->curr_size] |= help;
            param->curr_size++;
            param->curr_run[param->curr_size] = 1;
            param->fill_len++;
        }
    }
}


//increases tail_len in header by one
//concatenates a literal byte to the tail of curr_run
//This funtion should only be used for types 1 and 3 where the tail length is guaranteed to be the last 4 bits of the header
void incrementTail(struct blockSeg *param) {

    if(param->run_type == TYPE_2 || param->run_type == TYPE_4)
    {
        printf("ERROR: trying to incrementing tail when run type is of type 2 or 4!!!");
    }
    else
    {
        param->tail_len++;
        byte temp = param->tail_len;
        //temp <<= 4; //clear first half of temp, use left shift bitwise operation by 4
        //temp >>= 4; //move tail bits to LSBs position in temp, this way we can view the actual value of the tail length, use right shitf bitwise operation by 4
        param->header >>= 4; //clear out the old tail length bits from header
        param->header <<= 4; //shift it back
        param->header |= temp; //add the new tail length to header using an or bitwise operation
        param->curr_size++; //increment the length of the current run
        param->curr_run[0] = param->header;
        param->curr_run[param->curr_size] = param->next_byte; //concatenate the literal byte to the current run array
    }



}
