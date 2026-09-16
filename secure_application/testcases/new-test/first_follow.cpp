#include <iostream>
#include <map>
#include <set>
#include <string>
using namespace std;

map<char,string> g;
map<char,set<char>> first, follow;

set<char> FIRST(char c)
{
    if (!isupper(c))
        return {c};

    if (!first[c].empty())
        return first[c];

    for(char x : g[c])
    {
        if(x == '#')
            first[c].insert('#');
        else
        {
            set<char> t = FIRST(x);
            for(char a : t)
                first[c].insert(a);
        }
    }
    return first[c];
}

int main()
{
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (use # for epsilon):\n";

    for(int i=0; i<n; i++)
    {
        string s;
        cin >> s;
        g[s[0]] = s.substr(2);
    }

    // FIRST
    for(auto x : g)
        FIRST(x.first);

    // FOLLOW of start symbol
    follow[g.begin()->first].insert('$');

    // FOLLOW
    for(auto rule : g)
    {
        char A = rule.first;
        string s = rule.second;

        for(int i=0; i<s.length(); i++)
        {
            if(isupper(s[i]))
            {
                if(i+1 < s.length())
                {
                    set<char> t = FIRST(s[i+1]);

                    for(char x : t)
                        if(x != '#')
                            follow[s[i]].insert(x);
                }
                else
                {
                    for(char x : follow[A])
                        follow[s[i]].insert(x);
                }
            }
        }
    }

    cout << "\nFIRST:\n";
    for(auto x : first)
    {
        cout << x.first << " = { ";
        for(char c : x.second)
            cout << c << " ";
        cout << "}\n";
    }

    cout << "\nFOLLOW:\n";
    for(auto x : follow)
    {
        cout << x.first << " = { ";
        for(char c : x.second)
            cout << c << " ";
        cout << "}\n";
    }

    return 0;
}
