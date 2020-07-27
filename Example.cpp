#include "EasyLexer.h"

//Define the valid token types - higher number = higher priority
enum Tokens
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
    lexer.add_new_token(Tokens::token_open_scope, "\\{");
    lexer.add_new_token(Tokens::token_if, "if");
    lexer.add_new_token(Tokens::token_left_bracket, "\\(");
    lexer.add_new_token(Tokens::token_right_bracket, "\\)");
    lexer.add_new_token(Tokens::token_eof, std::string(1, EOF));
    lexer.add_new_token(Tokens::token_close_scope, "\\}");
    lexer.add_new_token(Tokens::token_variable, "[a-z]+");
    lexer.add_new_token(Tokens::token_greater_than, ">=");
    lexer.add_new_token(Tokens::token_greater, ">");
    lexer.add_new_token(Tokens::token_new_line, "\n");

    //Write the string we want to extract tokens from
    std::vector<Token> tokens = lexer.parse("--\nif--(hello>=world){}");

    std::vector<Token>::iterator iter;
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
        if (lexer.successful)
            printf("\nToken: %s, Type: %d", iter->value.c_str(), iter->token);
        else

            printf("Error on line %d at character %d, unknown characters: %s\n", iter->line_number, iter->start_character, iter->value.c_str());
}