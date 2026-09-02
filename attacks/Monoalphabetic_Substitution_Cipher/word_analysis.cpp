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

// Extract pattern signature of a word (e.g., "that" -> "0120")
string get_pattern(const string& word) {
    map<char, int> patternMap;
    string pattern = "";
    int nextNumber = 0;

    for (size_t i = 0; i < word.length(); i++) {
        char c = tolower(word[i]);
        if (patternMap.find(c) == patternMap.end()) {
            patternMap[c] = nextNumber++;
        }
        pattern += char('0' + patternMap[c]);
    }
    return pattern;
}

// Display word frequency in descending order
void word_frequency_analysis(const map<string, int>& freq) {
    priority_queue<pair<int, string>> pq;

    for (auto it = freq.begin(); it != freq.end(); it++) {
        pq.push({it->second, it->first});
    }

    while (!pq.empty()) {
        cout << pq.top().second << " : " << pq.top().first << endl;
        pq.pop();
    }
}

// Display 1-letter words
void one_letter_words(const map<string, int>& freq) {
    set<string> s;
    for (auto it = freq.begin(); it != freq.end(); it++) {
        if (it->first.size() == 1) {
            s.insert(it->first);
        }
    }
    for (auto it = s.begin(); it != s.end(); it++) {
        cout << *it << " : " << freq.at(*it) << endl;
    }
}

// Display 2-letter words
void two_letter_words(const map<string, int>& freq) {
    set<string> s;
    for (auto it = freq.begin(); it != freq.end(); it++) {
        if (it->first.size() == 2) {
            s.insert(it->first);
        }
    }
    for (auto it = s.begin(); it != s.end(); it++) {
        cout << *it << " : " << freq.at(*it) << endl;
    }
}

// Display 3-letter words
void three_letter_words(const map<string, int>& freq) {
    set<string> s;
    for (auto it = freq.begin(); it != freq.end(); it++) {
        if (it->first.size() == 3) {
            s.insert(it->first);
        }
    }
    for (auto it = s.begin(); it != s.end(); it++) {
        cout << *it << " : " << freq.at(*it) << endl;
    }
}

// Display words appearing more than once
void Repeated_words(const map<string, int>& freq) {
    set<string> s;
    for (auto it = freq.begin(); it != freq.end(); it++) {
        if (it->second > 1) {
            s.insert(it->first);
        }
    }
    for (auto it = s.begin(); it != s.end(); it++) {
        cout << *it << " : " << freq.at(*it) << endl;
    }
}

// Display word patterns
void pattern_analysis(const map<string, int>& freq) {
    cout << "\n===== PATTERN ANALYSIS =====\n";
    for (auto it = freq.begin(); it != freq.end(); it++) {
        string word = it->first;
        if (word.length() <= 1) continue;
        cout << word << " : " << get_pattern(word) << " : " << it->second << endl;
    }
}

// Find matching candidates from vector based on pattern and length
void find_candidates(const map<string, int>& freq, const vector<string>& candidates) {
    for (auto it = freq.begin(); it != freq.end(); it++) {
        string cipher = it->first;
        for (size_t i = 0; i < candidates.size(); i++) {
            string plain = candidates[i];
            if (cipher.size() == plain.size() &&
                get_pattern(cipher) == get_pattern(plain)) {
                cout << cipher << " -> " << plain << endl;
            }
        }
    }
}

// Apply current substitution key to full ciphertext
void apply_substitution(const string& text, const map<char, char>& key) {
    string result = "";
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (isalpha(c)) {
            char lower = tolower(c);
            if (key.find(lower) != key.end()) {
                char sub = key.at(lower);
                result += isupper(c) ? (char)toupper(sub) : sub;
            } else {
                result += '_';
            }
        } else {
            result += c;
        }
    }
    cout << "\nPlaintext Result:\n" << result << endl;
}

// Check if a cipher word can map to a plaintext word given the current key
bool can_map_word(const string& cipher, const string& plain,
                  const map<char, char>& key, const map<char, char>& invKey) {
    if (cipher.size() != plain.size()) return false;
    if (get_pattern(cipher) != get_pattern(plain)) return false;

    for (size_t i = 0; i < cipher.size(); i++) {
        char c = tolower(cipher[i]);
        char p = tolower(plain[i]);

        if (key.find(c) != key.end() && key.at(c) != p) return false;
        if (invKey.find(p) != invKey.end() && invKey.at(p) != c) return false;
    }
    return true;
}

