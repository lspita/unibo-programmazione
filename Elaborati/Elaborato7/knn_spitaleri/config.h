#ifndef CONFIG_H
#define CONFIG_H

// User defined
#define DATASET "dataset.csv"  // path to dataset
#define CSV_HEADER true        // whether the csv has the first line with column names
#define K 10                   // the value of K
#define TRAINING_PERCENTAGE 75 // percentage of dataset to use as training
#define N_CATEGORIES 2         // number of possible categories
#define PAUSE false            // whether to stop to see each result

// Private
#define BUFFER_SIZE 1024 // size of buffer to contain csv column value

#endif