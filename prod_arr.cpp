#include<vector>
#include<fstream>
#include<iostream>
#include<map>
#include<algorithm>
#include<string.h>
#include<stdlib.h>
using namespace std;
vector<string> prod_ar()
{
	ifstream fp("LetterCFG.txt");
	vector<string> prod;
	string line;
	
	while(!fp.eof())
	{
		getline(fp,line);
		if(line.size()==1)
			continue;
		else
			prod.push_back(line);
	}

	return prod;

}//one extra line is getting inserted at the end so use test condition as <(less than)vector.size()-1

map<int,string> prod_index()
{
	vector<string> l;
	l = prod_ar();
	map<int,string> index;
	
	for (int i=0;i<l.size()-1;i++)
	{
		index.insert({i,l[i]});
	}
	
	return index;
}


struct state
{
	vector<string> prod;
};
struct GotoTrans
{
	int originState,finalState;
	char input;
};

vector<GotoTrans> gotoList;

struct canonical{
	vector<struct state> states;
}canonSet;

void add_dots(struct state *I)
{
	vector<string> list;
	list = I->prod;
	for(int i=0;i<list.size()-1;i++)
	{
		int s = list[i].find('>');
		// if(list[i].find('x')>=0)continue;
		list[i].insert(s+1,".");
	}
	I->prod = list;
}

string move_dot(string b)
{
	// cout<<"\t\t\tBefore Moving"<<b.c_str()<<endl;
	int s = b.find('.');
	if(s==b.length()-1)
		return b;
	string t(1,b[s+1]);
	b.replace(s,1,t);
	b.replace(s+1,1,".");
	// cout<<"\t\t\tAfter Moving"<<b.c_str()<<endl;
	return b;
}
bool is_non_terminal(char a){
	if(a>='A' && a<='Z'	)
			return true;
	else
			return false;
}

bool in_state(struct state *I,string a)
{
	for(int i=0;i<I->prod.size()-1;i++)
	{
		if(I->prod[i]==a)
			return true;
	}
	return false;
}


char char_after_dot(string l)
{
	char b;
	int s = l.find('.');
	b = l[s+1];
	return b;
}


void closure(struct state *I0,struct state *G)
{
	for (int i=0; i<I0->prod.size()-1;i++)
	{
		char b = char_after_dot(I0->prod[i]);
		if(is_non_terminal(b))
		{
			for(int j=0;G->prod.size()-1;j++)
			{
				if(G->prod[j][0]==b)
				{
					if(!in_state(I0,G->prod[j]))
					{
						I0->prod.push_back(G->prod[j]);
					}
				}
			}
		}
	}
}

void goto_state(struct state *I, struct state *S, char a)
{

	int time=1;
	for(int i=0;i<I->prod.size();i++)
	{
		if(I->prod[i].length()-1==I->prod[i].find('.'))
			continue;
		if(char_after_dot(I->prod[i])==a)
		{
			if(time==1)
			{
				time++;
			}
			(S->prod.push_back(move_dot(I->prod[i])));
		}
		
	}	
	
}		


bool checkIfalready(struct state s){
	// cout<<"checkIfAlready\n";
	int flag=0;
	struct canonical filter;
	for(int i=0;i<canonSet.states.size();i++){
		struct state j=canonSet.states[i];
		if(j.prod.size()==s.prod.size())
			{filter.states.push_back(j);}
	}
	if(filter.states.size()==0)
		return false;
	else  {
		int noofStates=0;
		for(int q=0;q<filter.states.size();q++){
			int count=0;
		for(int r=0;r<filter.states[q].prod.size();r++){
			
			for(int z=0;z<s.prod.size();z++)
			{
				if(strcmp(filter.states[q].prod[r].c_str(),s.prod[z].c_str())==0)
			{
			// cout<<"\t\t"<<s.prod[z]<<"@@@@@@"<<filter.states[q].prod[r]<<"@@@@@@";			
			count+=1;}
			}

			}
			// cout<<"\n\n\n";

			if (count!=filter.states[q].prod.size())
				noofStates+=1;
			if (count==filter.states[q].prod.size())
				return true;
	}
	if(noofStates>0)
		return false;

	return true;}
}


