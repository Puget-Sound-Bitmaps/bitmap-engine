#include "Core.h"

/**
 * Runs formatter/compressor/query engines as set in Control.h
 *
 * format:      F   bitmap_file
 * compress:    C   bitmap_file     striped/unstriped   num_threads
 * query:       Q   bitmap_path     query_file          num_threads
 */
int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    /* FORMAT */
    if (argc == 3 && strcmp(argv[1], "F") == 0)
    {
        char* bitmap_file = argv[2];

        int reformatted = reformat( & bitmap_file);

        if (reformatted == 0)
        {
            printf("Unsuccessful reformatting of %s\n", bitmap_file);
        }
    }

    /* COMPRESS */
    else if (argc == 5 && strcmp(argv[1], "C") == 0)
    {
        int num_threads = atoi(argv[4]);
        if (num_threads < 1) return -1;

        char* format = argv[3];
        int striped = strcmp(format, "STRIPED");
        int unstriped = strcmp(format, "UNSTRIPED");

        if (striped != 0 && unstriped != 0) return -1;

        char* bitmap_file = argv[2];

        char results_name[BUFF_SIZE];
        /* where the results are being stored */
        snprintf(results_name, BUFF_SIZE, "%s_RESULTS.csv", bitmap_file);

        double time;

        /* run compression here */
        if (strcmp(format, "UNSTRIPED") == 0)
        {
            time = compress(bitmap_file, UNSTRIPED, BBC, n);
        }
        else
        {
            time = compress(bitmap_file, STRIPED, WAH, n);
        }

        printf("time: %f...", time);
        /* open result file (appending to end) */
        FILE *results_file = fopen(results_name, "a");
        if (results_file == NULL)
        {
            printf("Failed to open results file %s\n", results_name);
            return 0;
        }
        /* write result to file */
        fprintf(results_file, "%f,", time);
        fclose(results_file);
    }

    /* QUERY */
    else if (argc == 5 && strcmp(argv[1], "Q") == 0)
    {
        char* bitmap_path = argv[2];
        char* query_file = argv[3];
        int num_threads = atoi(argv[4]);

        if (num_threads < 1) return -1;

        runQueries(bitmap_path, query_file, num_threads);
    }

    return 0;
}
