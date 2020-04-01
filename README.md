# CompilerDesign

* Building a basic compiler of C-based programs supporting following features:
  * Data types - ```int```, ```char```
  * Assignments to the variables can be done at the time of declaration.
  * IO  ```get x;```(taking x as input from stdin)\
        ```put x;```(printing x at stdout)
  * Conditional statement syntax:
     ```id = expression ? expression:expression;```
  * Relational Operators supported: {>, <, =}
  * Arithmetic Operators supported: {+, -, *}
  * Only ```main()``` function is supported. The function may definition may contain arguments and return statement.
  * White-spaces and new lines within statements are ignored.
  * Statements end with **;**
* Steps involved in the construction of the Compiler - 
  * [x] Construction of CFG for the language.
  * [ ] Building Lexical Analyser to scan the stream of characters from a program written in above language and generate a stream of tokens to pass to the parser.
  * [ ] Building a Bottom-up parser for this language using SLR(0) parsing technique.
