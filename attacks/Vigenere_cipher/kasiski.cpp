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

map<int, int> find_factors(vector<int>& distances) {
    map<int, int> factor_count;
    for (int d : distances) {
        for (int i = 2; i <= d; i++) {
            if (d % i == 0) {
                factor_count[i]++;
            }
        }
    }
    return factor_count;
}
void kasiski(string &final) {
    map<int, int> total_factors;
    for(int n = 3; n <= 5; n++) {
        auto patterns = find_repeated_patterns(n, final);
        vector<int> distances = calculate_distances(patterns);
        map<int, int> factors = find_factors(distances);
        for(auto &p : factors) {
            total_factors[p.first] += p.second;
        }
    }
    cout << "\nKasiski Analysis\n";
    cout << "Candidate Key Lengths:\n";
    for(auto &p : total_factors) {
        cout << "Length: " << p.first
             << "  Score: " << p.second << endl;
    }
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

  kasiski(final);

  return 0;
}
