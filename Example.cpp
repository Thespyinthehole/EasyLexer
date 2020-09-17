#include "EasyLexer.h"

//Define the valid token types - higher number = higher priority
enum Tokens : int
{
    token_eof = -1,
    token_open_scope = -2,
    token_close_scope = -3,
    token_if = -4,
    token_greater_than = -5,
    token_greater = -6,
    token_left_bracket = -7,
    token_right_bracket = -8,
    token_variable = -9,
    token_new_line = -10
};

int main()
{
    EasyLexer lexer;

    //Add the valid tokens to the lexer with the regex that describes them
    lexer.add_new_token(token_open_scope, "\\{");
    lexer.add_new_token(token_if, "if");
    lexer.add_new_token(token_left_bracket, "\\(");
    lexer.add_new_token(token_right_bracket, "\\)");
    lexer.add_new_token(token_close_scope, "\\}");
    lexer.add_new_token(token_variable, "[a-z]+");
    lexer.add_new_token(token_greater_than, ">=");
    lexer.add_new_token(token_greater, ">");
    lexer.add_new_token(token_new_line, "\n");

    //Write the string we want to extract tokens from
    try
    {
        std::vector<Token> tokens = lexer.parse("if(hello>=world){}");
        for (int i = 0; i < tokens.size(); i++)
            printf("\nToken: %s, Type: %d", tokens[i].value.c_str(), tokens[i].token);
    }
    catch (LexicalException e)
    {
        printf("%s", e.what());
    }
}