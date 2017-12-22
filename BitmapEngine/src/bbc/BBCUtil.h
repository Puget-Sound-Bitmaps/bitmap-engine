//bbcUtil.h

#define ZERO_BYTE 0
#define ONE_BYTE 1
#define ODD_BYTE 2
#define MESSY_BYTE 3

#define TYPE_1 0
#define TYPE_2 1
#define TYPE_3 2
#define TYPE_4 3

#define FILL_LIMIT 3 //maybe????
#define TAIL_LIMIT 15 //maybe could be 16

//typedef unsigned char byte;

typedef struct blockSegBBC {
	unsigned int size;  //the size of the current block
	byte *compressBytes; //the current block of uncompressed bytes
	struct blockBytes *nextBlock; //the next block of bytes to compress
	unsigned int fill_len;
	unsigned int tail_len;
	unsigned int run_type;
	byte fill_match; //either 0 or 1 depending on if we're a
	byte fill_bit;
	FILE *colFile; //the file we are writing our compressed bytes to
	byte next_byte;
	unsigned int byte_type;
	byte header;
	byte *curr_run; /*the current run (an array of bytes, in case we have literals, counter bytes, etc.)*/
	unsigned int curr_size; //the length of the curr_run array
} blockSegBBC;

void startNewRun(struct blockSeg *param);

void changeRunType(unsigned int run_type, struct blockSeg *param);

void placeOddBit(struct blockSeg *param);

void incrementFill(struct blockSeg *param);

void getByteType(struct blockSeg *param);

void incrementTail(struct blockSeg *param);
