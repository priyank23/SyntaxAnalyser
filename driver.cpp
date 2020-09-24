#include "./LexicalAnalyser/lexical_analyser.h"
#include "./Parser/parser.h"
#include "inter.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc!=2) {
        cerr<<"Error!\t Usage: ./compile filename\n";
        return 0;
    }
    cout<<"Compiler Design\n";
    if(!lex(argv[1])) return 0;
    string s = printint(tokens);
    if(s=="") return 0;
    if(parse(s)) cout<<"Result:\n The Program is Syntactically Correct!!!\n\n";
    else cout<<"Result:\n Error during parsing!!\n\n";
    return 0;
}