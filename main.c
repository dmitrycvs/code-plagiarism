#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <tree_sitter/api.h>

void extract_function_types(TSNode node, const char *code)
{
    if (strcmp(ts_node_type(node), "function_definition") == 0)
    {
        TSNode function_type_node = ts_node_named_child(node, 0);
        uint32_t start_byte = ts_node_start_byte(function_type_node);
        uint32_t end_byte = ts_node_end_byte(function_type_node);

        const char *node_type = ts_node_type(function_type_node);
        char *function_type = (char *)malloc(100 * sizeof(char));
        if (function_type == NULL)
        {
            printf("An error occured while the memory was allocated!");
            exit(1);
        }

        snprintf(function_type, 100, "%.*s", end_byte - start_byte, code + start_byte);

        printf("Function: %s\n", function_type);
        free(function_type);
    }

    uint32_t child_count = ts_node_child_count(node);
    for (uint32_t i = 0; i < child_count; ++i)
    {
        TSNode child = ts_node_child(node, i);
        extract_function_types(child, code);
    }
}

const TSLanguage *tree_sitter_cpp(void);

int main()
{
    TSParser *parser = ts_parser_new();

    ts_parser_set_language(parser, tree_sitter_cpp());

    const char *source_code = "\
            #include <iostream>\n\
            string main(int a, int b) {\n\
                cout << \"Hello, world!\\n\";\n\
                return 0;\n\
            }\n\
        ";
    TSTree *tree = ts_parser_parse_string(
        parser,
        NULL,
        source_code,
        strlen(source_code));

    TSNode root_node = ts_tree_root_node(tree);

    extract_function_types(root_node, source_code);

    ts_tree_delete(tree);
    ts_parser_delete(parser);
    return 0;
}
