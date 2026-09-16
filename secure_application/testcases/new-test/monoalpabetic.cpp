#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <cctype>
#include<unordered_map>
using namespace std;

#define ll long long
#define endl '\n'
#define pb push_back
#define all(x) x.begin(), x.end()

void frequency(string& cipher, unordered_map<char,char>& key) {
    vector<double> freq = {
        8.167, 1.492, 2.782, 4.253, 12.702,
        2.228, 2.015, 6.094, 6.966, 0.153,
        0.772, 4.025, 2.406, 6.749, 7.507,
        1.929, 0.095, 5.987, 6.327, 9.056,
        2.758, 0.978, 2.360, 0.150, 1.974,
        0.074
    };
    vector<int> obs(26, 0);
    int total = 0;
    for (char c : cipher) {
        if (isalpha(c)) {
            c = tolower(c);
            obs[c - 'a']++;
            total++;
        }
    }
    vector<pair<int,char>> cipherFreq;
    for (int i = 0; i < 26; i++) {
        cipherFreq.push_back({obs[i], char('a' + i)});
    }
    sort(cipherFreq.rbegin(), cipherFreq.rend());
    vector<pair<double,char>> englishFreq;
    for (int i = 0; i < 26; i++) {
        englishFreq.push_back({freq[i], char('a' + i)});
    }
    sort(englishFreq.rbegin(), englishFreq.rend());
    // Only top 5 frequency matches
    for (int i = 0; i < 5; i++) {
        char cipherLetter = cipherFreq[i].second;
        char plainLetter = englishFreq[i].second;
        key[cipherLetter] = plainLetter;
    }
}
void one_word_analysis(string& cipher,
                       unordered_map<char, char>& key) {

    // Check one-letter words before iteration
    bool aMapped = key.find('a') != key.end();
    bool iMapped = key.find('i') != key.end();

    if (aMapped && !iMapped) {
        if (key['a'] == 'a')
            key['i'] = 'i';
        else if (key['a'] == 'i')
            key['i'] = 'a';
    }
    else if (!aMapped && iMapped) {
        if (key['i'] == 'a')
            key['a'] = 'i';
        else if (key['i'] == 'i')
            key['a'] = 'a';
    }
    else {
      cout<<"both is not mapped try and check bot 'a' and 'i' "<<endl;
    }
}


int main() {
  /*s-1 -> frequecy analysis
  s-2 word analysis 
  s-3 pattern 
  s-4 key find
  Plain : abcdefghijklmnopqrstuvwxyz
  Key   : qazwsxedcrfvtgbyhnujmikolp
  */

  // frequncy analysis
  ifstream cih("cipher.txt");
  string cipher,line;

  while(getline(cih,line)){
    cipher+=line;
  }
  unordered_map<char,char>key;

  frequency(cipher,key);
  
  for(auto &c:cipher){
    if(key.find(c)!=key.end()){
      c=key[c];
    }
  }
  cout<<"by frequency analaysis"<<endl;
  cout<<cipher<<endl;

  one_word_analysis(cipher,key);
  cout<<cipher;
    return 0;
}