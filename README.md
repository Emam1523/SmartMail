# SPL1 Project - Spam Detection in C

## Overview
This project implements a complete spam detection pipeline in C using a logistic regression model.

The program:
1. Merges two raw datasets.
2. Performs exploratory data analysis (EDA).
3. Preprocesses text (lowercasing, stop-word removal, simple stemming).
4. Vectorizes messages using a generated vocabulary.
5. Splits data into training and testing sets.
6. Trains and evaluates a logistic regression model.
7. Classifies messages from a test message file.

## Tech Stack
- Language: C
- Compiler: GCC
- Math library: libm (linked with -lm)

## Project Files
### Main source
- main.c: Runs the full end-to-end pipeline.
- Header.h: Includes all module headers.

### Module headers
- EDA.h: Exploratory data analysis and dataset statistics.
- Preprocessing.h: Text cleaning and preprocessing.
- Vectorization.h: Vocabulary creation and feature vector generation.
- Split_Vectorized_Dataset.h: Train/test split.
- Logistic_Model.h: Logistic regression training and evaluation.
- test_message.h: Message classification using trained model.

### Input files
- DATASET.txt
- New_DATASET.txt
- Test_Message.txt

### Generated files
- merged_dataset.txt
- EDA.txt
- Preprocessed_Dataset.csv
- vocabulary.txt
- Vectorized_Dataset.csv
- Training_Dataset.csv
- Testing_Dataset.csv
- logistic_model.txt

## Expected Data Format
The raw dataset files should contain one message per line in this format:

1, "example spam message"
0, "example ham message"

Where:
- 1 = spam
- 0 = ham

## Build
From the project root:

gcc main.c -o spam_classifier -lm

On Windows, this creates spam_classifier.exe.

## Run
From the project root:

./spam_classifier

On Windows PowerShell, this also works:

.\spam_classifier.exe

## Program Flow
1. Merge DATASET.txt and New_DATASET.txt into merged_dataset.txt
2. Run EDA and write report to EDA.txt
3. Preprocess merged data into Preprocessed_Dataset.csv
4. Build vocabulary and vectorized dataset
5. Split into Training_Dataset.csv and Testing_Dataset.csv (80/20)
6. Train logistic regression and save model to logistic_model.txt
7. Read Test_Message.txt and print spam probability + prediction

## Notes
- Keep all input files in the same directory as the executable.
- File names are hardcoded in main.c and module functions.
- The vocabulary/model size is set to 550 features.

## How To Test With Your Own Messages
1. Open Test_Message.txt
2. Add one message per line
3. Save the file
4. Run the program again

Each line will be classified as SPAM or HAM with probability output.
