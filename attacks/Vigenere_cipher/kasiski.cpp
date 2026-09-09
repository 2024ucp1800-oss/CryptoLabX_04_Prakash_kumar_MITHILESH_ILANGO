#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <cctype>
#include <unordered_map>

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
    for(int i = 0; i+n <= final.size(); i++) {
        string seq = final.substr(i,n);
        mp[seq].push_back(i);
    }
    return mp;
}

vector<int> calculate_distances(unordered_map<string,vector<int>>&mp) {
    vector<int>distance;
    for(auto p:mp) {
        if(p.second.size()>1) {
            vector<int>check=p.second;
            for(int i=1;i<check.size();i++) {
                distance.push_back(check[i]-check[i-1]);
            }
        }
    }
    return distance;
}

map<int,int> find_factors(vector<int>& distances) {
    map<int,int> factor_count;
    for(int d:distances) {
        for(int i=2;i<=d;i++) {
            if(d%i==0) {
                factor_count[i]++;
            }
        }
    }
    return factor_count;
}

vector<int> kasiski(string &final) {
    map<int,int> total_factors;
    for(int n=3;n<=5;n++) {
        auto patterns=find_repeated_patterns(n,final);
        vector<int>distances=calculate_distances(patterns);
        map<int,int>factors=find_factors(distances);
        for(auto &p:factors) {
            total_factors[p.first]+=p.second;
        }
    }
    vector<pair<int,int>>candidates;
    for(auto &p:total_factors) {
        if(p.first<=30) {
            candidates.push_back({p.first,p.second});
        }
    }
    sort(candidates.begin(),candidates.end(),[](auto &a,auto &b) {
        return a.second>b.second;
    });
    cout<<"Top 5 Candidate Key Lengths:\n";
    vector<int>top;
    for(int i=0;i<5&&i<candidates.size();i++) {
        cout<<"Length: "<<candidates[i].first<<" Score: "<<candidates[i].second<<endl;
        top.push_back(candidates[i].first);
    }
    return top;
}

double calculate_ic(string &text) {
    int freq[26]={0};
    for(char c:text) {
        freq[c-'A']++;
    }
    int N=text.size();
    if(N<=1) {
        return 0.0;
    }
    int numerator=0;
    for(int i=0;i<26;i++) {
        numerator+=freq[i]*(freq[i]-1);
    }
    return(double)numerator/(N*(N-1));
}

vector<string> split_into_groups(string &final,int keyLength) {
    vector<string>groups(keyLength);
    for(int i=0;i<final.size();i++) {
        groups[i%keyLength]+=final[i];
    }
    return groups;
}

double average_ic(string &final,int keyLength) {
    vector<string>groups=split_into_groups(final,keyLength);
    double total=0;
    for(auto &group:groups) {
        total+=calculate_ic(group);
    }
    return total/keyLength;
}

vector<int> frequency_analysis(string &group) {
    vector<int>freq(26,0);
    for(char c:group) {
        freq[c-'A']++;
    }
    return freq;
}

int find_shift(string &group) {
    vector<int>freq=frequency_analysis(group);
    int total=group.size();
    double english[26]={
        0.082,0.015,0.028,0.043,0.127,0.022,0.020,
        0.061,0.070,0.0015,0.0077,0.040,0.024,0.067,
        0.075,0.019,0.00095,0.060,0.063,0.091,0.028,
        0.0098,0.024,0.0015,0.020,0.00074
    };
    double best_score=1e9;
    int best_shift=0;
    for(int shift=0;shift<26;shift++) {
        double score=0;
        for(int i=0;i<26;i++) {
            int observed=freq[(i+shift)%26];
            double expected=english[i]*total;
            if(expected>0) {
                score+=(observed-expected)*(observed-expected)/expected;
            }
        }
        if(score<best_score) {
            best_score=score;
            best_shift=shift;
        }
    }
    return best_shift;
}

string find_key(string &final,int keyLength) {
    vector<string>groups=split_into_groups(final,keyLength);
    string key="";
    for(auto &group:groups) {
        int shift=find_shift(group);
        key+=char('A'+shift);
    }
    return key;
}

string vigenere_decrypt(string &final,string &key) {
    string plaintext="";
    for(int i=0;i<final.size();i++) {
        int c=final[i]-'A';
        int k=key[i%key.size()]-'A';
        plaintext+=char((c-k+26)%26+'A');
    }
    return plaintext;
}

string vigenere_encrypt(string &plaintext,string &key) {
    string ciphertext="";
    for(int i=0;i<plaintext.size();i++) {
        int p=plaintext[i]-'A';
        int k=key[i%key.size()]-'A';
        ciphertext+=char((p+k)%26+'A');
    }
    return ciphertext;
}

bool verify(string &final,string &ciphertext) {
    return final==ciphertext;
}

int main() {
    ifstream file("ciphertext.txt");
    string ciphertext;
    string line;
    while(getline(file,line)) {
        ciphertext+=line;
    }
    file.close();

    string final=clean_ciphertext(ciphertext);

    vector<int>top=kasiski(final);

    cout<<"\nIC Analysis:\n";
    vector<pair<int,double>>ic_candidates;
    for(int keyLength:top) {
        double ic=average_ic(final,keyLength);
        ic_candidates.push_back({keyLength,ic});
        cout<<"Length: "<<keyLength<<" IC: "<<ic<<endl;
    }

    sort(ic_candidates.begin(),ic_candidates.end(),[](auto &a,auto &b) {
        return a.second>b.second;
    });

    int keyLength=ic_candidates[0].first;
    string key=find_key(final,keyLength);
    cout<<"\nRecovered Key: "<<key<<endl;
    string plaintext=vigenere_decrypt(final,key);
    cout<<"Plaintext:\n"<<plaintext<<endl;
    string encrypted=vigenere_encrypt(plaintext,key);
    cout<<"Verification: "<<(verify(final,encrypted)?"SUCCESS":"FAILED")<<endl;
    
    return 0;
}