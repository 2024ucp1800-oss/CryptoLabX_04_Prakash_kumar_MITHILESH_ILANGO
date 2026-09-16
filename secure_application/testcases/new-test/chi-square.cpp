#include <iostream>
#include<bits/stdc++.h>
using namespace std;

//chi-sqaure analysis in shift cipher
string ans="";
int mini=INT_MAX;
void chi_square(string&cipher){
  vector<double> englishFreq = {
    8.167, 1.492, 2.782, 4.253, 12.702,
    2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507,
    1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974,
    0.074
  };

  vector<double>observed(26,0);
  for(auto c:cipher){
     observed[c-'a']++;
  }

  double x_square=0;

  for(int i=0;i<26;i++){
    double e =
            (englishFreq[i] / 100.0) * cipher.size();
    double o=observed[i];

    double x=((e-o)*(e-o))/e;
    x_square+=x;
  }

  if(mini>x_square){
    mini=x_square;
    ans=cipher;
  }
}

int main() {
  string ciphertext="";
  string plaintext="thisisplaintextmadebyprakash";
  int key=5;

  for(auto c:plaintext){
    ciphertext+='a'+(c-'a'+key)%26;
  }
  for(int i=0;i<25;i++){
    string test="";

    for(auto c:plaintext){
      test+='a'+(c-'a'+i)%26;
    }

    chi_square(test);

  }
  cout<<ciphertext<<endl;
  cout<<ans;
    return 0;
}