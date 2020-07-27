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

    //What line did this token start on
    int line_number;

    //What character on that line did this token start at
    int start_character;

    //Default constructor
    Token();

    //Constructor in which the token is set. _token - the token type of this token, line - which line this token starts on, char_start - the character count which this token start at
    Token(int _token, int line, int char_start);

    //Constructor in which the token is set. _token - the token type of this token, line - which line this token starts on, char_start - the character count which this token start at
    Token(int _token, int line, int char_start, std::string value);

    //Constructor in which the token is set. line - which line this token starts on, char_start - the character count which this token start at
    Token(int line, int char_start, std::string value);
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

    //Extract the next token from the current string. return - the token which has just been extracted, check has next to see if it is a valid token
    Token next_token();

    //Stores the lastest set of characters that caused issues
    std::string error_characters;

    //Stores a list of strings that were not able to be parsed
    std::vector<Token> errors;

    //Stores the current char position
    int error_char_position;

    //Stores the current line number
    int error_line_number;

    //Stores the current char position
    int char_position;

    //Stores the current line number
    int line_number;

public:
    //Stores if the lexer was successful when parsing
    bool successful;

    //What token defines the last token - this will be added at the end
    int end_of_field_token = 0;

    //Add a new valid token to the set of valid tokens. token- the token type, regex - the regex that defines the token
    void add_new_token(int token, std::string regex);

    //Set up a new string to be processed. read_string - the string to be processed, return - the list of tokens generated from the string
    std::vector<Token> parse(std::string read_string);
};
#endif