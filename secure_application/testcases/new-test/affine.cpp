#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include<bits/stdc++.h>
using namespace std;

int main() {
  //affine cipher
  //key are a and b 
  // e(n)=(ax+b)mod 26
  // gcd(a,26)=1,0<=b<=25
  vector<int>a;
  for(int i=0;i<26;i++){
    if(gcd(i,26)==1){
      cout<<i<<endl;
      a.push_back(i);
    }
  }
  //a=5 b=4
  string plaintext="THIS IS A LONG SECRET MESSAGE FOR AFFINE CIPHER";
  string c_gen="";

  for(auto c : plaintext) {
    if(isalpha(c)) {
        int x = tolower(c) - 'a';
        int mod = (5 * x + 4) % 26;

        c_gen += char('A' + mod);
    }
  }
  cout<<plaintext<<endl;
  cout<<c_gen;

  //decrption


return 0;
}