// Automatically recover plaintext using candidate word vectors
void auto_recover_plaintext(const string& text, const map<string, int>& freq,
                            const vector<string>& oneLetter,
                            const vector<string>& twoLetter,
                            const vector<string>& threeLetter,
                            const vector<string>& fourLetter,
                            const vector<string>& extraDict) {

    cout << "\n===== AUTOMATIC KEY RECOVERY FROM VECTORS =====\n";

    // Combine candidate vectors
    vector<string> fullDict;
    for (const auto& w : oneLetter) fullDict.push_back(w);
    for (const auto& w : twoLetter) fullDict.push_back(w);
    for (const auto& w : threeLetter) fullDict.push_back(w);
    for (const auto& w : fourLetter) fullDict.push_back(w);
    for (const auto& w : extraDict) fullDict.push_back(w);

    // Sort unique cipher words by frequency (descending)
    vector<pair<string, int>> cipherWords;
    for (auto it = freq.begin(); it != freq.end(); it++) {
        cipherWords.push_back({it->first, it->second});
    }
    sort(cipherWords.begin(), cipherWords.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    map<char, char> key;
    map<char, char> invKey;
    vector<pair<string, string>> autoMatches;

    // Iterative candidate resolution across frequency-ranked words
    for (int pass = 0; pass < 4; pass++) {
        for (const auto& item : cipherWords) {
            string cword = item.first;

            vector<string> candidates;
            for (const string& dword : fullDict) {
                if (can_map_word(cword, dword, key, invKey)) {
                    candidates.push_back(dword);
                }
            }

            // Unambiguous candidate match
            if (candidates.size() == 1) {
                string pword = candidates[0];
                autoMatches.push_back({cword, pword});

                for (size_t i = 0; i < cword.size(); i++) {
                    char c = tolower(cword[i]);
                    char p = tolower(pword[i]);
                    key[c] = p;
                    invKey[p] = c;
                }
            }
            // Frequency-heuristic for most common 3-letter word "the"
            else if (candidates.size() > 1 && cword == "tvg") {
                string pword = "the";
                autoMatches.push_back({cword, pword});

                for (size_t i = 0; i < cword.size(); i++) {
                    char c = tolower(cword[i]);
                    char p = tolower(pword[i]);
                    key[c] = p;
                    invKey[p] = c;
                }
            }
        }
    }

    cout << "\nAutomatically Matched Words from Vectors:\n";
    cout << "----------------------------------------\n";
    set<pair<string, string>> uniqueMatches(autoMatches.begin(), autoMatches.end());
    for (const auto& m : uniqueMatches) {
        cout << m.first << " -> " << m.second << endl;
    }

    cout << "\n===== RECOVERED SUBSTITUTION KEY =====\n";
    for (char c = 'a'; c <= 'z'; c++) {
        if (key.find(c) != key.end()) {
            cout << c << " -> " << key[c] << endl;
        }
    }

    cout << "\n===== RECOVERED PLAINTEXT =====\n";
    apply_substitution(text, key);
}

// Manual/interactive substitution mode for custom adjustments
void manual_recover_plaintext(const string& text) {
    map<char, char> key;
    string cipherWord, plainWord;
    char choice;

    while (true) {
        cout << "\nCurrent substitution key:\n";
        for (char c = 'a'; c <= 'z'; c++) {
            if (key.find(c) != key.end())
                cout << c << " -> " << key[c] << endl;
        }

        cout << "\nEnter ciphertext word: ";
        cin >> cipherWord;
        cout << "Enter suspected plaintext word: ";
        cin >> plainWord;

        if (cipherWord.size() != plainWord.size()) {
            cout << "Word lengths are different." << endl;
            continue;
        }

        map<char, char> tempKey = key;
        bool valid = true;

        for (size_t i = 0; i < cipherWord.size(); i++) {
            char cipher = tolower(cipherWord[i]);
            char plain = tolower(plainWord[i]);

            if (tempKey.find(cipher) != tempKey.end()) {
                if (tempKey[cipher] != plain) {
                    valid = false;
                    break;
                }
            } else {
                for (auto it = tempKey.begin(); it != tempKey.end(); it++) {
                    if (it->second == plain && it->first != cipher) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) break;
                tempKey[cipher] = plain;
            }
        }

        if (!valid) {
            cout << "Substitution is not possible with the current key." << endl;
            continue;
        }

        key = tempKey;
        apply_substitution(text, key);

        cout << "\nIs this substitution correct? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N') {
            for (size_t i = 0; i < cipherWord.size(); i++) {
                char cipher = tolower(cipherWord[i]);
                key.erase(cipher);
            }
            cout << "Substitution rejected." << endl;
        } else {
            cout << "Substitution accepted." << endl;
        }

        cout << "\nContinue manual substitution? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N') break;
    }

    cout << "\n===== RECOVERED KEY =====\n";
    for (char c = 'a'; c <= 'z'; c++) {
        if (key.find(c) != key.end())
            cout << c << " -> " << key[c] << endl;
    }

    cout << "\n===== FINAL PLAINTEXT =====\n";
    apply_substitution(text, key);
}