void canonicalSet(struct state *I0){
	canonSet.states.push_back(*I0);

	for (int z = 0; z < (*I0).prod.size(); ++z)
	{
	cout<<(*I0).prod[z]<<" @ ";
	}
	vector<char> symbList{'a','b','c','d','e','f','g','h','i','j','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','z','=','(',')','{','}','?',':',';','>','<','+','-','*'};
	int prev_size=1;
	while(true){
		int si=canonSet.states.size();
		// if(si>500)
		// 	break;
		// cout<<"Size of set: "<<si<<endl;
		for(int i=0;i<si;i++){
			for(int j=0;j<symbList.size();j++){
				struct state s;
				
				goto_state(&canonSet.states[i],&s,symbList[j]);
				if(s.prod.size()!=0 && checkIfalready(s)==false)
				{
				// 	cout<<" Goto "<<symbList[j]<<" : "<<i<<" -> "<<(si+1)<<endl; 
				canonSet.states.push_back(s);
				struct GotoTrans temp;
				temp.originState=i;
				temp.finalState=canonSet.states.size()-1;
				temp.input=symbList[j];
				gotoList.push_back(temp);
				}
			}
		}
		if(prev_size==canonSet.states.size())
			break;
		// cout<<"Next Iter\n";
		prev_size=canonSet.states.size();
	}
	cout<<canonSet.states.size();
}

