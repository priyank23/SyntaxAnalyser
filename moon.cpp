#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the journeyToMoon function below.
int journeyToMoon(int n, vector<vector<int>> astronaut) {
    int arr[100000];
    int countMap[10000];
    memset(arr,0,sizeof(int));
    memset(countMap,0,sizeof(int));
    int countryCount=0;
    int noOfPairs=astronaut.size();
    for(int i=0;i<noOfPairs;i++){
        if(arr[astronaut[i][0]]==0 &&arr[astronaut[i][1]]==0)
        {
            countryCount++;
            arr[astronaut[i][0]]=countryCount;
            arr[astronaut[i][1]]=countryCount;
            countMap[countryCount]+=2;         
        }
        else if(arr[astronaut[i][0]]!=0 ^ arr[astronaut[i][1]]!=0){
            int val=(arr[astronaut[i][0]]==0)?1:0;
            countMap[arr[astronaut[i][val]]]+=1;
            arr[astronaut[i][1-val]]=arr[astronaut[i][val]];
        }
    }
    
    for(int i=0;i<n;i++){
        if(arr[i]==0)
        {countryCount++;arr[i]=countryCount;
         countMap[countryCount]=1;}
    }
    int noOfWays=0;
    for(int j=1;j<countryCount;j++){
        for(int k=j+1;k<=countryCount;k++)
            noOfWays+=(countMap[j]*countMap[k]);
     
    }
    return noOfWays;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string np_temp;
    getline(cin, np_temp);

    vector<string> np = split_string(np_temp);

    int n = stoi(np[0]);

    int p = stoi(np[1]);

    vector<vector<int>> astronaut(p);
    for (int i = 0; i < p; i++) {
        astronaut[i].resize(2);

        for (int j = 0; j < 2; j++) {
            cin >> astronaut[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = journeyToMoon(n, astronaut);

    cout << result << "\n";

    

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}