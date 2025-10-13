#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH3 1000
#define MAX_EMAILS 2350

void split_dataset(const char *input_file, const char *train_file, const char *test_file, float train_ratio) {
    FILE *input = fopen(input_file, "r");
    FILE *train_output = fopen(train_file, "w");
    FILE *test_output = fopen(test_file, "w");

    if (!input || !train_output || !test_output) {
        printf("Error: Unable to open one or more files.\n");
        return;
    }

    char line[MAX_LINE_LENGTH3];
    int total_lines = 0;

    fgets(line, sizeof(line), input);  
    while (fgets(line, sizeof(line), input)) {
        total_lines++;
    }
    rewind(input); 

    int train_count = (int)(total_lines * train_ratio);
    int test_count = total_lines - train_count;

    fgets(line, sizeof(line), input);
    fprintf(train_output, "%s", line);
    fprintf(test_output, "%s", line);

    int line_num = 0;
    while (fgets(line, sizeof(line), input)) {
        if (line_num < train_count) {
            fprintf(train_output, "%s", line);
        } else {
            fprintf(test_output, "%s", line);
        }
        line_num++;
    }

    fclose(input);
    fclose(train_output);
    fclose(test_output);

    // printf("Dataset split completed.\n");
    // printf("Training dataset: %s (%d emails)\n", train_file, train_count);
    // printf("Testing dataset: %s (%d emails)\n", test_file, test_count);
}
