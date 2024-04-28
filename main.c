#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>
#include <sqlite3.h>
#include "linkedlist.h"

const TSLanguage *tree_sitter_cpp(void);

void incrementCount(HashMap *Map, char *countName)
{
    char *count = searchInHashMap(Map, countName);
    if (count)
    {
        int countNumber = atoi(count) + 1;
        char *str = (char *)malloc((strlen(count) + 2) * sizeof(char));
        sprintf(str, "%d", countNumber);
        str[strlen(str)] = '\0';

        insertInHashMap(Map, countName, str);
        free(str);
    }
    else
    {
        insertInHashMap(Map, countName, "1");
    }
}

void extractFunctionInformation(TSNode node, char *code, Node *head)
{
    HashMap *Map = head->data;
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
    // Add also for while/do while and else/else if
    else if (strcmp(ts_node_type(node), "for_statement") == 0)
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
        extractFunctionInformation(child, code, head);
    }
}

void extractDataFromDatabase(Node *head)
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
            insertInList(&head, Map);

            extractFunctionInformation(root_node, data, head);

            ts_tree_delete(tree);
            ts_parser_delete(parser);
            free(data);
            freeHashMap(Map);
            break;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int main()
{
    Node *head = NULL;
    extractDataFromDatabase(head);
    printf("%s", searchInHashMap(head->data, "return_function_number"));

    deleteList(&head);

    return 0;
}
