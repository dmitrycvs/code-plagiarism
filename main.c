#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>
#include <sqlite3.h>
#include "hashmap.h"

const TSLanguage *tree_sitter_cpp(void);

void extractDataFromDatabase()
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
    else
    {
        fprintf(stderr, "Opened database successfully\n");
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
        int num_columns = sqlite3_column_count(stmt);
        for (int j = 0; j < num_columns; j++)
        {
            const unsigned char *column_text = sqlite3_column_text(stmt, j);
            if (column_text)
            {
                char *data = (char *)malloc((strlen(column_text) + 1) * sizeof(char));
                strcpy(data, (const char *)column_text);
                printf("Data from database: %s\n", data);
                free(data);
            }
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void extract_function_information(TSNode node, char *code)
{
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
            printf("Function: %s\n", function_type);
        }
        else if (strcmp(function_type, "void") == 0)
        {
            printf("Function: %s\n", function_type);
        }
        // For numbers
        else
        {
            printf("Function: %s\n", function_type);
        }

        free(function_type);
    }

    uint32_t child_count = ts_node_child_count(node);
    for (uint32_t i = 0; i < child_count; ++i)
    {
        TSNode child = ts_node_child(node, i);
        extract_function_information(child, code);
    }
}

int main()
{
    TSParser *parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_cpp());

    char *source_code;

    TSTree *tree = ts_parser_parse_string(
        parser,
        NULL,
        source_code,
        strlen(source_code));

    TSNode root_node = ts_tree_root_node(tree);

    extract_function_information(root_node, source_code);

    ts_tree_delete(tree);
    ts_parser_delete(parser);

    extractDataFromDatabase();

    HashMap *myMap = createHashMap();
    insert(myMap, "name", "dimon");
    insert(myMap, "surname", "dimon123");
    // printf("%s\n%s\n", search(myMap, "name"), search(myMap, "surname"));
    freeHashMap(myMap);

    return 0;
}
