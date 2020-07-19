# EasyLexer

## Introduction
This library is a lexer in C++ inspired by JavaCC. 
I wrote this as I have started looking into using LLVM, but their tutorial shows a different lexer for each language you want to create.
Therefore I have made a lexer for most cases.

## How To Use
Place both `EasyLexer.cpp` and `EasyLexer.h` in your project directory. In your project file add `#include "EasyLexer.h"`.

You are now ready to set up your lexer, which can be create by doing `EasyLexer [variable name]`.

### Defining valid token
A token requires 2 parts in order to be created, an integer(which is both it's ID and priority) and a regex string that defines the token.

The most readable way of defining the IDs for the tokens is to use an enum. For example

```
enum Tokens {
  token_space = 0,
  token_new_scope = -1
};
```

The priority mentioned above results in tokens with a higher value will be scanned for first. In the example above, `token_space` will be looked for before `token_new_scope`. In this example priority does not matter. However, when you define the regex strings which are able to overlap with each other then priority needs to be taken into account. An example of this is the regex for `>` and the regex for `>=`, in which the `>=` needs to have a higher priority than `>` as the `>` appears in `>=`.

Now we have got onto regex, we now are able to use our enum to define a full token in the lexer. In this example, I will be using the enum above and my lexer object will be called `lexer`.

```
lexer.add_token(Tokens::token_space," ");
lexer.add_token(Tokens::token_new_scope,"\\{");
```

By using the `add_token` method, we have signed up our 2 valid tokens to the lexer so now it knows what it is looking for. The `add_token` method requires 2 parameters, the first being an int and the second being the regex string. In the second example listed, `\\` has to be placed before `{` as the `{` is a special character in regex so we have to escape that character, if your project does not work, it is likely you have forgotten to escape the special characters. As this parameter is regex, you are able to enter `[a-z]+` to find all lower case words. One final note: if you do this instead `[a-z]*`, this token will return the full string back as this will fit any string.


### Running the lexer

Now that the tokens have been defined, we now need to give the lexer a string to extract tokens from. 

`lexer.assign_string(" { {");`

Finally, now that everything has been set up, it is time to extract our tokens. This is done by calling `lexer.next_token()`. No matter if a token has been successfully extracted or not, a token object will always been returned. To test if the token is valid, the `has_next` property will be true if successful. Using this you can easily loop over the tokens as seen in the example below.

```
while ((next_token = a.next_token()).hasNext)   
  printf("\nToken: %s, Type: %d", next_token.value.c_str(), next_token.token);
    
```

### Compiling the code
When compiling your code, do what you would normally do when compiling except you will need to add the `EasyLexer.cpp` to the compilier, either through an IDE or by adding the file to the command in the terminal.