# CompilerDesign

# QUESTION
* Building a basic compiler of C-based programs supporting following features:
  * Data types - ```int```, ```char```
  * Assignments to the variables can be done at the time of declaration.
  * IO  ```get x;```(taking x as input from stdin)\
        ```put x;```(printing x at stdout)
  * Conditional statement syntax:
     ```id = expression ? expression:expression;```
  * Relational Operators supported: {>, <, ~}
  * Arithmetic Operators supported: {+, -, *}
  * Only ```main()``` function is supported. The function may definition may contain arguments and return statement.
  * White-spaces and new lines within statements are ignored.
  * Statements end with **;**
* Steps involved in the construction of the Compiler - 
  * [x] Construction of CFG for the language.
  * [x] Building Lexical Analyser to scan the stream of characters from a program written in above language and generate a stream of tokens to pass to the parser.
  * [x] Building a Bottom-up parser for this language using SLR(1) parsing technique.
  
  # CFG
  * Map from symbols of the language to letters of CFG -> mod.txt
  * Productions -> LetterCFG.txt
  * Human read format for CFG -> augCFG.txt
  
  # INPUT FORMAT
  * Input File -> input.cpp
  * File for keywords -> keywords.txt
  * File for the CFG -> LetterCFG.txt
  
  # OUTPUT FORMAT
  * Output from Lexical Analyser -> output.txt
  * DFA states -> DFA_for_Lexical.txt
  * Item set -> StatesFile.txt
  * Goto (I,a) for all item set -> GoToTransitions.txt
  * SLR parsing table -> Parsing Table.csv
  * Result of Parsing -> Accepted/Error
  
  # MEMBERS
  * Abhay Agarwala -> 001710501040
  * Priyank Lohariwal -> 001710501055
  * Tamaghna Sarkar -> 001710501057
  * Bikram Boote -> 001710501059
