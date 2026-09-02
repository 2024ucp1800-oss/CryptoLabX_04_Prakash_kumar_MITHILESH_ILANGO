#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>

using namespace std;

int main(){
    ifstream file("ciphertext.txt");

    string text;
    string line;

    while (getline(file, line)) {
        text += line;
        text += '\n';
    }

    cout << text;
    return 0;
}