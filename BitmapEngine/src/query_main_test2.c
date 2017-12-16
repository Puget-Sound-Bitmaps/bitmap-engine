#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Core.h"

int main(int argc, char*argv[])
{

	//byte *ret = (byte*) malloc(sizeof(byte)*10000);

	byte *col1;
	unsigned int col1Size;
	int loopSize3;
	byte *or_uncomp;
	int malSize;
	byte *col2;

	unsigned int col2Size;

<<<<<<< HEAD:BitmapEngine/src/query_main_test2.c
	int option = 1; //use hard coded or read file
	int option2 = 7;
=======
	int option = 0; //use hard coded or read file
	int option2 = 0;
>>>>>>> b78626c5848d84fce9fe74280982ffaf15652a72:BitmapEngine/src/query_main_test.c

	if(!option)
	{
		if(option2 == 0)
		{
			byte test1[10];
			byte test2[10];

			test1[0] = 0b10111001;
			test2[0] = 0b11111001;


			col1Size = 10;
			col2Size = 10;

			int i;

			test1[1] = 0b10101011;
			test2[1] = 0b11010101;


			test1[2] = 0b00111010;
			test2[2] = 0b10110000;

			//
			test1[3] = 0b01110111;
			test2[3] = 0b10011010;

			test1[4] = 0b11001100;
			test2[4] = 0b00011011;

			test1[5] = 0b01000101;
			test2[5] = 0b10110101;

			test1[6] = 0b10101110;
			test2[6] = 0b11110111;

			test1[7] = 0b10010101;
			test2[7] = 0b10100011;

			test1[8] = 0b10110111;
			test2[8] = 0b10000001;

			test1[9] = 0b11111111;
			test2[9] = 0b11111111;


			/*for(i = 1; i < col1Size; i++)
			{
				test1[i] = 0;
				test2[i] = 0;
			}*/

			col1 = test1;
			col2 = test2;

			printf("%u\n",test1[0]);
			printf("%u\n", test1[1]);
			printf("OR of spot 1's is: %u\n", (test1[1] | test2[1]));
		}
		else if(option2 == 1)
		{
			char test1[1];
			char test2[1];

			test1[0] = 0b11110000;
			test2[0] = 0b11110000;

			col1Size = 1;
			col2Size = 1;

			col1 = test1;
			col2 = test2;
		}
		else if(option2 == 2)
		{
			char test1[11];
			char test2[11];

			test1[0] = 0b11110000;
			test2[0] = 0b11110000;

			test1[1] = 0b10111001;
			test2[1] = 0b11111001;

			test1[2] = 0b10101011;
			test2[2] = 0b11010101;

			test1[3] = 0b00111010;
			test2[3] = 0b10110000;

			test1[4] = 0b01110111;
			test2[4] = 0b10011010;

			test1[5] = 0b11001100;
			test2[5] = 0b00011011;

			test1[6] = 0b01000101;
			test2[6] = 0b10110101;

			test1[7] = 0b10101110;
			test2[7] = 0b11110111;

			test1[8] = 0b10010101;
			test2[8] = 0b10100011;

			test1[9] = 0b10110111;
			test2[9] = 0b10000001;

			test1[10] = 0b11111111;
			test2[10] = 0b11111111;



			col1Size = 11;
			col2Size = 11;

			col1 = test1;
			col2 = test2;
		}
		else if(option2 == 3)
		{
			char test1[1];
			char test2[2];

			test1[0] = 0b01110011;
			test2[0] = 0b11100001;

			test2[1] = 0b10010101;

			col1Size = 1;
			col2Size = 2;

			col1 = test1;
			col2 = test2;
		}

		else if(option2 == 4)
		{
			char test1[2];
			char test2[17];

			//run
			test1[0] = 0b00010000;
			test1[1] = 0b00010100;
			//test1[2] = 0b10101010;
			//test1[3] = 0b01000111;
			//test1[4] = 0b00100101;
			//test1[5] = 0b01101001;
			//test1[6] = 0b10010101;
			//test1[7] = 0b11101101;
			//test1[8] = 0b00010001;
			//test1[9] = 0b10010010;
			//test1[10] = 0b10101000;
			//test1[11] = 0b00011100;
			//test1[12] = 0b11001110;
			//test1[13] = 0b01101101;
			//test1[14] = 0b01101100;
			//test1[15] = 0b01101111;
			//test1[16] = 0b10010011;

			//run
			test2[0] = 0b01010111;

			//run
			test2[1] = 0b11110010;
			test2[2] = 0b10101101;
			test2[3] = 0b00111011;

			//run
			test2[4] = 0b10000101;
			test2[5] = 0b10101011;
			test2[6] = 0b11010101;
			test2[7] = 0b10000110;
			test2[8] = 0b00110000;
			test2[9] = 0b11101101;

			//run
			test2[10] = 0b10100110;
			test2[11] = 0b10010100;
			test2[12] = 0b10001110;
			test2[13] = 0b01110111;
			test2[14] = 0b10010100;
			test2[15] = 0b11111111;
			test2[16] = 0b11000001;
			

			col1Size = 2;
			col2Size = 17;

			col1 = test1;
			col2 = test2;
		}
	}
	else
	{
		system("python binhextrans.py query_test_data1.txt bin 1");
		system("python binhextrans.py query_test_data2.txt bin 2");
		system("./bbc_test_main 1");

		//system("python binhextrans.py uncompressed_OR.dat hex 3");
		//system("python binhextrans.py bbc_test_output1.dat hex 4");
		//system("python binhextrans.py bbc_test_output2.dat hex 5");

		FILE *inPtr1 = fopen("bbc_test_output1.dat", "rb");
	    FILE *inPtr2 = fopen("bbc_test_output2.dat", "rb");
	    FILE *inPtr3 = fopen("uncompressed_OR.dat", "rb");

	    fseek(inPtr1, 0L, SEEK_END);
	    fseek(inPtr2, 0L, SEEK_END);
	    fseek(inPtr3, 0L, SEEK_END);
	    
	    int loopSize1 = ftell(inPtr1);
	    int loopSize2 = ftell(inPtr2);
	    loopSize3 = ftell(inPtr3);

	    rewind(inPtr1);
	    rewind(inPtr2);
	    rewind(inPtr3);
	    int i;
	    col1 = (byte*) malloc(sizeof(byte)*loopSize1);
	    col2 = (byte*) malloc(sizeof(byte)*loopSize2);
	    or_uncomp = (byte*) malloc(sizeof(byte)*loopSize3);
	    for(i = 0; i < loopSize3; i++){
	    	fread(or_uncomp, 1, loopSize3, inPtr3);
	    }
	   	for(i = 0; i < loopSize3; i++){
	    	printf("d\n", or_uncomp[i]);
	    }
	   	printf("loopsize: %d\n", loopSize1);

	    for(i = 0; i < loopSize1; i++)
	    {
	      	fread(col1, 1, loopSize1, inPtr1);
	    }
	   	for(i = 0; i < loopSize2; i++)
	    {
	      	fread(col2, 1, loopSize2, inPtr2);
	    }

		col1Size = loopSize1;
		col2Size = loopSize2;
		if(col1Size > col2Size){
			malSize = col1Size;
		}
		else{
			malSize = col2Size;
		}
	}
	byte *ret = (byte*) malloc(sizeof(byte)*malSize);
	int ret_size = OR_BBC(ret, col1, col1Size, col2, col2Size);
	printf("ret_size %u\n", ret_size);
	int i;
	FILE *or_query = fopen("query_OR.dat", "wb");
	fwrite(ret, sizeof(byte), malSize+1, or_query);
	printf("Result of query: \n");
	for(i = 0; i < ret_size; i++)
	{
		printf("%u", ret[i]);
		if(i < ret_size -1)
		{
			printf(", ");
		}
		
	}
	printf("\n\n\n");
	if(option){
		printf("or uncomp \n");
		for(i = 0; i < ret_size; i++)
		{
			printf("%u", or_uncomp[i]);
			if(i < loopSize3 - 1)
			{
				printf(", ");
			}
		}
	}
	printf("\n");
	free(or_uncomp);
	free(col1);
	free(col2);
	free(ret);

}