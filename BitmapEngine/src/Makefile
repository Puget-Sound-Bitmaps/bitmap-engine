# A note to aingerson, por que no Makefile?
all :
	gcc -Wall -c ActiveWord.c
	gcc -Wall -c Clock.c
	gcc -Wall -c Query.c
	gcc -Wall -c QueryUtil.c
	gcc -Wall -c RawBitmapReader.c
	gcc -Wall -c SegUtil.c
	gcc -Wall -c VALCompressor.c
	gcc -Wall -c VALQuery.c
	gcc -Wall -c WAHCompressor.c
	gcc -Wall -c WAHQuery.c
	gcc -Wall -c Writer.c
	gcc -Wall -c BBCUtil.c
	gcc -Wall -c BBCCompressor.c
	gcc -Wall -c ActiveRun.c
	gcc -Wall -c BBCQuery.c
	gcc -Wall -o main main.c ActiveWord.o Clock.o Query.o QueryUtil.o RawBitmapReader.o SegUtil.o VALCompressor.o VALQuery.o WAHCompressor.o WAHQuery.o Writer.o BBCUtil.o BBCCompressor.o ActiveRun.o BBCQuery.o -lpthread -lm

# format bitmap
# $ ./main F <bitmap_file>
format :
	./main F ../../BitmapWorkloadGenerator/src/bitmap_out.txt

# unstriped compression, single thread
# $ ./main C <bitmap_file> <num_threads> UNSTRIPED
compress_u :
	./main C ../../BitmapWorkloadGenerator/src/bitmap_out.txt 1 UNSTRIPED

# striped compression, single thread
# $ ./main C <bitmap_file> <num_threads> STRIPED
compress_s :
	./main C ../../BitmapWorkloadGenerator/src/bitmap_out.txt 1 STRIPED

# query unstriped
# $ ./main Q <BITMAP_PATH> <QUERY_FILE> <NUM_THREADS>
query_u :
	./main Q ../../BitmapWorkloadGenerator/src/bitmap_out.txt_UNSTRIPED_1_COMPRESSED ../../BitmapWorkloadGenerator/src/query_out.txt 1

# query striped
# $ ./main Q <BITMAP_PATH> <QUERY_FILE> <NUM_THREADS>
query_s :
	./main Q ../../BitmapWorkloadGenerator/src/bitmap_out.txt_STRIPED_1_COMPRESSED ../../BitmapWorkloadGenerator/src/query_out.txt 1

clean:
	rm main
	rm *.o

bintest:
	#gcc -Wall -c ActiveRun.c
	gcc -Wall -o bintest binarytest.c #ActiveRun.o

cleantest:
	rm -f *.o bintest
	rm -f *.o 

bbcquery:
	gcc -Wall -c BBCQuery.c
	gcc -Wall -c ActiveRun.c 

bbctest:
	gcc -Wall -c BBCCompressor.c
	gcc -Wall -c BBCUtil.c
	gcc -Wall -c bbc_test_main.c
	gcc -Wall -o bbc_test_main BBCCompressor.o BBCUtil.o bbc_test_main.o

testquery:
	gcc -Wall -c ActiveWord.c
	gcc -Wall -c Clock.c
	gcc -Wall -c Query.c
	gcc -Wall -c QueryUtil.c
	gcc -Wall -c RawBitmapReader.c
	gcc -Wall -c SegUtil.c
	gcc -Wall -c VALCompressor.c
	gcc -Wall -c VALQuery.c
	gcc -Wall -c WAHCompressor.c
	gcc -Wall -c WAHQuery.c
	gcc -Wall -c Writer.c
	gcc -Wall -c BBCUtil.c
	gcc -Wall -c BBCCompressor.c
	gcc -Wall -c ActiveRun.c
	gcc -Wall -c BBCQuery.c
	gcc -Wall -o mainquery query_main_test.c ActiveWord.o Clock.o Query.o QueryUtil.o RawBitmapReader.o SegUtil.o VALCompressor.o VALQuery.o WAHCompressor.o WAHQuery.o Writer.o BBCUtil.o BBCCompressor.o ActiveRun.o BBCQuery.o -lpthread -lm


testquery2:
	gcc -Wall -c ActiveWord.c
	gcc -Wall -c Clock.c
	gcc -Wall -c Query.c
	gcc -Wall -c QueryUtil.c
	gcc -Wall -c RawBitmapReader.c
	gcc -Wall -c SegUtil.c
	gcc -Wall -c VALCompressor.c
	gcc -Wall -c VALQuery.c
	gcc -Wall -c WAHCompressor.c
	gcc -Wall -c WAHQuery.c
	gcc -Wall -c Writer.c
	gcc -Wall -c BBCUtil.c
	gcc -Wall -c BBCCompressor.c
	gcc -Wall -c ActiveRun.c
	gcc -Wall -c BBCQuery.c
	gcc -Wall -o mainquery2 query_main_test2.c ActiveWord.o Clock.o Query.o QueryUtil.o RawBitmapReader.o SegUtil.o VALCompressor.o VALQuery.o WAHCompressor.o WAHQuery.o Writer.o BBCUtil.o BBCCompressor.o ActiveRun.o BBCQuery.o -lpthread -lm