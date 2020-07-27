#include "EasyLexer.h"

Token::Token()
{
    //This token is the end of the processing
    hasNext = false;
}

Token::Token(int _token, int line, int char_start)
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

Token::Token(int _token, int line, int char_start, std::string value)
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

void EasyLexer::add_new_token(int token, std::string regex)
{
    //Add a new valid token to the map, turning the regex string into a regex object
    tokens.insert(std::pair<int, std::regex>(token, std::regex(regex)));
}

std::list<Token> EasyLexer::parse(std::string read_string)
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

    std::list<Token> tokens;

    //Read token until end of file
    Token token;
    while ((token = next_token()).hasNext)
        tokens.push_back(token);

    //Return if there are errors
    if (errors.size() > 0)
    {
        successful = false;
        return errors;
    }

    //Return successful tokens
    successful = true;
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
    std::map<int, std::regex>::iterator iter;
    //Loop over each valid token
    for (iter = tokens.begin(); iter != tokens.end(); iter++)
    {
        //How many characters should we read
        int offset = 1;
        //Has this token matched any characters yet
        bool found = false;
        //Store current line and
        int current_line = line_number;
        int current_char = char_position;

        //The current search string
        std::string current_analysis = string_to_analysis.substr(current_char_location, offset);

        //Loop over the rest of the characters
        while (current_char_location + offset < string_to_analysis.size())
        {
            //If we have a match already, keep searching until it doesnt match
            if (found)
            {
                if (!std::regex_match(current_analysis, iter->second))
                {
                    current_analysis = string_to_analysis.substr(current_char_location, --offset);
                    break;
                }
            }
            //If we just found a match note it
            else if (std::regex_match(current_analysis, iter->second))
                found = true;

            //If we have reached a \n move line number on 1 and reset char, otherwise increment current char
            if (string_to_analysis.at(current_char_location + offset - 1) == '\n')
            {
                current_line++;
                current_char = 1;
            }
            else
                current_char++;
            //Update the current search string
            current_analysis = string_to_analysis.substr(current_char_location, ++offset);
        }

        //If a match has been found return a token object with the correct type and value.
        if (found)
        {
            //If you reached the second to last character it kinda messes up, but this solves it
            if ((current_char_location + offset) == string_to_analysis.size())
                current_analysis = string_to_analysis.substr(current_char_location, --offset);

            //Move along the number of characters we used
            current_char_location += offset;

            //If an error was found add it to the list
            if (error_characters.size() > 0)
                errors.push_back(Token(error_line_number, error_char_position, error_characters));

            //Reset the error string
            error_characters = "";

            //Add the successful token
            Token token = Token(iter->first, line_number, char_position, current_analysis);

            //Move the line and char position along
            line_number = current_line;
            char_position = current_char;
            error_char_position = char_position;
            error_line_number = line_number;
            return token;
        }
    }

    //If we have reached a \n move line number on 1 and reset char, otherwise increment current char
    if (string_to_analysis.at(current_char_location) == '\n')
    {
        line_number++;
        char_position = 0;
    }
    else
        char_position++;

    //Add the invalid character to the error string
    error_characters += string_to_analysis.at(current_char_location++);

    //Look for other succesful tokens
    return next_token();
}