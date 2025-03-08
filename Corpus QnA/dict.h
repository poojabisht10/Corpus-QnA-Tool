// Do NOT add any other includes
#pragma once
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

class Dict {
private:
    //You can add attributes/helper functions here
    class Node1{
        public:
        string word;
        long long count;
        
        Node1();
        Node1(string w,long long c);
    };
    vector<vector<Node1*>> v;
    int hash1(string s);
    

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
    int bkcde,pge,para,kg;
    double score;
    void insert_word(string w,long long c);
    Dict(int b);
    
};