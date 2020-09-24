#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <stack>
using namespace std;

//Read the productions list from the file
vector<string> prod_ar()
{
	ifstream fp("./input/LetterCFG.txt");
	vector<string> prod;
	string line;

	while (!fp.eof())
	{
		getline(fp, line);
		if (line.size() == 1)
			continue;
		else
			prod.push_back(line);
	}

	return prod;

} //one extra line is getting inserted at the end so use test condition as <(less than)vector.size()-1

//Get a mapping of production to its index
map<int, string> prod_index()
{
	vector<string> l;
	l = prod_ar();
	map<int, string> index;

	for (int i = 0; i < l.size() - 1; i++)
	{
		index.insert({i, l[i]});
	}

	return index;
}


//Structure for each Item Set/State
struct state
{
	vector<string> prod;
};

//Structure for storing GOTO Transitions
struct GotoTrans
{
	int originState, finalState;
	char input;
};
//List of all goto transitions
vector<GotoTrans> gotoList;

//The complete item set / state collection
struct canonical
{
	vector<struct state> states;
} canonSet;


//Add . dots to the starting of RHS of each of productions
void add_dots(struct state *I)
{
	vector<string> list;
	list = I->prod;
	for (int i = 0; i < list.size(); i++)
	{
		int s = list[i].find('>');
		// if(list[i].find('x')>=0)continue;
		list[i].insert(s + 1, ".");
	}
	I->prod = list;
}

//Create a string after moving the dot to one position right in original
string move_dot(string b)
{
	
	int s = b.find('.');
	if (s == b.length() - 1)
		return b;
	string t(1, b[s + 1]);
	b.replace(s, 1, t);
	b.replace(s + 1, 1, ".");
	
	return b;
}

//Check if a symbol is terminal or not
bool is_non_terminal(char a)
{
	if (a >= 'A' && a <= 'Z')
		return true;
	else
		return false;
}

//whether a production is in a state or not
bool in_state(struct state *I, string a)
{
	for (int i = 0; i < I->prod.size() - 1; i++)
	{
		if (I->prod[i] == a)
			return true;
	}
	return false;
}

//Get the character just after dot when creating Item sets 
char char_after_dot(string l)
{
	char b;
	int s = l.find('.');
	b = l[s + 1];
	return b;
}


void closure(struct state *I0, struct state *G)
{
	for (int i = 0; i < I0->prod.size() - 1; i++)
	{
		char b = char_after_dot(I0->prod[i]);
		if (is_non_terminal(b))
		{
			for (int j = 0; G->prod.size() - 1; j++)
			{
				if (G->prod[j][0] == b)
				{
					if (!in_state(I0, G->prod[j]))
					{
						I0->prod.push_back(G->prod[j]);
					}
				}
			}
		}
	}
}


//Creation of a state 
void goto_state(struct state *I, struct state *S, char a, struct state I0)
{

	int time = 1;
	for (int i = 0; i < I->prod.size(); i++)
	{
		if (I->prod[i].length() - 1 == I->prod[i].find('.'))
			continue;
		if (char_after_dot(I->prod[i]) == a)
		{
			if (time == 1)
			{
				time++;
			}
			(S->prod.push_back(move_dot(I->prod[i])));
		}
	}

	while (true)
	{
		int s = S->prod.size();
		for (int i = 0; i < S->prod.size(); i++)
		{
			if (S->prod[i].length() - 1 == S->prod[i].find('.'))
				continue;

			if (is_non_terminal(char_after_dot(S->prod[i])))
			{
				// cout<<"State: "<<char_after_dot(S->prod[i]);
				for (int temp = 0; temp < I0.prod.size(); temp++)
				{

					if (I0.prod[temp][0] == char_after_dot(S->prod[i]))
					{
						// cout<<"\n"<<I0.prod[temp][0]<<" "<<S->prod[i]<<"PROD ";;
						int found = 0;
						for (int ptr = 0; ptr < S->prod.size(); ptr++)
						{
							if (S->prod[ptr].compare(I0.prod[temp]) == 0)
								found = 1;
						}

						if (!found)
						{
							S->prod.push_back(I0.prod[temp]);
						}
					}
				}
			}
		}
		if (S->prod.size() == s)
			break;
	}
}


