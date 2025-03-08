// Do NOT add any other includes
#include "dict.h"
Dict::Node1::Node1()
{
    word = "";
    count = 0;
}
Dict::Node1::Node1(string w, long long c)
{
    word = w;
    count = c;
}

Dict::Dict()
{
    this->bkcde=0;
    this->pge=0;
    this->para=0;
    this->score=0.0;
    kg=16411;
    for (int i = 0; i < kg; i++)
    {
        v.push_back({});
    }
}
Dict:: Dict(int b){
    this->bkcde=0;
    this->pge=0;
    this->para=0;
    this->score=0.0;
    kg=229;

    for (int i = 0; i < kg; i++)
    {
        v.push_back({});
    }
}

Dict::~Dict()
{
    for (int i = 0; i < kg; i++)
    {
        int j = 0;
        for (int j = 0; j < v[i].size(); j++)
        {
            delete v[i][j];
        }
    }
}


void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    this->bkcde=book_code;
    this->pge=page;
    this->para=paragraph;
    int x = sentence.size();
    char arr[] = {'@', ',', '-', '.', ':', '!', '\"', '(', ')', '?', '[', ']', ';', '\''};
    string s = "";
    for (int i = 0; i < x; i++)
    {
        char c = sentence[i];
        if (isupper(c))
        {
            c = tolower(c);
        }
        bool g = 0;
        if (c == ' ')
        {
            g = 1;
        }
        else
        {
            for (int j = 0; j < 14; j++)
            {
                if (arr[j] == c)
                {
                    g = 1;
                    break;
                }
            }
            if (c <= 32){
                g = 1;
                break;
            }
        }
        if (g == 1)
        {
            if (s == "")
            {
                continue;
            }
            else
            {
                // if(kg==1321){
                //     cout<<0<<endl;
                // }
                // if(kg==16411){
                //     cout<<1<<endl;
                // }
                int hv = hash1(s);
                if (v[hv].size() == 0)
                {
                    Node1 *n = new Node1(s, 1);
                    v[hv].push_back(n);
                }
                else
                {
                    bool f = 0;
                    for (int a = 0; a < v[hv].size(); a++){
                        if (v[hv][a]->word == s){
                            v[hv][a]->count += 1;
                            f = 1;
                            break;
                        }
                    }
                    if (f == 0)
                    {
                        Node1 *n = new Node1(s, 1);
                        v[hv].push_back(n);
                    }
                }
            }
            s = "";
        }
        else
        {
            s += c;
        }
    }
    if (s == "")
    {
        return;
    }
    else
    {
        int hv = hash1(s);
        if (v[hv].size() == 0)
        {
            Node1 *n = new Node1(s, 1);
            v[hv].push_back(n);
        }
        else
        {
            bool f = 0;
            for (int a = 0; a < v[hv].size(); a++)
            {
                if (v[hv][a]->word == s)
                {
                    v[hv][a]->count++;
                    f = 1;
                    break;
                }
            }
            if (f == 0)
            {
                Node1 *n = new Node1(s, 1);
                v[hv].push_back(n);
            }
        }
    }
}
void Dict::insert_word(string w,long long c){
    string s = "";
    for (int i = 0; i < w.size(); i++)
    {
        if (isupper(w[i]))
        {
            s += tolower(w[i]);
        }
        else
        {
            s += w[i];
        }
    }
    int idx=hash1(s);
    v[idx].push_back(new Node1(s,c));
}

long long Dict::get_word_count(string word)
{
    string s = "";
    for (int i = 0; i < word.size(); i++)
    {
        if (isupper(word[i]))
        {
            s += tolower(word[i]);
        }
        else
        {
            s += word[i];
        }
    }
    int hv = hash1(s);
    for (int a = 0; a < v[hv].size(); a++)
    {
        if (v[hv][a]->word == s)
        {
            return v[hv][a]->count;
        }
    }
    return 0;
}

void Dict::dump_dictionary(string filename)
{
    fstream my_output;
    my_output.open(filename, std::ios::out);
    for (int i = 0; i < kg; i++)
    {
        int num = v[i].size();
        for (int j = 0; j < num; j++)
        {
            if (v[i][j]->count > 0)
            {
                my_output << v[i][j]->word << ", " << v[i][j]->count << endl;
            }
        }
    }
    my_output.close();
    return;
}
int Dict::hash1(string s)
{
    int h = 1;
    for (int i = 0; i < s.length(); i++)
    {
        h += ((int(s[i]) * (i + 1)) % kg);
    }
    h = abs(h);
    h = h % kg;
    return h;
}
// int main(){
//     Dict* d=new Dict();
//     string s="I also pledged myself never to steal in future";
//     d->insert_sentence(0,0,0,0,s);
//     s="An Autobiography, Pt. I, Ch. VIII A12n a12n A12n An An An";
//     d->insert_sentence(0,0,0,0,s);
//     cout<<d->get_word_count("a12n")<<endl;
// }
// int main(){
//     Dict abcd=Dict();
//     abcd.insert_sentence(1,1,1,1,"but afterwards found a very valuable customer in the native of South Afric");
//     abcd.insert_sentence(1,1,1,1,"hurting a customer's who nearly lost his eye.");
//     abcd.insert_sentence(1,1,1,1,"hurting a customer who nearly lost his eye.");

//     cout<<abcd.get_word_count("customer");
// }