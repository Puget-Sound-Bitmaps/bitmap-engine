#include "Core.h"

/**
 * Runs formatter/compressor/query engines as set in Control.h
 *
 *
 * format:		F	BITMAP_FILE
 * compress:	C 	BITMAP_FILE		NUM_THREADS		STRIPED/UNSTRIPED
 * query: 		Q 	BITMAP_PATH		QUERY_FILE		NUM_THREADS
 *
 */
int main(int argc, char *argv[])
{

    setbuf(stdout, NULL);
    // TODO: this function is overloaded. I think rather than using flags to tell the function what to do,
    // move each function to a separate one to simplify the logic of this code.
    // e.g., bformat, bcompress, bquery

    char *engine_choice = argv[1];
    char *bitmap_path = argv[2];

    if (argc > 2 && (strcmp(argv[1], "F") == 0 || strcmp(argv[1], "C") == 0 || strcmp(argv[1], "Q") == 0))
    {

        if (argc == 3 && strcmp(argv[1], "F") == 0) // FORMATTING
        {
            printf("formatting recognized\n");
            if (reformat( & argv[2]) == 0)
            {
                printf("Unsuccessful reformatting of %s\n", argv[2]);
            }

        }
        else if (strcmp(argv[1], "C") == 0 && argc == 5) // COMPRESSION
        {
            int n = atoi(argv[3]); // number of threads
            if (n < 1) return -1;

            if (strcmp(argv[4], "STRIPED") != 0 && strcmp(argv[4], "UNSTRIPED") != 0) return -1;

            char results_name[BUFF_SIZE];
            snprintf(results_name, BUFF_SIZE, "%s_RESULTS.csv", argv[2]); // where the results are being stored

            double time;

            // run compression here
            if (strcmp(argv[4], "UNSTRIPED") == 0)
            {
                time = compress(argv[2], UNSTRIPED, BBC, n);
            } 
            else
            {
                time = compress(argv[2], STRIPED, WAH, n);
            }

            printf("time: %f...", time);
            FILE *results_file = fopen(results_name, "a"); // open result file (appending to end)
            if (results_file == NULL)
            {
                printf("Failed to open results file %s\n", results_name);
                return 0;
            }
            fprintf(results_file, "%f,", time); // write result to file
            fclose(results_file);

        }
        else if (strcmp(argv[1], "Q") == 0 && argc == 5) // make a query
        {
            int n = atoi(argv[4]);
            if (n < 1) return -1;

            // call run queries and pass in the bitmap compressed directory, the query_out.txt file path, and the number of threads (all provided as command line arguments)
            runQueries(argv[2], argv[3], atoi(argv[4]));

        }
    }

    return 0;
}