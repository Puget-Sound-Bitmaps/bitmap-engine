#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "BlockSeg.h"
#include "BBCCompressor.h"

int main(int argc, char*argv[])
{
  printf("start of main\n");

  int manualSet;
  int query = 1;
  if(argc == 0)
  {
    manualSet = 1;
  }
  else
  {
    manualSet = 0;
  }
  manualSet = 0; //testing

  struct blockSeg *segtest1;
  struct blockSeg *segtest2;

  segtest1 = (blockSeg*) malloc(sizeof(blockSeg));
  segtest2 = (blockSeg*) malloc(sizeof(blockSeg));


/////////////////for possibly writing an actual text file and reading it///////////////
    /*segtest2->
  	int i;
	for(i=0;i<colsPerFile;i++){

		saving[i]->toCompress = (word_read *) malloc(sizeof(word_read)*(block/colsPerFile));
		saving[i]->size=fread(saving[i]->toCompress,sizeof(word_read),block/colsPerFile,toRead);*/
  if(manualSet)
  {

    int i;
    int tnum = 13;

    //TEST #0 (WORKING)
    //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    if(tnum == 0){
    	segtest1->size = 10;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*10);
    	for(i = 0; i < segtest1->size; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    }
    //TEST #1 (WORKING)
    //00000000 00000000 00000000 00010000
    else if(tnum == 1){
    	segtest1->size = 4;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*4);
    	for(i = 0; i < 4; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    	segtest1->toCompress[3] = 0b00010000;
    	/*for(i = 4; i < segtest1->size; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}*/
    }
    //TEST #2 (W0RKING)
    //00000000 00000000 00000000
    else if(tnum == 2){
    	segtest1->size = 3;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*3);
    	for(i = 0; i < 3; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    }

    //TEST #3 (WORKING)
    //we need to figure out when to set fill_bit and fill_match correctly
    //also made a change to incrementFill because it automatically was setting the fill bit to 0 (not what we want)
    //11111111 11111111 11111111
    else if(tnum == 3){
    	segtest1->size = 3;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*3);
    	for(i = 0; i < 3; i++)
    	{
    	  segtest1->toCompress[i] = 0b11111111;
    	}
    }

    //TEST #4 (WORKING)
    //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000010 00000000 00000000 00000000
    else if(tnum == 4){
    	segtest1->size = 14;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*14);
  	for(i = 0; i < 10; i++)
  	{
  	  segtest1->toCompress[i] = 0;
  	}
  	segtest1->toCompress[10] = 0b00000010;
  	for(i = 11; i < 14; i++)
  	{
  	  segtest1->toCompress[i] = 0;
  	}
    }

    //TEST #5 (WORKING)
    //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 11111111 11111111
    else if(tnum == 5){
    	segtest1->size = 13;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*13);
    	for(i = 0; i < 10; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    	segtest1->toCompress[10] = 0b00000000;
    	segtest1->toCompress[11] = 0b11111111;
    	segtest1->toCompress[12] = 0b11111111;

    }
    //TEST #6 (WORKING)
    //00000000 00000000 11111111 11111111
    else if(tnum == 6){
    	segtest1->size = 4;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*4);
    	for(i = 0; i < 2; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    	segtest1->toCompress[2] = 0b11111111;
    	segtest1->toCompress[3] = 0b11111111;

    }
    //TEST #7
    //11111111 11111111 11010010 11101011 (10000001)*13
    else if(tnum == 7){
    	segtest1->size = 19;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*19);
    	for(i = 0; i < 2; i++)
    	{
    	  segtest1->toCompress[i] = 0b11111111;
    	}
    	segtest1->toCompress[2] = 0b11010010;
    	for(i = 3; i < 6; i++)
    	{
    	  segtest1->toCompress[i] = 0b11101011;
    	}
    	for(i = 6; i < 19; i++)
    	{
    		segtest1->toCompress[i] = 0b10000001;
    	}
    }
    //TEST #8
    //00000000
    else if(tnum == 8){
    	segtest1->size = 1;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*1);
    	segtest1->toCompress[0] =  0b00000000;

    }
    //TEST #9
    //11110111
    else if(tnum == 9){
    	segtest1->size = 1;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*1);
    	segtest1->toCompress[0] =  0b11110111;

    }

    //TEST #10
    //11111111 11111111 11111111 11011111
    else if(tnum == 10){
    	segtest1->size = 4;
    	segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*4);
    	for(i = 0; i < 3; i++)
    	{
    		segtest1->toCompress[i] = 0b11111111;
    	}
    	segtest1->toCompress[3] =  0b11011111;

    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*                                                                                                         */
    /*This test combines all previous tests (test 0 -> test 10)                                                */
    /*                                                                                                         */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if(tnum == 11)
    {

      segtest1->size = 76;
      segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*segtest1->size);

      //TEST #0 (WORKING)
      //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
      int tempSize = 0;
      for(i = 0; i < 10; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}

      tempSize +=i;

      //TEST #1 (WORKING)
      //00000000 00000000 00000000 00010000
      for(i = 10; i < 14; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    	segtest1->toCompress[13] = 0b00010000;

      tempSize +=i;


      //TEST #2 (W0RKING)
      //00000000 00000000 00000000

      for(i = 14; i < 17; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}

      tempSize +=i;

      //TEST #3 (WORKING)
      //we need to figure out when to set fill_bit and fill_match correctly
      //also made a change to incrementFill because it automatically was setting the fill bit to 0 (not what we want)
      //11111111 11111111 11111111

      for(i = 17; i < 20; i++)
    	{
    	  segtest1->toCompress[i] = 0b11111111;
    	}

      tempSize +=1;

      //TEST #4 (WORKING)
      //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000010 00000000 00000000 00000000

      for(i = 20; i < 30; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    	segtest1->toCompress[30] = 0b00000010;
    	for(i = 31; i < 34; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}

      //TEST #5 (WORKING)
      //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 11111111 11111111

      for(i = 34; i < 45; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    	segtest1->toCompress[45] = 0b11111111;
    	segtest1->toCompress[46] = 0b11111111;


      //TEST #6 (WORKING)
      //00000000 00000000 11111111 11111111

      for(i = 47; i < 49; i++)
    	{
    	  segtest1->toCompress[i] = 0;
    	}
    	segtest1->toCompress[49] = 0b11111111;
    	segtest1->toCompress[50] = 0b11111111;

      //TEST #7
      //11111111 11111111 11010010 (11101011)*3 (10000001)*13

      for(i = 51; i < 53; i++)
    	{
    	  segtest1->toCompress[i] = 0b11111111;
    	}
    	segtest1->toCompress[53] = 0b11010010;
    	for(i = 54; i < 57; i++)
    	{
    	  segtest1->toCompress[i] = 0b11101011;
    	}
    	for(i = 57; i < 70; i++)
    	{
    		segtest1->toCompress[i] = 0b10000001;
    	}

      //TEST #8
      //00000000

      segtest1->toCompress[70] =  0b00000000;

      //TEST #9
      //11110111

      segtest1->toCompress[71] =  0b11110111;

      //TEST #10
      //11111111 11111111 11111111 11011111

      for(i = 72; i < 75; i++)
    	{
    		segtest1->toCompress[i] = 0b11111111;
    	}
    	segtest1->toCompress[75] =  0b11011111;
    }
    else if(tnum == 12)
    {
      segtest1->size = 170;
      segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*segtest1->size);

      for(i = 0; i < 150; i++)
      {
        segtest1->toCompress[i] = 0;
      }
      for(i = 150; i < 170; i++)
      {
        segtest1->toCompress[i] = 0b00001100;
      }
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////
  /*                                                                                            */
  /*This test is when a file of bytes to compress is read instead of hard coding in the bytes   */
  /*                                                                                            */
  ////////////////////////////////////////////////////////////////////////////////////////////////

  else if(query == 1)
  {
    ////QUERY TESTING
    //printf("about to open bbc_tester2.dat\n");
    char cwd[1024];
    char test1[1024];
    char test2[1024];
    char resFile[1024];
    //char test[100];
    getcwd(test1, sizeof(cwd));
    getcwd(test2, sizeof(cwd));
    getcwd(resFile, sizeof(cwd));
    strcat(test1, "/converted_test1.dat");
    strcat(test2, "/converted_test2.dat");
    
    printf("%s\n", test1);
    printf("%s\n", test2);
    //FILE *inPtr = fopen("C:/Users/bill/Documents/Summer2017/Bitmap Project/CompressionSummerResearch/Develop/Bitmap-Engine/BitmapWorkloadGenerator/src/bitmap_out.txt","rb");
    FILE *inPtr1 = fopen(test1, "rb");
    FILE *inPtr2 = fopen(test2, "rb");


    printf("about to read from testers \n");
    //int readSize = 8;
    int help = sizeof(inPtr1);
    printf("help: %d\n", help);
    int i;
    //int loopSize = 3000/8;
    fseek(inPtr1, 0L, SEEK_END);
    printf("about to read from testers \n");
    fseek(inPtr2, 0L, SEEK_END);
    int loopSize = ftell(inPtr1);
    //int loopSize2 = ftell(inPtr2);
    rewind(inPtr1);
    rewind(inPtr2);
    printf("about to read from testers \n");
    segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*loopSize);
    segtest1->size = loopSize;
    segtest2->toCompress = (word_read*) malloc(sizeof(word_read)*loopSize);
    segtest2->size = loopSize;
    word_read *uncomp_or_array = (word_read*) malloc(sizeof(word_read)*loopSize);
    printf("loopsize: %d\n", loopSize);
    for(i = 0; i < loopSize; i++)
    {
      //printf("inside loop\n");
      //unsigned char buff[1];
      fread(segtest1->toCompress, 1, loopSize, inPtr1);
      fread(segtest2->toCompress, 1, loopSize, inPtr2);
      //fread()
      //fgets(buff,readSize,inPtr);
      //segtest1->toCompress[i] = buff;
      //printf("%02x", segtest1->toCompress[i]);
    }
    for(i = 0; i < loopSize; i++){
      uncomp_or_array[i] = segtest1->toCompress[i] | segtest2->toCompress[i];
      printf("segtest1 at pos: %02x ", segtest1->toCompress[i]);
      printf("segtest2 at pos: %02x ", segtest2->toCompress[i]);
      printf("or_array at pos: %d\n", uncomp_or_array[i]);
    }
    printf("\nloop end in query\n");


  printf("here!!!!!!!!!!!!!\n");

  FILE *ptr1 = fopen("bbc_test_output1.dat","wb");
  FILE *ptr2 = fopen("bbc_test_output2.dat","wb");
  FILE *or_ptr = fopen("uncompressed_OR.dat", "wb");
  
  //FILE *readfrom2 = fopen("")
    printf("in final query \n");
    segtest1->colFile = ptr1;
    segtest2->colFile = ptr2;
    for(i = 0; i < 3000/8; i++)
    {
      //printf("%x!!!\n",segtest1->toCompress[i]);
    }
    printf("colFile is pointing\n");
    bbcCompress(segtest1);
    bbcCompress(segtest2);
    printf("writing to query uncompressed output file\n");
    fwrite(uncomp_or_array, sizeof(byte), loopSize+1, or_ptr);

    free(segtest1->toCompress);
    free(segtest1);
    free(segtest2->toCompress);
    free(segtest2);
    free(uncomp_or_array);

  }else{
    printf("about to open bbc_tester2.dat\n");
    char cwd[1024];
    //char test[100];
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/bbc_tester_2.dat");
    printf("%s\n", cwd);
    //FILE *inPtr = fopen("C:/Users/bill/Documents/Summer2017/Bitmap Project/CompressionSummerResearch/Develop/Bitmap-Engine/BitmapWorkloadGenerator/src/bitmap_out.txt","rb");
    FILE *inPtr = fopen(cwd,"rb");


    printf("about to read from bbc_tester2.dat\n");
    //int readSize = 8;
    int i;
    //int loopSize = 3000/8;
    fseek(inPtr, 0L, SEEK_END);
    int loopSize = ftell(inPtr);
    rewind(inPtr);

    segtest1->toCompress = (word_read*) malloc(sizeof(word_read)*loopSize);
    segtest1->size = loopSize;
    printf("loopsize: %d\n", loopSize);
    for(i = 0; i < loopSize; i++)
    {
      //printf("inside loop\n");
      //unsigned char buff[1];
      fread(segtest1->toCompress, 1, loopSize, inPtr);
      //fread()
      //fgets(buff,readSize,inPtr);
      //segtest1->toCompress[i] = buff;
      //printf("%02x", segtest1->toCompress[i]);
    }
    printf("\nloop end\n");
    FILE *ptr = fopen("bbc_test_output.dat","wb");
    segtest1->colFile = ptr;
    for(i = 0; i < 3000/8; i++)
    {
      //printf("%x!!!\n",segtest1->toCompress[i]);
    }
    printf("colFile is pointing\n");
    bbcCompress(segtest1);
    free(segtest1->toCompress);
    free(segtest1);
  }

}
