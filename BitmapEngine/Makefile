SRC := src
TST := tst
BIN := bin

bitmap_out := ../BitmapWorkloadGenerator/bin/bitmap_out.txt
query_out := ../BitmapWorkloadGenerator/bin/query_out.txt

all: clean main format compress_u compress_s

# FIXME queries cause segmentation faults
# query_s \
# query_u \
# bintest \
# testquery \

clean:
	@echo "Setting Up Output Directory"
	@if test -d $(BIN); then rm -rf $(BIN)/*; else mkdir $(BIN); fi

main:
	@echo "Compiling main"
	@gcc -Wall -o $(BIN)/main \
		$(SRC)/main.c \
		$(SRC)/active/word/ActiveWord.c \
		$(SRC)/clock/Clock.c \
		$(SRC)/query/Query.c \
		$(SRC)/query/QueryUtil.c \
		$(SRC)/raw-bitmap-reader/RawBitmapReader.c \
		$(SRC)/seg-util/SegUtil.c \
		$(SRC)/val/VALCompressor.c \
		$(SRC)/val/VALQuery.c \
		$(SRC)/wah/WAHCompressor.c \
		$(SRC)/wah/WAHQuery.c \
		$(SRC)/writer/Writer.c \
		$(SRC)/bbc/BBCUtil.c \
		$(SRC)/bbc/BBCCompressor.c \
		$(SRC)/active/run/ActiveRun.c \
		$(SRC)/bbc/BBCQuery.c \
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

bintest:
	@echo "Compiling `bintest` Program"
	@gcc -Wall -o $(BIN)/bintest $(TST)/binarytest.c

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

compressor_run:
	@echo "Compiling compressor run"
	@gcc -Wall -o $(BIN)/compressor_run \
		$(SRC)/Compressor_Run.c \
		-lpthread -lm
