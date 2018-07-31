# The source directory.
SRC := src
# The binary output directory.
BIN := bin
# The java source files.
JSOURCE := \
	$(SRC)/DataGenerator.java \
	$(SRC)/GreyCodeSorter.java \
	$(SRC)/QueryGenerator.java \
	$(SRC)/WorkloadGenerator.java \
	$(SRC)/WorkloadMain.java \
	$(SRC)/Zipf.java

# Set up the output folder and generate the test data.
all: clean compile run

# Empty the bin/ directory if it exists,
# otherwise make the bin/ directory.
clean:
	@echo "Setting Up Output Directory"
	@if test -d $(BIN); then rm -rf $(BIN)/*; else mkdir $(BIN); fi

# Compile the java source files in the src/ directory
# to java class files in bin/ directory.
compile:
	@echo "Compiling Java Source"
	@javac -d $(BIN) $(JSOURCE)

# Run WorkloadMain to generate sample data.
run:
	@echo "Running Java Executable"
	@cd $(BIN) && java WorkloadMain

convert: $(BIN)/mb2sb
	mkdir -p $(BIN)/vecs
	cd $(BIN) && ./mb2sb

$(BIN)/mb2sb: $(SRC)/mb2sb.c
	gcc -Wall -o $(BIN)/mb2sb $(SRC)/mb2sb.c
