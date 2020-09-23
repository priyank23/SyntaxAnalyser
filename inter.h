#include <bits/stdc++.h>
using namespace std;

string printint(vector< tuple<int, string, int, int, int> > store)
{
	map<string,char> mp;
	mp.insert(pair<string,char>("main",'a'));
	mp.insert(pair<string,char>("int",'h'));
	mp.insert(pair<string,char>("get",'f'));
	mp.insert(pair<string,char>("put",'g'));
	mp.insert(pair<string,char>("char",'i'));
	mp.insert(pair<string,char>("return",'j'));

	string output="";

	//ofstream outp;
	//outp.open("Intermediatefile.txt");
	for(int i=0;i<store.size();i++)
	{
			int type=get <2> (store[i]);
			if(type==0)
			{
				map<string,char>::iterator it=mp.find(get <1> (store[i]));
				//outp << it->second;
				output=output+it->second;
			}
			//else if(type==1)
			//	outp <<"2"<<" ";
			else if(type==2)//(id,b)
				//outp <<"b";
				output=output+"b";
			else if(type==7)//(number,c)
				//outp<<"c";(
				output=output+"c";
			else if(type==9)//(character,i)
				//outp<<"d";
				output=output+"d";
			else if(type==99){
				cout<<"Invalid token found: "<<get<1>(store[i])<<endl;;
				return "";
			} else {
				output = output+get<1>(store[i]);
			}
				

	}
	//outp.close();
	output += "$";
	
 	return output;
}
/*int main()
{
	vector< tuple<int, string, int, int, int> > tokens;
	//tokens.push_back( make_tuple(0,"int",0,1,1));
	tokens.push_back( make_tuple(1,"main",0,1,5));
	tokens.push_back( make_tuple(2,"(",6,1,9));
	tokens.push_back( make_tuple(3,")",6,1,10));
	tokens.push_back( make_tuple(4,"{",6,1,12));
	tokens.push_back( make_tuple(5,"int",0,4,3));
	tokens.push_back( make_tuple(6,"a",2,4,7));
	tokens.push_back( make_tuple(7,",",6,4,8));
	tokens.push_back( make_tuple(8,"b",2,4,9));
	tokens.push_back( make_tuple(9,",",6,4,10));
	tokens.push_back( make_tuple(9,"c",2,4,11));
	tokens.push_back( make_tuple(10,";",6,4,12));
	tokens.push_back( make_tuple(11,"char",0,5,1));
	tokens.push_back( make_tuple(11,"d",2,5,1));
	tokens.push_back( make_tuple(11,"=",4,5,1));
	tokens.push_back( make_tuple(11,"'a'",9,5,1));
	tokens.push_back( make_tuple(11,";",6,5,1));
	tokens.push_back( make_tuple(11,"a",2,6,1));
	tokens.push_back( make_tuple(11,"=",4,6,1));
	tokens.push_back( make_tuple(11,"5",7,6,1));
	tokens.push_back( make_tuple(11,";",6,6,1));
	tokens.push_back( make_tuple(11,"get",0,7,1));
	tokens.push_back( make_tuple(11,"b",2,7,1));
	tokens.push_back( make_tuple(11,";",6,7,1));
	tokens.push_back( make_tuple(11,"c",2,8,1));
	tokens.push_back( make_tuple(11,"=",4,8,1));
	tokens.push_back( make_tuple(11,"a",2,8,1));
	tokens.push_back( make_tuple(11,"*",3,8,1));
	tokens.push_back( make_tuple(11,"b",2,8,1));
	tokens.push_back( make_tuple(11,";",6,8,1));
	tokens.push_back( make_tuple(11,"put",0,8,1));
	tokens.push_back( make_tuple(11,"c",2,8,1));
	tokens.push_back( make_tuple(11,";",6,8,1));
	tokens.push_back( make_tuple(11,"return",0,9,1));
	tokens.push_back( make_tuple(11,"c",2,9,1));
	tokens.push_back( make_tuple(11,";",6,9,1));
	tokens.push_back( make_tuple(11,"}",6,9,1));

	printint(tokens);
	return 0;
}
/*
main(){
int a,b,c;
char d='a';
a=5;
get b;
c=a*b;
put c;
return c;}
*/