#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <tree_sitter/api.h>
#include <sqlite3.h>
#include "hashmap.h"
#include "main.h"

const TSLanguage *tree_sitter_cpp(void);

void extractFunctionInformation(TSNode node, char *code, HashMap **arr, unsigned *arr_index)
{
    HashMap *Map = arr[*arr_index];
    if (strcmp(ts_node_type(node), "function_definition") == 0)
    {
        TSNode function_type_node = ts_node_child(node, 0);
        uint32_t start_byte = ts_node_start_byte(function_type_node);
        uint32_t end_byte = ts_node_end_byte(function_type_node);

        const char *node_type = ts_node_type(function_type_node);
        char *function_type = (char *)malloc(20 * sizeof(char));
        if (function_type == NULL)
        {
            fprintf(stderr, "An error occurred while allocating memory!");
            exit(1);
        }

        snprintf(function_type, 20, "%.*s", end_byte - start_byte, code + start_byte);
        if (strcmp(function_type, "string") == 0 || strcmp(function_type, "char") == 0)
        {
            incrementCount(Map, "return_function_string");
        }
        else if (strcmp(function_type, "void") == 0)
        {
            incrementCount(Map, "return_function_void");
        }
        // For numbers
        else
        {
            incrementCount(Map, "return_function_number");
        }

        free(function_type);
    }
    else if (strcmp(ts_node_type(node), "for_statement") == 0 || strcmp(ts_node_type(node), "while_statement") == 0)
    {
        incrementCount(Map, "for_count");
    }
    else if (strcmp(ts_node_type(node), "if_statement") == 0)
    {
        incrementCount(Map, "if_count");
    }

    uint32_t child_count = ts_node_child_count(node);
    for (uint32_t i = 0; i < child_count; ++i)
    {
        TSNode child = ts_node_child(node, i);
        extractFunctionInformation(child, code, arr, arr_index);
    }
}

void extractDataFromDatabase(HashMap **arr, unsigned *arr_index, WordFreq *ArrWordFreq[], int *size1, int *size2)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    sqlite3_stmt *stmt;

    rc = sqlite3_open("inputs.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sql = "SELECT * FROM files";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const char *column_text = sqlite3_column_text(stmt, 1);
        if (column_text)
        {
            char *data = (char *)malloc((strlen(column_text) + 1) * sizeof(char));
            strcpy(data, column_text);

            TSParser *parser = ts_parser_new();
            ts_parser_set_language(parser, tree_sitter_cpp());
            TSTree *tree = ts_parser_parse_string(
                parser,
                NULL,
                data,
                strlen(data));

            TSNode root_node = ts_tree_root_node(tree);

            HashMap *Map = createHashMap();
            arr[*arr_index] = Map;
            extractFunctionInformation(root_node, data, arr, arr_index);
            if (*arr_index)
            {
                ArrWordFreq[0] = createWordFreqArray(data, size1);
            }
            else
            {
                ArrWordFreq[1] = createWordFreqArray(data, size2);
            }
            (*arr_index)++;

            ts_tree_delete(tree);
            ts_parser_delete(parser);
            free(data);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

WordFreq *createWordFreqArray(const char *str, int *size)
{
    *size = 0;
    int capacity = 10;
    WordFreq *array = (WordFreq *)calloc(capacity, sizeof(WordFreq));
    if (array == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(1);
    }

    char word[MAX_WORD_LENGTH];
    const char *delimiters = " ;()";
    char *token = strtok((char *)str, delimiters);
    while (token != NULL)
    {
        for (int i = 0; token[i]; i++)
        {
            word[i] = tolower(token[i]);
        }
        word[strlen(token)] = '\0';

        int found = 0;
        for (int i = 0; i < *size; i++)
        {
            if (strcmp(array[i].word, word) == 0)
            {
                array[i].freq++;
                found = 1;
                break;
            }
        }

        if (!found)
        {
            if (*size == capacity)
            {
                capacity *= 2;
                array = (WordFreq *)realloc(array, capacity * sizeof(WordFreq));
                if (array == NULL)
                {
                    fprintf(stderr, "Memory allocation error.\n");
                    exit(1);
                }
            }
            strcpy(array[*size].word, word);
            array[*size].freq = 1;
            (*size)++;
        }
        token = strtok(NULL, delimiters);
    }

    return array;
}

double cosineSimilarity(const WordFreq *array1, int size1, const WordFreq *array2, int size2)
{
    double dotProduct = 0.0, magA = 0.0, magB = 0.0;

    for (int i = 0; i < size1; i++)
    {
        magA += array1[i].freq * array1[i].freq;
        for (int j = 0; j < size2; j++)
        {
            if (strcmp(array1[i].word, array2[j].word) == 0)
            {
                dotProduct += array1[i].freq * array2[j].freq;
                break;
            }
        }
    }

    for (int i = 0; i < size2; i++)
    {
        magB += array2[i].freq * array2[i].freq;
    }

    magA = sqrt(magA);
    magB = sqrt(magB);

    if (magA == 0 || magB == 0)
    {
        return 0.0;
    }

    return dotProduct / (magA * magB);
}

int main()
{
    unsigned arr_size = 0;
    char *elements[] = {"return_function_string", "return_function_void", "return_function_number", "for_count", "if_count"};
    int num_elements = sizeof(elements) / sizeof(elements[0]);
    HashMap **array = (HashMap **)calloc(SIZE, sizeof(HashMap *));
    int size1, size2;
    WordFreq *arrayWordFreq[2];
    double ASTWeigth = 0;
    if (array == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    extractDataFromDatabase(array, &arr_size, arrayWordFreq, &size1, &size2);

    for (int i = 0; i < num_elements; i++)
    {
        int count1 = atoi(searchInHashMap(array[0], elements[i]));
        int count2 = atoi(searchInHashMap(array[1], elements[i]));
        if (!count1 && !count2)
            continue;
        if (count1 <= count2)
        {
            ASTWeigth = count1 / count2;
        }
        else
        {
            ASTWeigth = count1 / count2;
        }
    }
    ASTWeigth *= 30;

    double cosineWeigth = cosineSimilarity(arrayWordFreq[0], size1, arrayWordFreq[1], size2) * 70;
    printf("Final similarity between the provided codes is: %.2f\n", cosineWeigth + ASTWeigth);

    for (int i = 0; i < arr_size; i++)
    {
        free(arrayWordFreq[i]);
        freeHashMap(array[i]);
    }
    free(array);
    return 0;
}
