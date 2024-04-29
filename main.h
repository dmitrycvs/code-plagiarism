#ifndef MAIN_H
#define MAIN_H

#define MAX_WORD_LENGTH 100

typedef struct
{
    char word[MAX_WORD_LENGTH];
    int freq;
} WordFreq;

WordFreq *createWordFreqArray(const char *str, int *size);
double cosineSimilarity(const WordFreq *array1, int size1, const WordFreq *array2, int size2);
void extractFunctionInformation(TSNode node, char *code, HashMap **arr, unsigned *arr_index);
void extractDataFromDatabase(HashMap **arr, unsigned *arr_index, WordFreq *ArrWordFreq[], int *size1, int *size2);

#endif /* MAIN_H */