//Check if a state is already present in the canonical set of items
int checkIfalready(struct state s)
{
	// cout<<"checkIfAlready\n";
	int flag = 0;
	struct canonical filter;
	vector<int> index;
	for (int i = 0; i < canonSet.states.size(); i++)
	{
		struct state j = canonSet.states[i];
		if (j.prod.size() == s.prod.size())
		{
			filter.states.push_back(j);
			index.push_back(i);
		}
	}
	if (filter.states.size() == 0)
		return -1;
	else
	{
		int noofStates = 0;
		for (int q = 0; q < filter.states.size(); q++)
		{
			int count = 0;
			for (int r = 0; r < filter.states[q].prod.size(); r++)
			{
				for (int z = 0; z < s.prod.size(); z++)
				{
					if (strcmp(filter.states[q].prod[r].c_str(), s.prod[z].c_str()) == 0)
					{
						
						count += 1;
					}
				}
			}
			// cout<<"\n\n\n";

			if (count != filter.states[q].prod.size())
				noofStates += 1;
			if (count == filter.states[q].prod.size())
				return index[q];
		}
		if (noofStates > 0)
			return -1;
	}
	return -1;
}


//Creation of the canonical set.
void canonicalSet(struct state *I0)
{
	canonSet.states.push_back(*I0);

	vector<char> symbList{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'A', 'B', 'C', 'D', 'G', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'z', 'x', '=', '(', ')', '{', '}', '?',',', ':', ';', '>', '<', '+', '-', '*','~'};
	int prev_size = 1;
	while (true)
	{
		int si = canonSet.states.size();
		
		for (int i = 0; i < si; i++)
		{
			for (int j = 0; j < symbList.size(); j++)
			{
				struct state s;

				goto_state(&canonSet.states[i], &s, symbList[j], *I0);
				
				if (s.prod.size() != 0 && checkIfalready(s) == -1)
				{
					
					canonSet.states.push_back(s);

					struct GotoTrans temp;
					temp.originState = i;
					temp.finalState = canonSet.states.size() - 1;
					temp.input = symbList[j];
					gotoList.push_back(temp);
					
				}
				else if (s.prod.size() != 0)
				{
					struct GotoTrans temp;
					temp.originState = i;
					temp.finalState = checkIfalready(s);
					temp.input = symbList[j];
					gotoList.push_back(temp);
				}
			}
			
		}
		if (prev_size == canonSet.states.size())
			break;
		
		prev_size = canonSet.states.size();
	}
	
}


