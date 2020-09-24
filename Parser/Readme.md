# Parser

A basic C/C++ Parser created using SLR(1) technique.

## Input

The parser takes following files as input:
* `LetterCFG.txt`: The grammar of the language
* `tokens.txt`: The stream of the tokens created by the Lexical Analyser

## Output

* `StatesFile.txt`: The canonical states created.
* `Parsing Table.csv`: The item sets created.
* `ParsingSteps.txt`: The actual steps taken by the parser while parsing an input.