#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <tree_sitter/api.h>
#include "hashmap.h"

const TSLanguage *tree_sitter_cpp(void);

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open the file.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL)
    {
        fclose(file);
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size)
    {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Error reading file.\n");
        return NULL;
    }

    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}

void extract_function_information(TSNode node, char *code)
{
    if (strcmp(ts_node_type(node), "function_definition") == 0)
    {
        TSNode function_type_node = ts_node_named_child(node, 0);
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

    const char *filename = "input.txt";
    char *source_code = read_file(filename);

    if (source_code == NULL)
    {
        return 1;
    }

    TSTree *tree = ts_parser_parse_string(
        parser,
        NULL,
        source_code,
        strlen(source_code));

    TSNode root_node = ts_tree_root_node(tree);

    extract_function_information(root_node, source_code);

    ts_tree_delete(tree);
    ts_parser_delete(parser);

    free(source_code);

    HashMap *myMap = createHashMap();
    insert(myMap, "name", "dimon");
    insert(myMap, "surname", "dimon123");
    // printf("%s\n%s\n", search(myMap, "name"), search(myMap, "surname"));
    freeHashMap(myMap);

    return 0;
}
