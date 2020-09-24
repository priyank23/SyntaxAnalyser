//A Basis C/C++ Lexical Analyser
//The program consists of various functions to identify the type of token.
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <tuple>
#include <fstream>

// Assigning an id to these special types
#define KEYWORD_ID 0
#define LITERAL_ID 99
#define IDENTIFIER_ID 2
#define INTEGER_ID 7
#define FLOAT_ID 99
#define CHAR_ID 9
#define INVALID_IDENTIFIER_ID 99

using namespace std;

//transition table declaration for dfa of keywords
int dfa_keyword[25][26];

// A Hash Table for storing the type of each token which is not a keyword
// the first integer is the id to this type. The second string is the string
// to be displayed in the type column of the output file
const map < int, pair< string, vector<char> > > char_token_types = {
  { 3, { "Arithmetic Operator", { '+', '-', '*', '/' } } },
  { 4, { "Relational Operator", { '<', '>', '~' } } },
  { 5, { "Assignment Operator", { '=' } } },
  { 6, { "Special Symbol", { '(', ')', '{', '}', ',', ';', ':', '?' } } }
};

// this array stores the tokens.
// the first parameter (int) is the token_id, second parameter
// (string) is the actual token read from input file. The
// third parameter (int) is a number representing the type
// of the token, and the last two parameters (int) represent
// the line number and character number in the line (position)
vector< tuple<int, string, int, int, int> > tokens;

// this hash table is used to assign unique token_id to every
// new token and reuse the old token_id for tokens which have
// already been assigned an id.
map<string, int> token_id_map;


// this function reads the words from the supplied keywords file
// and creates the keywords array from it.

void populate_keywords(ifstream& keywords_file) {
  vector<int>states(1);
  string line;
  int state,last_state=1,final_state=50;;
  while (getline(keywords_file, line)) {
    state=1;
    for(int i=0;i<line.length()-1;i++)
    {
      if(dfa_keyword[state-1][line[i]-97]==0) 
      {
        dfa_keyword[state-1][line[i]-97]=++last_state;
        state=last_state;
      }
      else state=dfa_keyword[state-1][line[i]-97];
    }
    dfa_keyword[state-1][line[line.length()-1]-97]=25;
  }
  fstream f;
  f.open("./output/DFA_for_Lexical.txt",ios::out);
  f<<"DFA created for keywords (Transition Table (50 X 26)):\n\n";
  for(int i=0;i<25;i++)
  {
    for(int j=0;j<26;j++) f<<dfa_keyword[i][j]<<" ";
    f<<endl;
  }
}

// takes a string and checks if it is a keyword or not
bool is_keyword(const string& str) {
  int state=1;
  for(int i=0;i<str.length();i++){
    if(str[i]<'a' || str[i]>'z'){state=0;break;}
    if(state!=0) state=dfa_keyword[state-1][str[i]-97];
  }
  if(state==25) return true;

  return false;
}

