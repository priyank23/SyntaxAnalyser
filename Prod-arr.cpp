#include<vector>
#include<fstream>
#include<iostream>
#include<map>
#include<algorithm>
#include<string.h>

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

void add_dots(struct state *I)
{
	vector<string> list;
	list = I->prod;
	for(int i=0;i<list.size()-1;i++)
	{
		int s = list[i].find('>');
		list[i].insert(s+1,".");
	}
	I->prod = list;
}

string move_dot(string b)
{
	int s = b.find('.');
	b.replace(s,1,b[s+1]);
	b.replace(s+1,1,'.');

	return b;
}
bool is_non_terminal(char a)
{
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
				if(G->prod[j][0]==a)
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
	for(int i=0;i<I->prod.size()-1;i++)
	{
		if(char_after_dot(I->prod[i])==a)
		{
			if(time==1)
			{
				time++;
			}
			strcpy(S->prod.push_back,move_dot(I-prod[i]));
		}
	}
}
			
//m is the map for first pos. 
void add_to_first(char a,map<char,string> &m,char b)
{
	iterator itr=m.find(a);
	string str=itr->second;
    if(str.find(b)!=string::npos)
    	m->second.append(b);
}
//FIRST(n) addded to FIRST(m)
void add_to_first(char m,char n,map<char,string> &mp)
{
	iterator itr1=mp.find(n);
	iterator itr2=mp.find(m);
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
            add_to_first(m,mp,str.at(i));
	}
}
//m is the map for follow pos.
void add_to_follow(char n,map<char,string> &m,char b)
{
	iterator itr=m.find(n);
	string str=itr->second;
    if(str.find(b)!=string::npos)
    	m->second.append(b);
}

void add_to_follow(char m,char n,map<char,string> &mp)
{
	iterator itr1=mp.find(n);
	iterator itr2=mp.find(m);
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
    iterator itr1=mpfirst.find(n);
	iterator itr2=mpfollow.find(m);
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

void find_follow(struct state *I,map <char,string> &mpfollow)
{
	vector<string> list;
	list = I->prod;
	for(int i=0;i<list.size()-1;i++)
	{
		for(int j=3;j<list[i].length();j++)//check the index. May be 2 or 4.
		{
			if(is_non_terminal(list[i].at(j))==true)
			{
				if(list[i].at(j+1)!='\0')
				{
					if(is_non_terminal(list[i].at(j+1))==false)
						add_to_follow(list[i].at(j),mpfollow,list[i].at(j+1));
				}
			}
		}
	}
}


/*
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
	
return 0;
}
*/
