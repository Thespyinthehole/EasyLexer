# EasyLexer

## Introduction
This library is a C++ lexer inspired by JavaCC. 
I wrote this as I have started looking into using LLVM, but their tutorial shows a different lexer for each language you want to create.
Therefore I have made a lexer for most cases.

## How To Use
This will use [Example.cpp](https://github.com/Thespyinthehole/EasyLexer/blob/master/Example.cpp) to show how to use this library. 

Place both `EasyLexer.cpp` and `EasyLexer.h` in your project directory. In your project file add `#include "EasyLexer.h"`.

You are now ready to set up your lexer, which can be create by doing `EasyLexer [variable name]`.

### Defining valid token
A token requires 2 parts in order to be created, an integer(which is both its ID and priority) and a regex string that defines the token.

To define a token, you will need to create an enum called `Tokens` and populate it with your tokens. For example:

```c
enum Tokens {
  token_space = 0,
  token_new_scope = -1
};
```

The lexer will look for the longest regex result first and then use the priority listed.

The priority mentioned above results in tokens with a higher value will be scanned for first. In the example above, `token_space` will be looked for before `token_new_scope`. In this example priority does not matter. However, when you define the regex strings which are able to overlap with each other then priority needs to be taken into account. An example of this is the regex for `>` and the regex for `>=`, in which the `>=` needs to have a higher priority than `>` as the `>` appears in `>=`.

Now we have got onto regex, we now are able to use our enum to define a full token in the lexer. In this example, I will be using the enum above and my lexer object will be called `lexer`.

```c
lexer.add_token(token_space," ");
lexer.add_token(token_new_scope,"\\{");
```

By using the `add_token` method, we have registered our 2 valid tokens to the lexer so now it knows what it is looking for. The `add_token` method requires 2 parameters, the first being an int and the second being the regex string. In the second example listed, `\\` has to be placed before `{` as the `{` is a special character in regex so we have to escape that character, if your project does not work, it is likely you have forgotten to escape the special characters. As this parameter is regex, you are able to enter `[a-z]+` to find all lower case words. One final note: if you do this instead `[a-z]*`, this token will return the full string back as this will fit any string.

### Ignored Tokens

Ignored tokens are tokens that when extracted will not be added to the list of valid tokens. These can be added by included a boolean parameter when you add the token

```c
lexer.add_token(token_space," ",true);
```

### Running the lexer

Now that the tokens have been defined, we now need to give the lexer a string to extract tokens from. 

`std::vector<Token> tokens = lexer.parse(" { {");`

This will extract the tokens from the string given and return a list of tokens. This can then be iterated over similar to the example below.

```c
for (int i = 0; i < tokens.size(); i++)
   printf("\nToken: %s, Type: %d", tokens[i].value.c_str(), tokens[i].token);    
```

### Error detection

When running `lexer.parse`, a `LexicalException` can be thrown. If this is thrown that means that there is at least 1 character which did not match a valid token. After putting a try catch block around the `lexer.parse`, you should be able to detect when the errors occur and print them too the user. You can either use `.what()`, which will give a full `char*` explaining the errors, or you can access the tokens in the `errors` property of the exception. These tokens will contain the line number and character number that the token started at. 

### Compiling the code
When compiling your code, do what you would normally do when compiling except you will need to add the `EasyLexer.cpp` to the compiler, either through an IDE or by adding the file to the command in the terminal.

### Limitations
* If two tokens share the same start, it will choose the higher priority. For example, if we have a token which is "int" and we have another which has "[a-z]+", and we want to extract "integer", we will recieve 2 tokens which will be first "int" and second "[a-z]+" storing "eger".

