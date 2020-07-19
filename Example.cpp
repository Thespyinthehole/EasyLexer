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
    token_variable = -9
};

int main()
{
    EasyLexer a;
    //Add the valid tokens to the lexer with the regex that describes them
    a.add_new_token(Tokens::token_open_scope, "\\{");
    a.add_new_token(Tokens::token_if, "if");
    a.add_new_token(Tokens::token_left_bracket, "\\(");
    a.add_new_token(Tokens::token_right_bracket, "\\)");
    a.add_new_token(Tokens::token_eof, std::string(1, EOF));
    a.add_new_token(Tokens::token_close_scope, "\\}");
    a.add_new_token(Tokens::token_variable, "[a-z]+");
    a.add_new_token(Tokens::token_greater_than, ">=");
    a.add_new_token(Tokens::token_greater, ">");

    //Write the string we want to extract tokens from
    std::list<Token> tokens = a.parse("if(hello>=world){}");

    std::list<Token>::iterator iter;
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
        printf("\nToken: %s, Type: %d", iter->value.c_str(), iter->token);
}