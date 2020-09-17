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

void EasyLexer::add_new_token(Tokens token, std::string regex)
{
    add_new_token(token, regex, false);
}

void EasyLexer::add_new_token(Tokens token, std::string regex, bool ignore)
{
    tokens.push_back({token, std::regex(regex), ignore});
}

const bool operator<(const LexerToken a, const LexerToken b)
{
    return a.type < b.type;
}

const bool operator>(const LexerToken a, const LexerToken b)
{
    return a.type > b.type;
}

std::vector<Token> EasyLexer::parse(std::string read_string)
{
    //Clear the errors
    errors.clear();

    //Set up the needed references
    std::vector<Token> tokens;
    std::pair<Token, bool> extracted;
    LexerData data = {1, 1, 0};
    std::map<LexerToken, LexerExtraction, std::greater<LexerToken>> matches;

    for (LexerToken token : this->tokens)
    {
        std::smatch match;
        LexerExtraction extracted = {0, "", "", ""};
        if (std::regex_search(read_string, match, token.rgx))
            extracted = {0, match[0].str(), match.prefix(), match.suffix()};
        matches.insert({token, extracted});
    }
    //Extract the tokens
    while ((extracted = next_token(data, matches)).first.hasNext)
        if (!extracted.second)
            tokens.push_back(extracted.first);

    if(read_string.substr(data.full_char) != "")
        errors.push_back(Token(data.line_num, data.char_num, read_string.substr(data.full_char)));

    //Return if there are errors
    if (errors.size() > 0)
        throw LexicalException(errors);

    //Return successful tokens
    tokens.push_back(Token(end_of_field_token, data.line_num, data.char_num));
    return tokens;
}

//Updates the data line and char num as well as total chars read
void EasyLexer::update_char_pointer(LexerData &data, std::string str)
{
    for (char chr : str)
    {
        if (chr == '\n')
        {
            data.line_num++;
            data.char_num = 1;
        }
        else
            data.char_num++;
    }
    data.full_char += str.length();
}

std::pair<Token, bool> EasyLexer::next_token(LexerData &data, std::map<LexerToken, LexerExtraction, std::greater<LexerToken>> &matches)
{
    int first_char = INT_MAX;
    std::pair<LexerToken, LexerExtraction> match = {{}, {0, "", "", ""}};

    for (std::pair<LexerToken, LexerExtraction> current_match : matches)
    {
        if (current_match.second.match.length() == 0)
            continue;
        int pos = current_match.second.offset + current_match.second.prefix.length();
        if (pos < first_char)
        {
            first_char = pos;
            match = current_match;
        }
        else if (pos == first_char && current_match.second.match.size() > match.second.match.size())
            match = current_match;
    }

    if (match.second.match.size() == 0)
        return {Token(), false};

    int error_length = match.second.prefix.length();
    int offset_pos = data.full_char - match.second.offset;

    if (error_length > offset_pos)
    {
        std::string error = match.second.prefix.substr(offset_pos);
        errors.push_back(Token(data.line_num, data.char_num, error));
        update_char_pointer(data, error);
    }

    Token output(match.first.type, data.line_num, data.char_num, match.second.match);
    update_char_pointer(data, match.second.match);

    std::string next = match.second.suffix;
    int end = match.second.offset + match.second.prefix.length() + match.second.match.length();
    for (std::pair<LexerToken, LexerExtraction> current_match : matches)
    {
        if (current_match.second.match == "")
            continue;
        int current_pos = current_match.second.offset + current_match.second.prefix.length();
        if (current_pos >= end)
            continue;
        std::smatch new_match;
        LexerExtraction extracted = {end, "", "", ""};
        if (std::regex_search(next, new_match, current_match.first.rgx))
            extracted = {end, new_match[0], new_match.prefix(), new_match.suffix()};
        matches[current_match.first] = extracted;
    }
    return {output, match.first.ignore};
}