//takes a string and checks if it is a valid id or not
bool is_id(const string& str)
{
  int dfa_id[3][3]=
  {
    {2,0,0},
    {2,2,0},
    {0,0,0}
  };
  int state=1,final_state=2;
  for(char c: str)
  {
    if((c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_') state=dfa_id[state-1][0];
    else if(c>='0' && c<='9') state = dfa_id[state-1][1];
    else state=dfa_id[state-1][2];
    if(state==0) break;
  }
  if(state==final_state) return true;
  return false;
}
// takes a string and checks if it is a literal or not
// it checks for character constants like 'a'
int is_literal(const string& str) {
  int dfa_literal[7][3]=
  {
    {2,7,0},
    {4,0,3},
    {4,0,5},
    {0,0,0},
    {6,0,5},
    {0,0,0},
    {7,6,7}
  };
  int state=1, final_char=4, final_string=6;
  for(char c:str)
  {
    if(c == '\'')
      state=dfa_literal[state-1][0];
    else if (c == '\"') state=dfa_literal[state-1][1];
    else state=dfa_literal[state-1][2];
    if(state==0) break;
  }
  if(state==final_char) return 1;
  if(state==final_string) return 2;
  return 0;
}

//takes a string and checks if it an integer or float
int is_integer(const string& str)
{
  int dfa_int_float[7][5]=
  {
    {1,0,0,0,6},
    {1,1,2,1,6},
    {2,3,2,2,6},
    {4,3,3,5,6},
    {4,4,4,4,6},
    {4,5,5,5,6},
    {6,6,6,6,6}
  };
  int final_int[]={1,4};int final_float=2;
  int state=0;
  for(char c: str)
  {
    if(c>='0' && c<='9') state=dfa_int_float[state][0];
    else if(c=='e'||c=='E') state=dfa_int_float[state][1];
    else if(c=='.') state=dfa_int_float[state][2];
    else if(c=='+'||c=='-') state=dfa_int_float[state][3];
    else state=dfa_int_float[state][4];
  }
  if(state==final_int[0] || state==final_int[1]) return 1;
  if (state==final_float) return 2;
  return 0;
}

// takes a string and checks if it marks the begining of a single line comment
bool is_singleline_comment(const string& str) {
  int dfa_comment[3][2]=
  {
    {2,0},
    {3,0},
    {3,3}
  };
  int state=1,final_state=3;
  for(char c:str)
  {
    if(c == '/')
      state=dfa_comment[state-1][0];
    else state=dfa_comment[state-1][1];   
    if(state==3) break; 
    if(state==0) break;
  }
  if(state==final_state) return true;
  else return false;

}

// takes a string and checks if it marks the begining of a multi line comment
bool is_multiline_comment_start(const string& str) {
  int dfa_comment[3][3]=
  {
    {2,0,0},
    {0,3,0},
    {3,3,3}
  };
  int state=1,final_state=3;
  for(char c:str)
  {
    if(c == '/')
      state=dfa_comment[state-1][0];
    else if(c == '*')
      state=dfa_comment[state-1][1];
    else state=dfa_comment[state-1][2];
    if(state==0) break;
  }
  if(state==final_state) return true;
  else return false;
}

// takes a string and checks if it marks the end of a multi line comment
bool is_multiline_comment_end(const string& str) {
  int dfa_comment[3][3]=
  {
    {2,0,1},
    {0,3,0},
    {3,3,3}
  };
  int state=1,final_state=3;
  for(char c:str)
  {
    if(c == '*')
      state=dfa_comment[state-1][0];
    else if(c == '/')
      state=dfa_comment[state-1][1];
    else state=dfa_comment[state-1][2];
    if(state==0) break;
  }
  if(state==final_state) return true;
  else return false;
}

// this function is the main parser function.
// It reads the input_file (file_to_analyse) line by line
// tokenises it by checking for space or any of the
// operators, and finally inserts them into the tokens array
// with additional collected information like type of token,
// position of token and token id.
void parse_input_file(ifstream& input_file) {
  string line, token;
  int lineNo = 0;
  // boolean to store if multiline comment in progress
  bool multiline_comment = false;
  // read line wise
  while (getline(input_file, line)) {
    // keep track of line number
    ++lineNo;
    // keep track of the character number in the line
    int start = 1;
    token = "";
    // boolean to store if singleline comment in progress
    bool singleline_comment = false;
    // iterate over the characters in the line
    for ( int loop_ind = 0; loop_ind < line.length(); ++loop_ind ) {
      const char token_char = line[loop_ind];
      const char next_token_char =
      loop_ind == line.length()-1 ? '\n' : line[loop_ind + 1];
      const char prev_token_char =
      loop_ind == 0 ? '\n' : line[loop_ind - 1];
      bool char_is_ops = false;
      int char_op_type = -1;
      // check if it is a space or an character operator
      for (const auto& operator_bundle : char_token_types) {
        if (token_char == ' ')
          break;
        if (token_char == '/' &&
            (next_token_char == '/' || next_token_char == '*'
              || prev_token_char == '/' || prev_token_char == '*'))
          break;
        if (token_char == '*' && (next_token_char == '/' || prev_token_char == '/'))
          break;
        if ( find( operator_bundle.second.second.begin(),
                  operator_bundle.second.second.end(),
                  token_char) != operator_bundle.second.second.end() ) {
          char_is_ops = true;
          char_op_type = operator_bundle.first;
          break;
        }
      }
      // if character encountered is indeed a space of a
      // character operator, then we process the token
      // string generated so far
      if (char_is_ops || token_char == ' ') {
        if (token == "" || singleline_comment) {}
        else if (multiline_comment) {
          if (is_multiline_comment_end(token)) {
            multiline_comment = false;
          }
        }
        // we check the type of token and process it
        else if (is_keyword(token)) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          tokens.push_back( make_tuple(
            token_id_map[token], token, KEYWORD_ID, lineNo, start
          ));
        }
        else if (is_literal(token)==1) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          tokens.push_back( make_tuple(
            token_id_map[token], token, CHAR_ID, lineNo, start
          ));
        }
        else if(is_literal(token)==2) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          tokens.push_back( make_tuple(
            token_id_map[token], token, LITERAL_ID, lineNo, start
          ));
        }
         else if (is_integer(token)!=0) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          if(is_integer(token)==1)
          tokens.push_back( make_tuple(
            token_id_map[token], token, INTEGER_ID, lineNo, start
          ));
          else if(is_integer(token)==2)
          tokens.push_back( make_tuple(
            token_id_map[token], token, FLOAT_ID, lineNo, start
          ));
        }
        else if (is_singleline_comment(token)) {
          singleline_comment = true;
        }
        else if (is_multiline_comment_start(token)) {
          multiline_comment = true;
        }
        else {
        if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
        if (is_id(token))
          tokens.push_back( make_tuple(
            token_id_map[token], token, IDENTIFIER_ID, lineNo, start
          ));
        else
          tokens.push_back( make_tuple(
            token_id_map[token], token, INVALID_IDENTIFIER_ID, lineNo, start
          ));
        }
        if (token_char == ' ')
          start += token.length() + 1;
        else
          start += token.length();
        token = "";
        if (char_is_ops) {
          string tmp_token = string({token_char});
          token_id_map[tmp_token]=token_id_map.size();
          tokens.push_back( make_tuple(
            token_id_map[tmp_token], tmp_token, char_op_type, lineNo, start
          ));
          ++start;
        }
      }
      // if not space nor character operator then we append
      // the character to the token string generated so far
      else {
        token.push_back(token_char);
      }
    }
    if (token != "" && !singleline_comment) {
      if (multiline_comment) {
        if (is_multiline_comment_end(token)) {
          multiline_comment = false;
        }
      }
      else if (is_keyword(token)) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          tokens.push_back( make_tuple(
            token_id_map[token], token, KEYWORD_ID, lineNo, start
          ));
        }
        else if (is_literal(token)==1) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          tokens.push_back( make_tuple(
            token_id_map[token], token, CHAR_ID, lineNo, start
          ));
        }
        else if(is_literal(token)==2) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          tokens.push_back( make_tuple(
            token_id_map[token], token, LITERAL_ID, lineNo, start
          ));
        }
         else if (is_integer(token)!=0) {
          if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
          if(is_integer(token)==1)
          tokens.push_back( make_tuple(
            token_id_map.size()-1, token, INTEGER_ID, lineNo, start
          ));
          else if(is_integer(token)==2)
          tokens.push_back( make_tuple(
            token_id_map[token], token, FLOAT_ID, lineNo, start
          ));
        }
      
      else {
        if(token_id_map.find(token) == token_id_map.end()){
            token_id_map[token] = token_id_map.size();
          }
        if (is_id(token) )
          tokens.push_back( make_tuple(
            token_id_map[token], token, IDENTIFIER_ID, lineNo, start
          ));
        else
          tokens.push_back( make_tuple(
            token_id_map[token], token, INVALID_IDENTIFIER_ID, lineNo, start
          ));
      }
    }
  }
}

