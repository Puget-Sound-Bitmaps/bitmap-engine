#ifndef BLOCKSEG_H_
#define BLOCKSEG_H_

#include "Control.h"

//struct that holds uncompressed data to send to compressor
typedef struct blockSeg{
	word_read *toCompress;//the current block seg of words
	int size;//number of words in toCompress
	int colNum;//column num compressed
	struct blockSeg *next;//next one to be compressed
	FILE *colFile;//where all the compressed words are going
	word_32 curr;//the latest compressed
	word_32 stored;
	int status;//first/last block of column? or not valid block (empty)?


	////////////////////////////////////////////////////////////////
	/*									Below section is for BBC									*/
	////////////////////////////////////////////////////////////////

	byte *compressBytes; //the current block of uncompressed bytes
	//struct blockBytes *nextBlock; //the next block of bytes to compress
	unsigned int fill_len;
	unsigned int tail_len;
	unsigned int run_type;
	byte fill_match; //either 0 or 1 depending on if we're a
	byte fill_bit;
	byte next_byte;
	unsigned int byte_type;
	byte header;
	byte *curr_run; /*the current run (an array of bytes, in case we have literals, counter bytes, etc.)*/
	unsigned int curr_size; //the length of the curr_run array

} blockSeg;

#endif /* BLOCKSEG_H_ */