//Defining the list of terminals and non-terminals used in the grammer
std::vector<char> terminals{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'z', 'x', '=', '(', ')', '{', '}','~', '?', '$', ':', ';', '>', '<', '+', '-', '*', '~',','};
std::vector<char> nonterminals{'A', 'B', 'C', 'D', 'G', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'};

//Get the position of terminal column in the Parser Table
int findposT(char c)
{
	for (int i = 0; i < terminals.size(); ++i)
	{
		if (terminals[i] == c)
			return i;
	}
	
}

//Get the position of non-terminal row in the Parser Table
int findposNT(char c)
{
	for (int i = 0; i < nonterminals.size(); i++)
	{
		if (nonterminals[i] == c)
			return i;
	}
	
}

//Get the Next state when an input symbol arrives
int findposgoto(int st, char c)
{
	for (int i = 0; i < gotoList.size(); ++i)
	{
		if (gotoList[i].originState == st && gotoList[i].input == c)
			return gotoList[i].finalState;
	}
	
}

void initializeFirstMap(map<char, string> &m)
{
	for (int i = 0; i < nonterminals.size(); i++)
	{
		m.insert({nonterminals[i], ""});
	}
}

//m is the map for first pos.
//b is added to FIRST(a)
void add_to_first(char a, map<char, string> &m, char b)
{
	map<char, string>::iterator itr = m.find(a);
	string str;
	if (itr != m.end())
		str = itr->second;

	if (str.find(b) == string::npos)
		itr->second += (b);
}
//FIRST(n) addded to FIRST(m)
void add_to_first(char m, char n, map<char, string> &mp)
{
	map<char, string>::iterator itr1 = mp.find(n);
	map<char, string>::iterator itr2 = mp.find(m);
	string str1 = itr1->second;
	string str2 = itr2->second;
	for (int i = 0; i < str1.length(); i++)
	{
		int flag = 0;
		for (int j = 0; j < str2.length(); j++)
		{
			if (str1.at(i) == str2.at(j))
				flag = 1;
		}
		if (flag == 0)
			add_to_first(m, mp, str1.at(i));
	}
}
//m is the map for follow pos.
//b is added to FOLLOW(n)
void add_to_follow(char n, map<char, string> &m, char b) //b added to n
{
	//cout<<"In Follow";
	if (b == 'x')
		return;
	map<char, string>::iterator itr = m.find(n);
	string str;
	if (itr != m.end())
		str = itr->second;

	if (str.find(b) == string::npos)
		itr->second += (b);
}
//FOLLOW(n) is added to FOLLOW(m)
void add_to_follow(char m, char n, map<char, string> &mp) //n is added to m
{
	map<char, string>::iterator itr1 = mp.find(n);
	map<char, string>::iterator itr2 = mp.find(m);
	string str1 = itr1->second;
	string str2 = itr2->second;
	for (int i = 0; i < str1.length(); i++)
	{
		int flag = 0;
		for (int j = 0; j < str2.length(); j++)
		{
			if (str1.at(i) == str2.at(j))
				flag = 1;
		}
		if (flag == 0)
			add_to_follow(m, mp, str1.at(i));
	}
}
//FIRST(n) is added to FOLLOW(m)
void add_to_follow_first(char m, char n, map<char, string> &mpfirst, map<char, string> &mpfollow)
{
	map<char, string>::iterator itr1 = mpfirst.find(n);
	map<char, string>::iterator itr2 = mpfollow.find(m);
	string str1 = itr1->second;
	string str2 = itr2->second;
	for (int i = 0; i < str1.length(); i++)
	{
		int flag = 0;
		for (int j = 0; j < str2.length(); j++)
		{
			if (str1.at(i) == str2.at(j))
				flag = 1;
		}
		if (flag == 0)
			add_to_follow(m, mpfollow, str1.at(i));
	}
}
//Terminal a added to FIRST(A). A->aB.
void find_first(struct state *I, map<char, string> &mpfirst)
{
	vector<string> list;
	list = I->prod;
	for (int i = 0; i < list.size(); i++)
	{
		if (is_non_terminal(list[i].at(0)) == true)
			if (is_non_terminal(list[i].at(3)) == false) //check position before executing. May be 2 or 4.
				add_to_first(list[i].at(0), mpfirst, list[i].at(3));
	}
}
//Terminal a added to FOLLOW(B). A->Ba
void find_follow(struct state *I, map<char, string> &mpfollow)
{
	vector<string> list;
	list = I->prod;
	for (int i = 0; i < list.size(); i++)
	{
		for (int j = 3; j < list[i].length(); j++) //check the index. May be 2 or 4.
		{
			if (is_non_terminal(list[i].at(j)) == true)
			{								//cout<<"\nbefore compare\n";
				if (j + 1 < list[i].size()) //if(list[i].at(j+1)!='\0')
				{
					//cout<<"\nlol";
					if (is_non_terminal(list[i].at(j + 1)) == false)
					{ //cout<<"before follow\n";
						add_to_follow(list[i].at(j), mpfollow, list[i].at(j + 1));
					}
				}
			}
		}
	}
}

//Action table and goto table
string actiontable[200][100];
int gototable[200][100];


//Creation of the SLR parsing table
void SLRParsingTable(map<char, string> followmap)
{
	// vector<vector<int> > vec( n , vector<int> (m));
	for (int sta = 0; sta < canonSet.states.size(); sta++)
	{
		for (int pro = 0; pro < canonSet.states[sta].prod.size(); pro++)
		{
			string temp = canonSet.states[sta].prod[pro];
			if (temp.compare("A->B.") == 0)
			{
				actiontable[sta][findposT('$')] = "accept";
				continue;
			}
			int dotpos = temp.find('.');
			if (dotpos == temp.length() - 1)
			{
				// actiontable[sta];//////Todo
				string followOfNT = followmap.find(temp[0])->second;
				for (int pos = 0; pos < followOfNT.length(); pos++)
				{
					string reduce = "r ";
					reduce = reduce + temp.substr(0, temp.length() - 1);
					(actiontable[sta][findposT(followOfNT[pos])] = reduce);
				}
				continue;
			}
			string s = "s ";
			s = s + to_string(findposgoto(sta, temp[dotpos + 1]));
		
			(actiontable[sta][findposT(temp[dotpos + 1])] = s);
		
		}
	}
	for (int count = 0; count < gotoList.size(); count++)
	{
		if (gotoList[count].input >= 'A' && gotoList[count].input <= 'Z')
			gototable[gotoList[count].originState][findposNT(gotoList[count].input)] = gotoList[count].finalState;
	}
	
	fstream ag;
	ag.open("./output/Parsing Table.csv",ios::out);
	ag<<"\nStates V || Symbols->"<<setw(10);
	for (int h = 0; h < terminals.size(); h++)
	{
		ag << terminals[h] <<"|"<< setw(15);
	}
	ag << "\n"<<setw(15);
	for (int h = 0; h < terminals.size(); h++)
	{
		ag << "-----------------" << setw(15);
	}
	ag << "\n"<<setw(15);
	for (int sta = 0; sta < canonSet.states.size(); sta++)
	{
		ag << sta<<"|"<<setw(15);
		for (int h = 0; h < terminals.size(); h++)
		{
			string output = actiontable[sta][h];
			ag << output<<"|" << setw(15);
		}
		
		ag << endl;
	}
	ag.close();
}

//function which parses the input string and checks if it is correct according to the grammer defined
bool parser(string in)
{
	char input[in.length() + 1];
	strcpy(input, in.c_str());
	char nextInput = input[0];
	int count = 0;
	stack<int> parserStack;
	parserStack.push(0);
	bool xcheck = false;

	fstream f;
	f.open("./output/ParsingSteps.txt",ios::out);
	f << "Action "<<setw(20)
				 << "Production/State" <<setw(20)<< "Next Input" <<setw(20)<<" Next State" <<setw(20)<< "\n";
			;
	f << "-------------------------------------------------------------------------------\n";
			;
	while (true)
	{
		int top = parserStack.top();
		string action = actiontable[top][findposT(nextInput)];
		
		if (action.find("s") != string::npos)
		{
			parserStack.push(stoi(action.substr(action.find(" "))));
			f << "Shift  "<<setw(20)
				 << action.substr(action.find(" "))<<setw(20) << nextInput<<setw(20) << "\n";
			nextInput = input[++count];
			xcheck = false;
		}
		else if (action.find("r") != string::npos)
		{
			string rhsProd = action.substr(5);
			for (int i = 0; i < rhsProd.length(); i++)
				parserStack.pop();
			int t = parserStack.top();
			f << "Reduce "<<setw(20)
				 << action.substr(2) <<setw(20)<< nextInput <<setw(20) << gototable[t][findposNT(action[2])] <<setw(20)<< "\n";
			;
			parserStack.push(gototable[t][findposNT(action[2])]);
			xcheck = false;
			//output production in action
		}
		else if (action.compare("accept") == 0)
		{
			return true;
		}
		else
		{
			if (xcheck)
				return false;
			nextInput = 'x';
			count--;
			xcheck = true;
		}
	}
}

//Start of parser.
bool parse(string str)
{
	vector<string> l;
	l = prod_ar();

	map<int, string> k;
	k = prod_index();

	struct state I;
	I.prod = l;
	map<char, string> firstmap, followmap;

	//------------------------------------------------------//
	initializeFirstMap(firstmap);
	find_first(&I, firstmap);
	//FIRST Rules
	for (int t = 0; t < 5; t++)
	{
		for (int abc = 0; abc < nonterminals.size(); abc++)
		{
			for (int i = 0; i < l.size(); i++)
			{
				int j = 3;
				while (true)
				{
					if (is_non_terminal(l[i][j]))
					{
						add_to_first(l[i][0], l[i][j], firstmap);//A->B. FIRST(B) added to FIRST(A)
					}

					else//Terminal a added to FIRST(A). A->aB
					{
						add_to_first(l[i][0], firstmap, l[i][j]);
						break;
					}
					map<char, string>::iterator itr = firstmap.find(l[i][j]);//FIRST is added for the case. A->BC. B->x.
					string str = itr->second;
					int foundx = str.find('x');
					if (foundx != string::npos)
					{
						j++;
						if (j == l[i].length())
							break;
					}
					else
						break;
				}
			}
		}
	}
	initializeFirstMap(followmap);
	find_follow(&I, followmap);
	add_to_follow('A', followmap, '$');
	
	int t = 0;
	while (t < 5)
	{
		
		for (int abc = 0; abc < nonterminals.size(); abc++)//FOLLOW is calculated.
		{
			for (int i = 0; i < l.size(); i++)
			{
				for (int k = 3; k < l[i].length(); k++)
				{
					
					if (l[i][k] == nonterminals[abc])
					{
						if (is_non_terminal(l[i][k + 1]))//A->aBC. FOLLOW(B) includes FIRST(C)
						{
							//flag=true;
							add_to_follow_first(nonterminals[abc], l[i][k + 1], firstmap, followmap);
							for (int gh = k + 1; gh < l[i].length() - 1; gh++)
							{
								map<char, string>::iterator itr = firstmap.find(l[i][gh]);
								string str = itr->second;
								int foundx = str.find('x');
								if (foundx != string::npos)
								{
									if (is_non_terminal(l[i][gh + 1]) != true)
									{
										add_to_follow(nonterminals[abc], followmap, l[i][gh + 1]);
										break;
									}
									add_to_follow_first(nonterminals[abc], l[i][gh + 1], firstmap, followmap);
								}
								else
								{
									//add_to_follow_first(nonterminals[abc],l[i][gh+1],firstmap,followmap);
									break;
								}
							}
						}
						if (l[i][k + 1] == '\0')//A->BC. FOLLOW(A) is added to FOLLOW(C). If C->x, then FOLLOW(A) is added to FOLLOW(B).
						{
							//flag=true;
							//cout<<"\n-------------------"<<nonterminals[abc]<<"-------------------------fgnxfgnsnsrn\n";
							add_to_follow(nonterminals[abc], l[i][0], followmap);
							int k2 = k;
							while (k2 > 3)
							{
								if (is_non_terminal(l[i][k2 - 1]) != true)
									break;
								map<char, string>::iterator itr = firstmap.find(l[i][k2]);
								string str = itr->second;
								int foundx = str.find('x');
								if (foundx != string::npos)
								{
									add_to_follow(nonterminals[l[i][k2 - 1]], l[i][0], followmap);
								}
								k2--;
							}
						}
					}
				}
			}
		}
		t++;
	}
	
	add_dots(&I);
	
	canonicalSet(&I);
	fstream f;
	f.open("./output/StatesFile.txt",ios::out);
	for (int q = 0; q < canonSet.states.size(); q++)
	{
		f <<((q<10)?"0":"")<< q << "  ----  "<<setw(10)<<std::left;
		for (int r = 0; r < canonSet.states[q].prod.size(); r++)
		{
			f<< canonSet.states[q].prod[r]<<setw(11)<<std::left;
			f<<"\n";
		}
		f << "\n";
	}
	f.close();
	SLRParsingTable(followmap);
	
	f.open("./output/GotoTransitions.txt",ios::out);
	f<<setw(15)<<"I/P State"<<setw(10)<<" Input"<<setw(10)<<"Next State"<<setw(10)<<"\n";
	for (int i = 0; i < gotoList.size(); i++)
	{
		f << gotoList[i].originState <<setw(10)<< " , " <<gotoList[i].input<<setw(10)<<" => " << gotoList[i].finalState<<setw(10)<< "\n";
	}
	f.close();
	actiontable[0][0] = "s 1";
	cout<<"\nInput To The Parser: \n ";
	cout << str <<"\n\n";
	return parser(str);
}
