#include "./LexicalAnalyser/lexical_analyser.h"
#include "./Parser/parser.h"
#include "inter.h"

using namespace std;

int main() {
    if(!lex()) return 0;
    string s = printint(tokens);
    if(s=="") return 0;
    if(parse(s)) cout<<"Accepted\n";
    else cout<<"Error during parsing!!\n";
    return 0;
}