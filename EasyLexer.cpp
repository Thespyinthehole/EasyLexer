#include "EasyLexer.h"

Token::Token()
{
    //This token is the end of the processing
    hasNext = false;
}

Token::Token(Tokens _token, int line, int char_start)
{
    //This token still has stuff to process
    hasNext = true;

    //Set the token type for this token
    token = _token;

    //Set the line number
    line_number = line;

    //Set the char position
    start_character = char_start;
}

Token::Token(Tokens _token, int line, int char_start, std::string value)
{
    //This token still has stuff to process
    hasNext = true;

    //Set the token type for this token
    token = _token;

    //Set the value
    this->value = value;

    //Set the line number
    line_number = line;

    //Set the char position
    start_character = char_start;
}

Token::Token(int line, int char_start, std::string value)
{
    //Set the value
    this->value = value;

    //Set the line number
    line_number = line;

    //Set the char position
    start_character = char_start;
}

void EasyLexer::add_new_token(Tokens token, std::string regex)
{
    //Add a new valid token to the map, turning the regex string into a regex object
    tokens.insert(std::pair<Tokens, std::regex>(token, std::regex("^(" + regex + ")")));
}

LexicalException::LexicalException(std::vector<Token> errors)
{
    lexical_errors = errors;
}

const char *LexicalException::what()
{
    error_message = "";
    for (int i = 0; i < lexical_errors.size(); i++)
    {
        error_message += "Error on line ";
        error_message += std::to_string(lexical_errors[i].line_number);
        error_message += " at character ";
        error_message += std::to_string(lexical_errors[i].start_character);
        error_message += " unknown characters: ";
        error_message += lexical_errors[i].value.c_str();
        error_message += "\n";
    }
    return error_message.c_str();
}

std::vector<Token> EasyLexer::parse(std::string read_string)
{
    //Reset the line and char values
    line_number = 1;
    char_position = 1;
    error_line_number = 1;
    error_char_position = 1;

    //Set the new string
    string_to_analysis = read_string;

    //Reset the offset
    current_char_location = 0;

    //Clear the errors
    errors.clear();

    std::vector<Token> tokens;

    //Read token until end of file
    Token token;

    //Get ignored tokens
    std::vector<Tokens>::iterator begin = ignored_tokens.begin();
    std::vector<Tokens>::iterator end = ignored_tokens.end();

    while ((token = next_token()).hasNext)
        if (std::find(begin, end, token.token) == end)
            tokens.push_back(token);

    //Return if there are errors
    if (errors.size() > 0)
        throw LexicalException(errors);

    //Return successful tokens
    tokens.push_back(Token(end_of_field_token, line_number, char_position));
    return tokens;
}

Token EasyLexer::next_token()
{
    //If there is a blank string do not process
    if (string_to_analysis.empty())
        return Token();

    //If we have reached the end do not process
    if (current_char_location >= string_to_analysis.size())
        return Token();

    //Get the iterator for the map of valid tokens
    std::map<Tokens, std::regex>::iterator iter;

    std::string current_analysis = string_to_analysis.substr(current_char_location);
    std::smatch regex_match;

    //Loop over each valid token
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
    {
        //Check if we match
        if (std::regex_search(current_analysis, regex_match, iter->second))
            break;
    }

    if (regex_match.size() != 0)
    {
        if (error_characters.size() > 0)
            errors.push_back(Token(error_line_number, error_char_position, error_characters));

        //Reset the error string
        error_characters = "";

        //Get the matched string and return a token linking to it
        std::string matched_string = regex_match[0].str();
        current_char_location += matched_string.size();
        Token token(iter->first, line_number, char_position, matched_string);

        //Move the line and char position along
        for (char chr : matched_string) {
            if (chr == '\n')
            {
                line_number++;
                char_position = 1;
            }
            else
                char_position++;
        }
        error_char_position = char_position;
        error_line_number = line_number;
        return token;
    }


    if (string_to_analysis.at(current_char_location) == '\n')
    {
        line_number++;
        char_position = 1;
    }
    else
        char_position++;

    //Didnt find a match so add an invalid character and try from next character
    error_characters += string_to_analysis.at(current_char_location++);

    if (current_char_location == string_to_analysis.size())
        errors.push_back(Token(error_line_number, error_char_position, error_characters));
    return next_token();
}

void EasyLexer::add_ignored_token(Tokens token)
{
    ignored_tokens.push_back(token);
}
