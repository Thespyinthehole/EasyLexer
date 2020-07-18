#ifndef EasyLexer_H
#define EasyLexer_H

#include <map>
#include <string>
#include <regex>

//Stores the token data
class Token
{
public:
    //What type of token this is
    int token;

    //The full string value this token is
    std::string value;

    //Is there a token after this one
    bool hasNext;

    //Default constructor
    Token();

    //Constructor in which the token is set. _token - the token type of this token
    Token(int _token);
};

//Stores the valid tokens and extracts tokens from a given string
class EasyLexer
{
private:
    //The set of valid tokens
    std::map<int, std::regex, std::greater<int>> tokens;
    //The current string that is being processed
    std::string string_to_analysis;
    //How far along the string have we processed up to
    int current_char_location;

public:
    //Add a new valid token to the set of valid tokens. token- the token type, regex - the regex that defines the token
    void add_new_token(int token, std::string regex);
    //Set up a new string to be processed. read_string - the string to be processed
    void assign_string(std::string read_string);
    //Extract the next token from the current string. return - the token which has just been extracted, check has next to see if it is a valid token
    Token next_token();
};
#endif