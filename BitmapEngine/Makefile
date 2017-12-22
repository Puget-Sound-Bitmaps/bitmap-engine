SRC := src
BIN := bin

bitmap_out := ../BitmapWorkloadGenerator/bin/bitmap_out.txt
query_out := ../BitmapWorkloadGenerator/bin/query_out.txt

all: \
	clean \
	main \
	format \
	compress_u \
	compress_s \
	query_u \
	query_s \
	# bintest \
	# testquery \

clean:
	@echo "Setting Up Output Directory"
	@if test -d $(BIN); then rm -rf $(BIN)/*; else mkdir $(BIN); fi

main:
	@echo "Compiling `main` Program"
	@gcc -Wall -o $(BIN)/main \
		$(SRC)/main.c \
		$(SRC)/ActiveWord.c \
		$(SRC)/Clock.c \
		$(SRC)/Query.c \
		$(SRC)/QueryUtil.c \
		$(SRC)/RawBitmapReader.c \
		$(SRC)/SegUtil.c \
		$(SRC)/VALCompressor.c \
		$(SRC)/VALQuery.c \
		$(SRC)/WAHCompressor.c \
		$(SRC)/WAHQuery.c \
		$(SRC)/Writer.c \
		$(SRC)/BBCUtil.c \
		$(SRC)/BBCCompressor.c \
		$(SRC)/ActiveRun.c \
		$(SRC)/BBCQuery.c \
		-lpthread -lm

# format bitmap
# $ ./main F <bitmap_file>
format:
	@echo "Formatting Bitmap Data"
	@$(BIN)/main F $(bitmap_out)

# unstriped compression, single thread
# $ ./main C <bitmap_file> <num_threads> UNSTRIPED
compress_u:
	@echo "Unstriped Compression; Single Thread"
	@$(BIN)/main C $(bitmap_out) 1 UNSTRIPED

# striped compression, single thread
# $ ./main C <bitmap_file> <num_threads> STRIPED
compress_s:
	@echo "Striped Compression; Single Thread"
	@$(BIN)/main C $(bitmap_out) 1 STRIPED

# query unstriped
# $ ./main Q <BITMAP_PATH> <QUERY_FILE> <NUM_THREADS>
query_u:
	@echo "Unstriped Query"
	@$(BIN)/main Q \
		$(bitmap_out)_UNSTRIPED_1_COMPRESSED \
		$(query_out) \
		1

# query striped
# $ ./main Q <BITMAP_PATH> <QUERY_FILE> <NUM_THREADS>
query_s:
	@echo "Striped Query"
	@$(BIN)/main Q \
		$(bitmap_out)_STRIPED_1_COMPRESSED \
		$(query_out) \
		1

# NOTHING BELOW HERE IS REFERENCED IN THE README

bintest:
	@echo "Compiling `bintest` Program"
	@gcc -Wall -o $(BIN)/bintest $(SRC)/binarytest.c

testquery:
	@echo "Compiling `mainquery` Program"
	@gcc -Wall -o $(BIN)/mainquery \
		$(SRC)/query_main_test.c \
		$(SRC)/ActiveWord.c \
		$(SRC)/Clock.c \
		$(SRC)/Query.c \
		$(SRC)/QueryUtil.c \
		$(SRC)/RawBitmapReader.c \
		$(SRC)/SegUtil.c \
		$(SRC)/VALCompressor.c \
		$(SRC)/VALQuery.c \
		$(SRC)/WAHCompressor.c \
		$(SRC)/WAHQuery.c \
		$(SRC)/Writer.c \
		$(SRC)/BBCUtil.c \
		$(SRC)/BBCCompressor.c \
		$(SRC)/ActiveRun.c \
		$(SRC)/BBCQuery.c \
		-lpthread -lm
