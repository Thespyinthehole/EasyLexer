#ifndef EasyLexer_H
#define EasyLexer_H

#include <map>
#include <string>
#include <regex>
#include <exception>

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

enum Tokens : int;

//Stores the token data
class Token
{
public:
    //What type of token this is
    Tokens token;

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
    Token(Tokens _token, int line, int char_start);

    //Constructor in which the token is set. _token - the token type of this token, line - which line this token starts on, char_start - the character count which this token start at
    Token(Tokens _token, int line, int char_start, std::string value);

    //Constructor in which the token is set. line - which line this token starts on, char_start - the character count which this token start at
    Token(int line, int char_start, std::string value);
};

class LexicalException : public std::exception
{
private:
    std::string error_message;

public:
    //Gives the error in string form
    const char *what();

    //Contains all the errors that the lexer has found
    std::vector<Token> lexical_errors;

    //Make this lexical exception with the errors the lexer has found
    LexicalException(std::vector<Token> errors);
};

struct LexerToken
{
    Tokens type;
    std::regex rgx;
    bool ignore;
};

struct LexerExtraction 
{
    int offset;
    std::string match;
    std::string prefix;
    std::string suffix;
};

struct LexerData
{
    //  std::string to_lex;
    int line_num;
    int char_num;
    int full_char;
};

const bool operator<(const LexerToken, const LexerToken);
const bool operator>(const LexerToken, const LexerToken);

//Stores the valid tokens and extracts tokens from a given string
class EasyLexer
{
private:
    //The set of valid tokens
    std::vector<LexerToken> tokens;

    //Extract the next token from the current string
    std::pair<Token, bool> next_token(LexerData &, std::map<LexerToken, LexerExtraction, std::greater<LexerToken>> &);

    void update_char_pointer(LexerData &, std::string);

    //Stores a list of strings that were not able to be parsed
    std::vector<Token> errors;

    //Stores a combination of all the regex
    // std::string combined_regex;

public:
    //What token defines the last token - this will be added at the end
    Tokens end_of_field_token;

    //Add a new valid token to the set of valid tokens. token- the token type, regex - the regex that defines the token
    void add_new_token(Tokens token, std::string regex);

    //Add a new valid token to the set of valid tokens. token- the token type, regex - the regex that defines the token, ignore - should the token be ignored by the lexer
    void add_new_token(Tokens token, std::string regex, bool ignore);

    //Set up a new string to be processed. read_string - the string to be processed, return - the list of tokens generated from the string
    std::vector<Token> parse(std::string read_string);
};
#endif