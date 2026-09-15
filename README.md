# CryptoLabX — Group 04

A modular C++ cryptography and cryptanalysis laboratory project implementing classical cipher algorithms and their cryptanalysis techniques.

## 👥 Team

* **Prakash Kumar**
* **Mithilesh**

## 📌 Project Overview

CryptoLabX is a hands-on cryptography project focused on understanding, implementing, and analyzing classical cryptographic algorithms.

The project covers both **encryption/decryption** and **cryptanalysis**, with emphasis on understanding how classical ciphers can be attacked using mathematical and statistical techniques.

## 🔐 Implemented Topics

### 1. Shift Cipher

* Encryption and decryption
* Brute-force cryptanalysis
* Dictionary-based scoring
* Chi-Square analysis

### 2. Monoalphabetic Substitution Cipher

* Encryption and decryption
* Frequency analysis
* English letter frequency comparison
* Key recovery techniques

### 3. Vigenère Cipher

* Encryption and decryption
* Kasiski Examination
* Key-length estimation
* Frequency analysis
* Key recovery
* Ciphertext decryption using the recovered key

### 4. Kasiski Examination

The Vigenère cryptanalysis module estimates the probable key length using repeated ciphertext patterns.

The process includes:

1. Preprocessing the ciphertext
2. Finding repeated patterns of length 3–5
3. Recording their positions
4. Calculating distances between repeated occurrences
5. Finding factors of the distances
6. Combining factor frequencies
7. Selecting probable key-length candidates
8. Dividing the ciphertext into groups according to the estimated key length
9. Performing frequency analysis on each group

## 🧠 Cryptanalysis Approach

The project follows the general workflow:

```text
Ciphertext
    ↓
Preprocessing
    ↓
Pattern / Frequency Analysis
    ↓
Cryptanalysis
    ↓
Key Recovery
    ↓
Decryption
    ↓
Plaintext
```

For Vigenère cipher cryptanalysis:

```text
Ciphertext
    ↓
Kasiski Examination
    ↓
Estimate Key Length
    ↓
Divide Ciphertext into Groups
    ↓
Frequency Analysis
    ↓
Recover Key
    ↓
Decrypt
```

## 📂 Project Structure

```text
CryptoLabX_Group04/
│
├── analysis/
│   └── Analysis and statistical processing modules
│
├── attacks/
│   └── Cryptanalysis implementations
│
├── datasets/
│   └── Ciphertexts and test datasets
│
├── secure_application/
│   └── Security-related application components
│
├── utils/
│   └── Helper utilities
│
├── main.py
├── requirements.txt
├── log_file.md
└── README.md
```

## 🛠️ Technologies

* **C++**
* **Python**
* Standard Template Library (STL)
* Git & GitHub

## ▶️ Compilation

For C++ programs:

```bash
g++ filename.cpp -o program
./program
```

For example:

```bash
g++ kai.cpp -o kai
./kai
```

## 📊 Kasiski Example

For repeated ciphertext patterns:

```text
Pattern → Positions → Distances → Factors
```

Example:

```text
ABC → 10, 16, 22
       ↓
     6, 6
       ↓
   2, 3, 6
```

Factor frequencies from different repeated patterns are combined to identify likely key-length candidates.

## 🎯 Learning Objectives

This project aims to develop practical understanding of:

* Classical cryptography
* Cryptanalysis
* Frequency analysis
* Statistical techniques
* Pattern analysis
* Key-length estimation
* C++ STL data structures
* Modular software organization
* Git/GitHub based collaboration

## 🚀 Future Improvements

* Improved automated key recovery
* Better statistical scoring
* Support for additional classical ciphers
* Automated plaintext scoring
* More robust Vigenère cryptanalysis
* Unit testing for cryptographic modules
* Improved documentation and examples

## 📜 Disclaimer

This project is developed for **educational and academic purposes** to understand classical cryptography and cryptanalysis techniques.
