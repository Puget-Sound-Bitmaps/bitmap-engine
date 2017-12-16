/**Revisted Psudeo Code**/
#include "Core.h"


void bbcCompress(struct blockSeg *param){

  //these methods gather information from the header
  int i;
  param->curr_run = (byte*) malloc(sizeof(byte)*100);
  param->curr_size = 0;
  printf("finna print?\n");
  for(i = 0; i < param->size; i++)
  {
    //printf("starting bbccompress\n");
    //these functions should go in rawbitmapreader.c, for each column there should be a new file.
    //sprintf(compfile, "compressed_%d", i);
    //param->colFile = fopen("filewrite/compressed%d.txt", i, "w");
    int j;
    for(j = 0; j < param->size; j++){
      //printf("ls %d\n", param->toCompress[j]);
    }
    param->next_byte = param->toCompress[i];//get the next byte from the block sequence of bytes
    //printf("next byte (#%d): %x\n", i, param->next_byte);

    getByteType(param);//get the type of next_byte: zero byte, one byte, odd byte ect ect

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /**
  ***FUNCTION DOCUMENTATION***
        //Run Types (as Described in our documentation and BBC paper)
        //'0' = TYPE_1
        //'1' = TYPE_2
        //'2' = TYPE_3
        //'3' = TYPE_4

        //takes a byte and decides what "type" of byte it is
        //Will either be ZERO_BYTE, ONE_BYTE, ODD_BYTE, MESSY_BYTE
        getType(next byte)

        //increases the tail length in the header and concatenates the messy literal bit to the tail of the run
        //@param char next_byte The literal byte to be concatenated
        incrementTail(char next_byte)

        //increments the fill length in the header
        incrementFill();

        //changes the current run type to the desired header type
        changeRunType(int type);

        //increments the counter bytes in a type 2 or type 4 run
        incrementCounterByte();

        //write current run to the compressed outfile
        //set header to 0
        //start new run, create new header
        startNewRun(byte_type);

        //creates the 3 bits in type 2 or type 4 header to represent odd bit in last byte of tail
        placeOddBit(byte next_byte);

        //in the case where after a tail, we run into an odd byte
        //places an odd byte header (type 2) in the compressed data before starting a new run (defaulting to a blank type 1 header)
        makeOddHeader();
  */
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    //0-fill byte or 1-fill byte (11111111 or 00000000)
    //printf("param->header before first if %x\n", param->header);

    //When a type 2 or type 4 is reached no new bytes can be added after the odd bit is placed so no matter what the new byte is a new run wll always be needed
    if(param->run_type == TYPE_2 || param->run_type == TYPE_4)
    {
      //printf("SETTING HEADER TO 0 TYPE 2 OR 4\n");
      param->header = 0;
    }

    if(param->header != 0){
      if(param->byte_type == ZERO_BYTE || param->byte_type == ONE_BYTE){

        //proper type of fill (0 or 1)

        if(param->fill_match == param->fill_bit){
         //printf("fill match = fill bit\n");
          //if we are currently in a TYPE_1 run
          if(param->run_type == TYPE_1){

            //we're not too long yet, so stay as a TYPE_1 and incrememt
            if(param->fill_len < FILL_LIMIT){
              incrementFill(param);

            }
            else{ //otherwise, change to type 3 run
              changeRunType(TYPE_3, param);
              incrementFill(param);
            }
          }
          else if(param->run_type == TYPE_3) //if we are currently in a TYPE_3 run
            //increment the counter bytes
            incrementFill(param);

        }
        else{ //if it's not the right kind of fill, start a new run
          param->header = 0;
          //startNewRun(param);
        }
      //odd byte (Eg: 00010000 or 111110111)
      }
      else if(param->byte_type==ODD_BYTE){ //if we have an odd one or zero bit in an otherwise fill worthy byte
        /*if the tail_length is 0, we can easily change to
        //FILL_MATCH to determine the value
        either a TYPE_2 or TYPE_4 run by concatenating the
        ODD_BYTE to the end of our fill. */
        if(param->tail_len == 0 && param->fill_match == param->fill_bit){

          //if we are a TYPE_1 run
          if(param->run_type == TYPE_1){

            //change ourselves to a TYPE_2 run
            //this will end the current run
            changeRunType(TYPE_2, param);
            placeOddBit(param);

          //if we are a TYPE_3 run
          }
          else if(param->run_type == TYPE_3){

            //change ourselves to a TYPE_4 run
            //this will end the current run
            changeRunType(TYPE_4, param);
            placeOddBit(param);
          }
        /*if we already have a tail, we must start a new run using
        the ODD_BYTE*/
        //the startNewRun() function checks for this specific case
        }
        else{
          param->header = 0;
          //startNewRun(param);
        }
      }
      else if(param->byte_type==MESSY_BYTE){ //messy byte (Eg: 11010100)

        //if we are a TYPE_1 run
        if(param->run_type == TYPE_1 || param->run_type == TYPE_3){

          if(param->tail_len < TAIL_LIMIT){

            //if we aren't too long yet, add the
            //messy bit to the tail and increment the tail length
            incrementTail(param);
          }

          /*if we already have a tail, we must start a new run using
          the ODD_BYTE*/
          //the startNewRun() function checks for this specific case

          else{
            param->header = 0;
            //startNewRun(param);
          }
        }
      }
    }
    //if the header is 0 and we're not on the last word of the block, start a new run.
    //printf("param->header before second if :::: %x\n", param->header);
    //if the header is 0 and we're not on the last word of the block, start a new run.
    if(param->header == 0 ){ //if any part of the above code caused a new run to be need this will start that new run //&& i != param->size-1
      //printf("param->header was 0?\n");
      startNewRun(param);
    }
    else{
      //for type 2 and 4 runs
    }
  }
  printf("param->header is %x\n", param->header);
  printf("param->size is %x\n", param->size);
  printf("writing final run, curr_size %x\n", param->curr_size);
  printf("sizeof byte is %x\n",(unsigned int) sizeof(byte));
  printf("WRITING OUT(file end) \n");
  fwrite(param->curr_run, sizeof(byte), param->curr_size+1, param->colFile);
  fclose(param->colFile);

  free(param->curr_run);
}
