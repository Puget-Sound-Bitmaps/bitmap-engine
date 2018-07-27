#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{
    size_t line_length = 0;
    uint64_t word = 0;

    long int row = 0;
    long int vector = 0;
    long int word_len = 64;
    long int num_rows = 1048576;
    long int num_vecs = 50000;

    char inputfile[] = "bitmap_out.txt";

    char fileform[] = "vecs/v_%d.dat";
    char filename[16];
    char hex[2 + (word_len / 4) + 1];
    char hexform[] = "0x%016lx\n";
    memset(filename, '\0', 16);
    char *line = NULL;
    char *line_ptr = NULL;
    char **vec_buf = malloc(sizeof(char *) * num_vecs);

    FILE **vec_out = malloc(sizeof(FILE *) * 50000);
    FILE *input = fopen(inputfile, "r");
    if (input == NULL)
    {
        printf("Could not open file: %s\n", inputfile);
        return 1;
    }

    // Open file pointers and create buffers
    puts("Opening file pointers and creating buffers.");
    for (vector = 0; vector < num_vecs; ++vector)
    {
        sprintf(filename, fileform, vector);
        vec_out[vector] = fopen(filename, "w");
        if (vec_out[vector] == NULL)
        {
            printf("File creation failed.\n");
            return 1;
        }

        vec_buf[vector] = malloc(sizeof(char) * (word_len + 1));
        memset(vec_buf[vector], ' ', word_len);
        vec_buf[vector][word_len] = '\0';
        memset(filename, '\0', 16);
    }

    // Process file
    puts("Processing file.");
    for (row = 0; row < num_rows; ++row)
    {
        getline(&line, &line_length, input);

        for (vector = 0; vector < num_vecs; ++vector)
        {
            // Grab value
            vec_buf[vector][row % word_len] = line_ptr[vector];

            // If the buffer is full, convert to number and write to file.
            if ((row + 1) % word_len == 0)
            {
                word = strtoull(vec_buf[vector], NULL, 2);
                sprintf(hex, hexform, word);
                fprintf(vec_out[vector], "%s", hex);
            }
        }

        if (row % word_len == 0)
            printf("Finished line %ld.\n", row);
    }

    // Close file pointers and destroy buffers
    puts("Closing file pointers and destroying buffers.");
    for (vector = 0; vector < num_vecs; ++vector)
    {
        fclose(vec_out[vector]);
        free(vec_buf[vector]);
    }

    fclose(input);
    free(vec_out);
    free(vec_buf);
    free(line);

    return 0;
}
