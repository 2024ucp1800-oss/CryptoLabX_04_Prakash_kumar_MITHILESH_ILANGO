#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <cctype>

using namespace std;
string clean_ciphertext(string &s) {
    string res = "";
    for(char c : s) {
        if(isalpha(c)) {
            res += toupper(c);
        }
    }
    return res;
}

unordered_map<string, vector<int>> find_repeated_patterns(int n, string& final) {

    unordered_map<string, vector<int>> mp;
    for (int i = 0; i+n <= final.size(); i++) {
        string seq = final.substr(i, n);
        mp[seq].push_back(i);
    }

    return mp;
}

vector<int> calculate_distances(unordered_map<string,vector<int>>&mp){
  vector<int>distance;
  for(auto p:mp){
    if(p.second.size()>1){
      vector<int>check=p.second;
      int dis;
      for(int i=1;i<check.size();i++){
        dis=check[i]-check[i-1];
        distance.push_back(dis);
      }
    }
  }
  return distance;
}

vector<int>find_factors(vector<int>&dis){
  
}

int main(){

  ifstream file("ciphertext.txt");

  string ciphertext;
  string line;
  
  while (getline(file, line)) {
        ciphertext += line;
  }
  file.close();

  string final=clean_ciphertext(ciphertext);

  unordered_map<string,vector<int>>len_3=find_repeated_patterns(3,final);
  unordered_map<string,vector<int>>len_4=find_repeated_patterns(4,final);
  unordered_map<string,vector<int>>len_5=find_repeated_patterns(5,final);

  return 0;
}