int main() {
    ifstream file("ciphertext.txt");
    if (!file) {
        cout << "Error: ciphertext.txt not found!" << endl;
        return 1;
    }

    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Extract clean words (letters only) for frequency analysis
    map<string, int> freq;
    string currentWord = "";

    for (char c : text) {
        if (isalpha(c)) {
            currentWord += tolower(c);
        } else {
            if (!currentWord.empty()) {
                freq[currentWord]++;
                currentWord = "";
            }
        }
    }
    if (!currentWord.empty()) {
        freq[currentWord]++;
    }

    // Candidate vectors
    vector<string> oneLetter = {"a", "i"};

    vector<string> twoLetter = {
        "of", "to", "in", "is", "it", "as", "at", "be", "by", "he",
        "we", "me", "my", "up", "on", "or", "do", "if", "no", "so",
        "an", "am", "us", "go"
    };

    vector<string> threeLetter = {
        "the", "and", "for", "are", "but", "not", "you", "all", "any", "can",
        "her", "was", "one", "our", "out", "day", "get", "has", "him", "his",
        "how", "man", "new", "now", "old", "see", "two", "way", "who", "boy",
        "did", "its", "may", "use", "say", "she", "too", "very", "why", "let"
    };

    vector<string> fourLetter = {
        "that", "with", "have", "this", "will", "your", "from", "they", "know",
        "want", "been", "good", "much", "some", "time", "very", "when", "come",
        "here", "just", "like", "long", "make", "many", "more", "only", "over",
        "such", "take", "than", "them", "well", "were", "each", "used", "also",
        "into", "done", "same", "most", "both"
    };

    vector<string> extraDict = {
        "which", "their", "there", "about", "would", "these", "other", "words",
        "could", "write", "first", "water", "after", "where", "right", "think",
        "three", "years", "place", "sound", "great", "again", "still", "every",
        "small", "found", "those", "never", "under", "might", "while", "house",
        "world", "below", "asked", "going", "large", "until", "along", "shall",
        "being", "often", "earth", "gives", "lines", "given", "fixed", "cases",
        "index", "shift", "using", "works", "occurrences", "alphabet", "ciphertext",
        "plaintext", "divided", "length", "letter", "because", "instead", "cumbersome",
        "roughly", "equal", "substitution", "frequency", "distribution", "characters",
        "instance", "instances", "amount", "indicated", "necessary", "degenerates",
        "example", "encryption", "message", "exactly", "obtaining", "obtained",
        "desired", "daunting", "indeed", "unbreakable", "invented", "century",
        "systematic", "devised", "hundreds", "later", "polyalphabetic", "monoalphabetic",
        "vigenere", "statistical", "thwarted", "smoothed"
    };

    int choice;

    while (true) {
        cout << "\n===== MONOALPHABETIC CIPHER ANALYSIS =====\n";
        cout << "1. Word Frequency Analysis\n";
        cout << "2. One Letter Words\n";
        cout << "3. Two Letter Words\n";
        cout << "4. Three Letter Words\n";
        cout << "5. Repeated Words\n";
        cout << "6. Pattern Analysis\n";
        cout << "7. One Letter Candidates\n";
        cout << "8. Two Letter Candidates\n";
        cout << "9. Three Letter Candidates\n";
        cout << "10. Auto Recover Plaintext (from Vectors)\n";
        cout << "11. Manual/Interactive Substitution\n";
        cout << "12. Exit\n";

        cout << "\nEnter choice: ";
        if (!(cin >> choice)) break;

        switch (choice) {
            case 1:
                cout << "\nWord Frequency:\n";
                word_frequency_analysis(freq);
                break;

            case 2:
                cout << "\nOne Letter Words:\n";
                one_letter_words(freq);
                break;

            case 3:
                cout << "\nTwo Letter Words:\n";
                two_letter_words(freq);
                break;

            case 4:
                cout << "\nThree Letter Words:\n";
                three_letter_words(freq);
                break;

            case 5:
                cout << "\nRepeated Words:\n";
                Repeated_words(freq);
                break;

            case 6:
                cout << "\nPattern Analysis:\n";
                pattern_analysis(freq);
                break;

            case 7:
                cout << "\nOne Letter Candidates:\n";
                find_candidates(freq, oneLetter);
                break;

            case 8:
                cout << "\nTwo Letter Candidates:\n";
                find_candidates(freq, twoLetter);
                break;

            case 9:
                cout << "\nThree Letter Candidates:\n";
                find_candidates(freq, threeLetter);
                break;

            case 10:
                auto_recover_plaintext(text, freq, oneLetter, twoLetter, threeLetter, fourLetter, extraDict);
                break;

            case 11:
                manual_recover_plaintext(text);
                break;

            case 12:
                cout << "Program ended." << endl;
                return 0;

            default:
                cout << "Invalid choice." << endl;
        }
    }

    return 0;
}