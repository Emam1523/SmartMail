#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE_LENGTH1 200
#define MAX_EMAILS 2350
#define STOP_WORDS_COUNT 115

const char *stop_words[STOP_WORDS_COUNT] = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your","he","she","it","they","them","their","him","her","his","am","is","are","was","were","be","been","have","has","would","should","will","shall","had","do","does","did","can","could","may","might","the","a","an","and","or","but","if","of","for","with","at","by","from","up","down","in","out","on","then","once","here","there","when","where","why","how","all","any","both","each","few","more","most","other","some","such","no","nor","not","only","own","same","so","than","too","very","s","t","can","will","just","don","should","now","ve","y","ain","aren","couldn","didn","doesn","hadn","hasn","haven","isn","ma","mightn","mustn","needn","shan","shouldn","wasn","weren","won","wouldn"};

typedef struct {
    int label1;       
    char email1[MAX_LINE_LENGTH1];
} Email1;

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

bool is_stop_word(const char *word) {
    for (int i = 0; i < STOP_WORDS_COUNT; i++) {
        if (strcmp(word, stop_words[i]) == 0) {
            return true;
        }
    }
    return false;
}

void remove_punctuation(char *str) {
    int i = 0;
    while (str[i]) {
        if (ispunct((unsigned char)str[i])) {
            memmove(&str[i], &str[i + 1], strlen(&str[i]));
        } else {
            i++;
        }
    }
}

void remove_special_characters_and_numbers(char *str) {
    int i = 0;
    while (str[i]) {
        if (!isalnum((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            memmove(&str[i], &str[i + 1], strlen(&str[i]));
        } else if (isdigit((unsigned char)str[i])) {
            memmove(&str[i], &str[i + 1], strlen(&str[i]));
        } else {
            i++;
        }
    }
}

void stem_word(char *word) {
    int length1 = strlen(word);
    if (length1 > 3) {
        if (strcmp(&word[length1 - 3], "ing") == 0) {
            word[length1 - 3] = '\0';  
        } else if (strcmp(&word[length1 - 2], "ed") == 0) {
            word[length1 - 2] = '\0'; 
        }
    }
}

void preprocess_email(char *email, char *processed_email) {
    char temp_email[MAX_LINE_LENGTH];
    strcpy(temp_email, email);  

    to_lowercase(temp_email);

    remove_special_characters_and_numbers(temp_email);

    char *token = strtok(temp_email, " ");
    bool first_word = true;

    while (token != NULL) {
        if (!is_stop_word(token)) {
            stem_word(token);
            if (!first_word) {
                strcat(processed_email, " ");
            }
            strcat(processed_email, token);
            first_word = false;
        }
        token = strtok(NULL, " ");
    }
}

void write_processed_data_to_csv(Email1 *emails, int email_count) {
    FILE *output = fopen("Preprocessed_Dataset.csv", "w");
    if (!output) {
        printf("Error: Could not create output file.\n");
        return;
    }

    fprintf(output, "label,Message\n");

    for (int i = 0; i < email_count; i++) {
        char processed_email[MAX_LINE_LENGTH] = "";
        preprocess_email(emails[i].email1, processed_email);
        fprintf(output, "%d,%s\n", emails[i].label1, processed_email);
    }

    fclose(output);
}

int preprocessing(FILE *file2) {
    Email1 emails1[MAX_EMAILS];
    int email_count = 0;
    char line[MAX_LINE_LENGTH];

    fgets(line, sizeof(line), file2);

    while (fgets(line, sizeof(line), file2) && email_count < MAX_EMAILS) {
        char *token = strtok(line, ",");
        if (!token) continue;

        emails1[email_count].label1 = atoi(token);

        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(emails1[email_count].email1, token, MAX_LINE_LENGTH);

        email_count++;
    }

    fclose(file2);

    write_processed_data_to_csv(emails1, email_count);
    return email_count;
}