std::vector<char> terminals{'a','b','c','d','e','f','g','h','i','j','z','x','=','(',')','{','}','?',':',';','>','<','+','-','*'};
std::vector<char> nonterminals{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R'};
	
int findposT(char c){
	for (int i = 0; i < terminals.size(); ++i)
	{
		if(terminals[i]==c)
			return i;
	}
}
int findposNT(char c){
	for (int i = 0; i < nonterminals.size(); ++i)
	{
		if(nonterminals[i]==c)
			return i;
	}
}
int findposgoto(int st,char c){
	for (int i = 0; i < gotoList.size(); ++i)
	{
		if(gotoList[i].originState==st&&gotoList[i].input==c)
			return gotoList[i].finalState;
	}
}

void SLRParsingTable(){
	// vector<vector<int> > vec( n , vector<int> (m));
	string actiontable[canonSet.states.size()][terminals.size()];
	int gototable[canonSet.states.size()][nonterminals.size()];
	for(int sta=0;sta<canonSet.states.size();sta++){
		for(int pro=0;pro<canonSet.states[sta].prod.size();pro++){
			string temp=canonSet.states[sta].prod[pro];
			int dotpos=temp.find('.');
			if(dotpos==temp.length()-1){
				// actiontable[sta];//////Todo
				continue;
			}
			string s="s ";
			s=s+to_string(findposgoto(sta,temp[dotpos+1]));
			(actiontable[sta][findposT(temp[dotpos+1])]=s);

		}
	}
	for(int count=0;count<gotoList.size();count++)
	{
		if(gotoList[count].input>='A' &&gotoList[count].input<='Z')
			gototable[gotoList[count].originState][findposNT(gotoList[count].input)]=gotoList[count].finalState;
	}
	for(int sta=0;sta<canonSet.states.size();sta++){
		for(int h=0;h<terminals.size();h++){
			cout<<actiontable[sta][h]<<"\t";
		}
		for(int h=0;h<nonterminals.size();h++){
			cout<<gototable[sta][h]<<"\t";
		}
		cout<<endl;
	}

}
void initializeFirstMap(map<char,string> &m){
	for(int i=0;i<terminals.size();i++){
		m.insert({terminals[i],""});
	}
	for(int i=0;i<nonterminals.size();i++){
		m.insert({nonterminals[i],""});
	}
}

//m is the map for first pos. 
void add_to_first(char a,map<char,string> &m,char b)
{
	map<char,string>::iterator itr=m.find(a);
	string str;
	if(itr!=m.end())
		str=itr->second;
	
	
    	if(str.find(b)==string::npos)
    		itr->second+=(b);
}
//FIRST(n) addded to FIRST(m)
void add_to_first(char m,char n,map<char,string> &mp)
{
	map<char,string>::iterator itr1=mp.find(n);
	map<char,string>::iterator itr2=mp.find(m);
	string str1=itr1->second;
	string str2=itr2->second;
	for(int i=0;i<str1.length();i++)
	{
		int flag=0;
		for(int j=0;j<str2.length();j++)
		{
            if(str1.at(i)==str2.at(j))
                flag=1;
        }
        if(flag==0)
            add_to_first(m,mp,str1.at(i));
	}
}
//m is the map for follow pos.
void add_to_follow(char n,map<char,string> &m,char b)
{
	cout<<"In Follow";
	map<char,string>::iterator itr=m.find(n);
	string str;
	if(itr!=m.end())
		str=itr->second;
	
    if(str.find(b)!=string::npos)
    	itr->second.push_back(b);
}

void add_to_follow(char m,char n,map<char,string> &mp)
{
	map<char,string>::iterator itr1=mp.find(n);
	map<char,string>::iterator itr2=mp.find(m);
	string str1=itr1->second;
	string str2=itr2->second;
	for(int i=0;i<str1.length();i++)
	{
		int flag=0;
		for(int j=0;j<str2.length();j++)
		{
            if(str1.at(i)==str2.at(j))
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,mp,str1.at(i));
	}
}

void add_to_follow_first(char m,char n,map<char,string> &mpfirst,map<char,string> &mpfollow)
{
    map<char,string>::iterator itr1=mpfirst.find(n);
	map<char,string>::iterator itr2=mpfollow.find(m);
	string str1=itr1->second;
	string str2=itr2->second;
	for(int i=0;i<str1.length();i++)
	{
		int flag=0;
		for(int j=0;j<str2.length();j++)
		{
            if(str1.at(i)==str2.at(j))
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,mpfollow,str1.at(i));
	}
}

void find_first(struct state *I,map <char,string> &mpfirst)
{
	vector<string> list;
	list = I->prod;
	for(int i=0;i<list.size()-1;i++)
	{
		if(is_non_terminal(list[i].at(0))==true)
			if(is_non_terminal(list[i].at(3))==false)//check position before executing. May be 2 or 4.
				add_to_first(list[i].at(0),mpfirst,list[i].at(3));
	}
}

void find_follow(struct state *I,map <char,string> &mpfollow)
{
	vector<string> list;
	list = I->prod;
	for(int i=0;i<list.size()-1;i++)
	{
		for(int j=3;j<list[i].length();j++)//check the index. May be 2 or 4.
		{
			if(is_non_terminal(list[i].at(j))==true)
			{cout<<"before compare\n";
				if(list[i].at(j+1)!='\0')
				{
					if(is_non_terminal(list[i].at(j+1))==false)
						{cout<<"before follow\n";add_to_follow(list[i].at(j),mpfollow,list[i].at(j+1));}
				}
			}
		}
	}
}


void populateFirst(map<char,string> &firstMap,struct state *I){
	vector<string> list=I->prod;
	
	for(int i=0;i<terminals.size();i++){
		map<char,string>::iterator itr=firstMap.find(terminals[i]);
			if(itr!=firstMap.end())
				{itr->second+=terminals[i];}
	}
	while(true){
		int change=0;cout<<"Turn\n\n";
		for(int i=0;i<list.size();i++){
			char fChar=list[i][0];
			int flagEpsilon=0;
			for(int j=3;j<list[i].size();j++){
				if(flagEpsilon)break;
				if(list[i][j]=='.')
				continue;
				if(!is_non_terminal(list[i][j]))
				{
					map<char,string>::iterator itr=firstMap.find(list[i][0]);
					if(itr!=firstMap.end())
						{	
							if(itr->second.find(list[i][j])==string::npos)
							{
								itr->second+=list[i][j];change++;
							}
						}
				}
				else
				{
					if(firstMap.find(list[i][j])->second.find('x')!=string::npos)
					{
						if(firstMap.find(list[i][0])->second.find('x')==string::npos)
							{firstMap.find(list[i][0])->second+='x';change++;}
						string s1=firstMap.find(list[i][j])->second;
						string s2=firstMap.find(list[i][0])->second;
						// cout<<s1<<" @@@ "<<s2<<endl;
						string news2=s2;
						for(int it=3;it<=s1.size();it++){
							if(s2.find(s1[it],3)==string::npos)
								{news2+=s1[it];change++;}
						}
						firstMap.find(list[i][0])->second=news2;
					}
					else flagEpsilon=1;
				}
				
			}
		}
		for(int i=0;i<list.size();i++){
			if(list[i].find('x')!=string::npos){
				map<char,string>::iterator itr=firstMap.find(list[i][0]);
				if(itr!=firstMap.end())
				{
					if(itr->second.find('x')==string::npos) 
					{itr->second+='x';change++;}
				}
			}
		}
		
		if(change==0)
		break;

	}
	
}

void populateFollow(map<char,string> &followMap,map<char,string> &firstMap,struct state *I){
	followMap.insert({'A',"$"});
	for(int i=0;i<nonterminals.size();i++){
		if(nonterminals[i]!='A')
		followMap.insert({nonterminals[i],""});
	}
	vector<string> list=I->prod;
	while(true){
		int change=0;
		
		for(int i=0;i<list.size();i++){
			// cout<<"&&&&&    "<<list[i]<<endl;
			for(int j=4;j<list[i].size();j++){
				
				if(is_non_terminal(list[i][j]))
				{
					for(int k=j+1;k<list[i].size();k++){
						// cout<<"1\n";
						string s2=followMap.find(list[i][j])->second;
						// cout<<"S2: "<<s2<<endl;
						// cout<<"2\n";
						string s1;
						if(followMap.find(list[i][k])!=followMap.end())
						s1=firstMap.find(list[i][k])->second;
						// cout<<"3\n";
						string news2=s2;
						// cout<<s1<<" s2 "<<s2<<endl;
						for(int it=0;it<s1.size();it++){
							// cout<<"s2: "<<s2<<" char: "<<s1[it]<<endl;
							if(s2.find(s1[it])==string::npos)
								{news2+=s1[it];change++;cout<<"news2: "<<news2<<endl;}
						}
						// cout<<"4\n";
						followMap.find(list[i][j])->second=news2;
						// cout<<"5\n";
					}
				}
			}
		}
		cout<<"3rdStep\n";
		
		if(change==0)
		break;
	}
	int change=0;
	for(int i=list.size()-1;i>=0;i--){
			if(is_non_terminal(list[i][list[i].size()-1]))
			{
				string s2=followMap.find(list[i][0])->second;
				string s1=followMap.find(list[i][list[i].size()-1])->second;
				string news2=s1;
				for(int it=0;it<s2.size();it++){
					if(s1.find(s2[it])==string::npos)
						{news2+=s2[it];cout<<"news3: "<<news2<<endl;change++;}
				}
				// cout<<"4\n";
				followMap.find(list[i][list[i].size()-1])->second=news2;
			}
			for(int j=4;j<list[i].size();j++){
				if(is_non_terminal(list[i][j]))
				{
					string first="";
					for(int k=j+1;k<list[i].size();k++){
						// cout<<"2#\n";
						string init=first;
						string newS;
						if(firstMap.find(list[i][k])!=firstMap.end())
						newS=firstMap.find(list[i][k])->second;
						for(int it=0;it<newS.size();it++){
							// cout<<"3#\n";
							if(init.find(newS[it])==string::npos)
								{init+=newS[it];change++;}
							}
						first=init;
					}
					if(first.find('x')!=string::npos)
					{
						string s2=followMap.find(list[i][0])->second;
						string s1=followMap.find(list[i][j])->second;
						string news2=s1;
						for(int it=0;it<s2.size();it++){
							// cout<<"5#\n";
							if(s1.find(s2[it])==string::npos)
								{news2+=s2[it];cout<<"news3: "<<news2<<endl;change++;}
						}
						// cout<<"4\n";
						followMap.find(list[i][j])->second=news2;
					}

				}
			}
			
		}
		
	
}
int main()
{
	vector<string> l;
	l = prod_ar();
	cout<<l.size()<<endl;;

	
	for (int i=0;i<l.size()-1;i++)
	{
		cout<<l[i]<<"\n";
	}


	map<int,string> k;
	k = prod_index();
	for(auto itr = k.begin(); itr!=k.end();itr++)
	{
		cout<<itr->first<<"	"<<itr->second;
		cout<<"\n";
	}


	struct state I;
	I.prod = l;
	add_dots(&I);
	for (int i=0;i<(I.prod).size()-1;i++)
	{
		cout<<I.prod[i]<<"\n";
	}
	canonicalSet(&I);
	for(int q=0;q<canonSet.states.size();q++){
		for(int r=0;r<canonSet.states[q].prod.size();r++){
			cout<<canonSet.states[q].prod[r]<<"    ";
		}
		cout<<"\n";
	}
	map<char,string> followmap,firstmap;
	initializeFirstMap(firstmap);
	populateFirst(firstmap,&I);

	for (map<char,string>::iterator itr = firstmap.begin(); itr != firstmap.end(); ++itr) { 
        cout << '\t' << itr->first 
             << '\t' << itr->second << '\n'; 
    } 
	populateFollow(followmap,firstmap,&I);

	for (map<char,string>::iterator itr = followmap.begin(); itr != followmap.end(); ++itr) { 
        cout << '\t' << itr->first 
             << '\t' << itr->second << '\n'; 
    } 
	
	// SLRParsingTable();
	cout<<"\n\n\n";
	
	// find_follow(&I,followmap);
return 0;
}

	
		
