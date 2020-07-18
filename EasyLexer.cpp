#include <stdio.h>
#include <map>
#include <string>
#include <regex>

class Token
{
public:
    int token;
    std::string value;
    bool hasNext;
    Token();
    Token(int _token);
};

Token::Token(){
    hasNext = false;
}

Token::Token(int _token)
{
    hasNext = true;
    token = _token;
}

class EasyLexer
{
private:
    std::map<int, std::regex, std::greater<int>> tokens;
    std::string string_to_analysis;
    int current_char_location;

public:
    void add_new_token(int token, std::string regex);
    void assign_string(std::string read_string);
    Token next_token();
};

void EasyLexer::add_new_token(int token, std::string regex)
{
    tokens.insert(std::pair<int, std::regex>(token, std::regex(regex)));
}

void EasyLexer::assign_string(std::string read_string)
{
    string_to_analysis = read_string;
    current_char_location = 0;
}

Token EasyLexer::next_token()
{
    if (string_to_analysis.empty())
        return Token();

    if (current_char_location > string_to_analysis.size())
        return Token();

    std::map<int, std::regex>::iterator iter;
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
    {
        int offset = 1;
        bool found = false;
        std::string current_analysis = string_to_analysis.substr(current_char_location, offset);

        while (current_char_location + offset <= string_to_analysis.size())
        {
            if (found)
            {
                if (!std::regex_match(current_analysis, iter->second))
                {
                    current_analysis = string_to_analysis.substr(current_char_location, --offset);
                    break;
                }
            }
            else
            {
                if (std::regex_match(current_analysis, iter->second))
                    found = true;
            }
            current_analysis = string_to_analysis.substr(current_char_location, ++offset);
        }

        if (found)
        {
            if ((current_char_location + offset) == string_to_analysis.size())
                current_analysis = string_to_analysis.substr(current_char_location, --offset);

            current_char_location += offset;
            std::string *value;
            value = new std::string;
            *value = current_analysis;
            Token token = Token(iter->first);
            token.value = current_analysis;
            return token;
        }
    }
    return Token();
}

//Example
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
    a.add_new_token(Tokens::token_open_scope, "\\{");
    a.add_new_token(Tokens::token_if, "if");
    a.add_new_token(Tokens::token_left_bracket, "\\(");
    a.add_new_token(Tokens::token_right_bracket, "\\)");
    a.add_new_token(Tokens::token_eof, std::string(1, EOF));
    a.add_new_token(Tokens::token_close_scope, "\\}");
    a.add_new_token(Tokens::token_variable, "[a-z]+");
    a.add_new_token(Tokens::token_greater_than, ">=");
    a.add_new_token(Tokens::token_greater, ">");

    a.assign_string("if(hello>=world){}");
    Token next_token;
    while ((next_token = a.next_token()).hasNext)
    {
        printf("Token: %s, Type: %d\n", next_token.value.c_str(), next_token.token);
    }
}