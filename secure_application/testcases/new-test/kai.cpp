#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>

using namespace std;

// Comparator: higher frequency gets higher priority
struct cmp {
    bool operator()(const pair<int,int>& a,
                    const pair<int,int>& b) const {
        return a.second < b.second;
    }
};

void patterns(unordered_map<string, vector<int>>& mp, string& cipher) {

    for(int n = 3; n <= 5; n++) {

        for(int i = 0; i <= cipher.size() - n; i++) {

            string par = cipher.substr(i, n);

            mp[par].push_back(i);
        }
    }
}

void distance_factor(unordered_map<int,int>& fact,
                     unordered_map<string, vector<int>>& mp) {

    for(auto p : mp) {

        if(p.second.size() > 1) {

            vector<int> dis;

            // Calculate distances for this pattern
            for(int i = 1; i < p.second.size(); i++) {
                dis.push_back(p.second[i] - p.second[i - 1]);
            }

            // Calculate factors
            for(int d : dis) {

                for(int k = 2; k < d; k++) {

                    if(d % k == 0) {
                        fact[k]++;
                    }
                }
            }
        }
    }
}

int main() {

    string cipher;
    string line;

    ifstream cip("kai_cipher.txt");

    while(getline(cip, line)) {

        for(char c : line) {

            if(isalpha(c)) {
                cipher += tolower(c);
            }
        }
    }

    unordered_map<string, vector<int>> mp;
    unordered_map<int, int> fact;

    // Find 3, 4 and 5 letter patterns
    patterns(mp, cipher);

    // Calculate distances and factor frequencies
    distance_factor(fact, mp);

    // Priority queue: highest frequency first
    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        cmp
    > pq;

    // Put factor -> frequency into priority queue
    for(auto p : fact) {
        pq.push(p);
    }

    // Display all factors
    cout << "Factors:\n";

    while(!pq.empty()) {
        cout << pq.top().first << " -> " << pq.top().second << endl;
        pq.pop();
    }

    // Most probable key length
    if(!pq.empty()) {
        cout << "\nMost probable key length: "
             << pq.top().first << endl;

        cout << "Frequency: "
             << pq.top().second << endl;
    }

    return 0;
}