// this function returns the string that is displayed in the output file
// based on the token type number.
string getTypeString(int typeId) {
  switch (typeId) {
    case 0: return "keyword";
    case 1: return "literal";
    case 2: return "id";
    case 7: return "int";
    case 8: return "float";
    case 9: return "char";
    case 99: return "invalid-id";
    default: return (*(char_token_types.find(typeId))).second.first;
  }
  return "";
}

// this function creates and writes to the output token file by iterating over
// the tokens array generated by the parser (the parse_input_file function)
void write_output_file(ofstream& fout) {
  fout << setw(10) << "id_no"
        << setw(20) << "lexeme"
        << setw(25) << "type"
        << setw(10) << "line_no"
        << setw(10) << "char_no"
        << '\n';
  for (int i = 0; i < 75; i++)
    fout << '-';
  fout << '\n';
  for (const auto& t : tokens) {
    fout << setw(10) << get<0>(t)
          << setw(20) << get<1>(t)
          << setw(25) << getTypeString(get<2>(t))
          << setw(10) << get<3>(t)
          << setw(10) << get<4>(t) << "\n";
  }
}

// the main driver function
bool lex(string inputfile) {

  ifstream ifile;
  ofstream ofile;
  ifile.open("./input/keywords.txt");
  if (!ifile) {
    cout << "Error opening keywords file. Aborting!\n";
    return false;
  }
  populate_keywords(ifile);
  ifile.close();
  ifile.open(inputfile);
  if (!ifile) {
    cout << "Error opening input file to analyse. Aborting!\n";
    return false;
  }
  parse_input_file(ifile);
  ifile.close();
  ofile.open("./output/tokens.txt");
  write_output_file(ofile);
  ofile.close();
  cout << " Lexical Analysis Completed !! \n";
  return true;
}
