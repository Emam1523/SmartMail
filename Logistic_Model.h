#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_EMAILS 2350
#define VOCAB_SIZE 550
#define LEARNING_RATE 0.1
#define EPOCHS 1500

double emails[MAX_EMAILS][VOCAB_SIZE];
int labels[MAX_EMAILS];

double weights[VOCAB_SIZE] = {0};
double bias = 0;

/*Sigmoid function*/
double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

/* Predict the probability of an email being spam*/
double predict(double *email) {
    double z = bias;
    for (int i = 0; i < VOCAB_SIZE; i++) {
        z += weights[i] * email[i];
    }
    return sigmoid(z);
}

/*Compute the log-likelihood (negative log-likelihood for minimization)*/
double log_likelihood() {
    double log_likelihood_value = 0;
    for (int i = 0; i < MAX_EMAILS; i++) {
        double y_pred = predict(emails[i]);
        log_likelihood_value += labels[i] * log(y_pred) + (1 - labels[i]) * log(1 - y_pred);
    }
    return -log_likelihood_value / MAX_EMAILS;
}

/*Train the logistic regression model using gradient descent*/
void train() {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double dw[VOCAB_SIZE] = {0}; 
        double db = 0;  

        for (int i = 0; i < MAX_EMAILS; i++) {
            double y_pred = predict(emails[i]);
            double error = y_pred - labels[i];

            for (int j = 0; j < VOCAB_SIZE; j++) {
                dw[j] += error * emails[i][j];
            }

            db += error;
        }

        for (int j = 0; j < VOCAB_SIZE; j++) {
            weights[j] -= LEARNING_RATE * dw[j] / MAX_EMAILS;
        }
        bias -= LEARNING_RATE * db / MAX_EMAILS;

        if (epoch % 100 == 0) {
            double loss = log_likelihood();
            //printf("Epoch %d, Loss: %f, dw[0]: %f, db: %f\n", epoch, loss, dw[0], db);
        }
    }
}

/* This function loads the dataset from a CSV file*/
void load_dataset(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[6002];
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error: File is empty or missing header\n");
        fclose(file);
        exit(1);
    }

    int row_count = 0;
    while (fgets(line, sizeof(line), file) && row_count < MAX_EMAILS) {
        char *token = strtok(line, ",\n");
        if (!token) {
            printf("Error: No label at row %d\n", row_count);
            continue;
        }

        labels[row_count] = atoi(token);

        int col = 0;
        while (col < VOCAB_SIZE) { 
            token = strtok(NULL, ",\n");
            emails[row_count][col] = (token != NULL) ? atof(token) : 0.0;
            col++;
        }

        row_count++;
    }

    fclose(file);
}

/*Save the trained model (weights and bias) to a file*/
void save_model(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < VOCAB_SIZE; i++) {
        if (i == VOCAB_SIZE - 1) {
            fprintf(file, "%lf", weights[i]); 
        } else {
            fprintf(file, "%lf,", weights[i]);
        }
    }

    fprintf(file, ",%lf\n", bias);
    
    fclose(file);
}

/*Evaluate the model using the test dataset*/
void evaluate_model(const char *test_file) {
    double (*test_emails)[VOCAB_SIZE] = (double (*)[VOCAB_SIZE])malloc(MAX_EMAILS * sizeof(*test_emails));
    int *test_labels = (int *)malloc(MAX_EMAILS * sizeof(int));
    if (!test_emails || !test_labels) {
        perror("Failed to allocate memory for test data");
        free(test_emails);
        free(test_labels);
        exit(1);
    }

    int test_row_count = 0;
    FILE *file = fopen(test_file, "r");
    if (!file) {
        free(test_emails);
        free(test_labels);
        perror("Error opening test file");
        exit(1);
    }

    char line[6002];
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error: Test file is empty or missing header\n");
        fclose(file);
        free(test_emails);
        free(test_labels);
        exit(1);
    }

    while (fgets(line, sizeof(line), file) && test_row_count < MAX_EMAILS) {
        char *token = strtok(line, ",\n");
        if (!token) {
            printf("Error: No label at test row %d\n", test_row_count);
            continue;
        }

        test_labels[test_row_count] = atoi(token);

        int col = 0;
        while ((token = strtok(NULL, ",\n")) != NULL && col < VOCAB_SIZE) {
            test_emails[test_row_count][col] = atof(token);
            col++;
        }

        if (col != VOCAB_SIZE) {
            printf("Error: Test row %d has %d features (expected %d)\n", test_row_count, col, VOCAB_SIZE);
            continue;
        }

        test_row_count++;
    }

    fclose(file);

    if (test_row_count == 0) {
        printf("Error: No test data loaded\n");
        free(test_emails);
        free(test_labels);
        exit(1);
    }

    int correct = 0;
    for (int i = 0; i < test_row_count; i++) {
        double y_pred = predict(test_emails[i]);
        int prediction = (y_pred >= 0.5) ? 1 : 0;
        if (prediction == test_labels[i]) {
            correct++;
        }
    }

    double accuracy = (double)correct / test_row_count;
    //printf("Test Accuracy: %.2f%%\n", accuracy * 100);

    free(test_emails);
    free(test_labels);
}

/*Main function to train the model and evaluate it on test data*/
void logistic_model(const char *train_file, const char *test_file) {
    if (train_file == NULL || test_file == NULL) {
        printf("Error: Invalid file paths.\n");
        return;
    }

    load_dataset(train_file);

    train();

    save_model("logistic_model.txt");

    evaluate_model(test_file);

    //printf("Model trained, saved, and evaluated successfully!\n");
}

