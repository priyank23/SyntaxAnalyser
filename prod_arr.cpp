#include<vector>
#include<fstream>
#include<iostream>
#include<map>

using namespace std;
vector<string> prod_ar()
{
	ifstream fp("augCFG.txt");
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

/* testing
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



return 0;
}
	
*/	
		
