#include <assert.h>
#include <sstream>
#include "qna_tool.h"
#include <algorithm>

using namespace std;

QNA_tool::QNA_tool()
{
    // Implement your function here
    yes = head;
}

QNA_tool::~QNA_tool()
{
    // Implement your function here
    delete gd;
    delete head;
    delete yes;
}

bool mycom(Dict *d1, Dict *d2)
{
    return (d1->score < d2->score);
}
void rf(Dict *x)
{
    ifstream file("unigram_freq.csv");
    if (!file.is_open())
    {
        cout << "Error opening the file!" << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        istringstream sgg(line);
        vector<string> row;
        string ce;
        while (getline(sgg, ce, ','))
        {
            row.push_back(ce);
        }
        long long temp_count = stoll(row[1]);
        x->insert_word(row[0], temp_count);
    }
    file.close();
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here
    gd->Dict::insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    if (yes->mem == NULL)
    {
        Dict *t = new Dict(0);
        t->Dict::insert_sentence(book_code, page, paragraph, sentence_no, sentence);
        head->mem = t;
        head->next = new Nigr();
        yes = head;
    }
    else
    {
        if (yes->mem->para == paragraph)
        {
            yes->mem->Dict::insert_sentence(book_code, page, paragraph, sentence_no, sentence);
        }
        else
        {
            Dict *t = new Dict(0);
            t->Dict::insert_sentence(book_code, page, paragraph, sentence_no, sentence);
            yes->next = new Nigr();
            yes = yes->next;
            yes->mem = t;
        }
    }
}

Node *QNA_tool::get_top_k_para(string question, int k)
{
    // Implement your function here
    vector<string> words;
    Node *head1 = new Node();
    // return head1;
    char arr[] = {'@', ',', '-', '.', ':', '!', '\"', '(', ')', '?', '[', ']', ';', '\''};
    string s = "";
    for (int i = 0; i < question.size(); i++)
    {
        char c = question[i];
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
            if (c < 32)
            {
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
                words.push_back(s);
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
    }
    else
    {
        words.push_back(s);
    }
    vector<string> cwdc={"i", "me", "my", "myself", "we", "our", "ours", "ourselves", 
        "you", "your", "yours", "yourself", "yourselves", 
        "he", "him", "his", "himself", "she", "her", "hers", "herself", 
        "it", "its", "itself", "they", "them", "their", "theirs", "themselves", 
        "what", "which", "who", "whom", "this", "that", "these", "those", 
        "am", "is", "are", "was", "were", "be", "been", "being", 
        "have", "has", "had", "having", "do", "does", "did", "doing", 
        "a", "an", "the", "and", "but", "if", "or", "because", 
        "as", "until", "while", "of", "at", "by", "for", "with", 
        "about", "against", "between", "into", "through", "during", "before", "after", 
        "above", "below", "to", "from", "up", "down", "in", "out", 
        "on", "off", "over", "under", "again", "further", "then", "once", 
        "here", "there", "when", "where", "why", "how", "all", "any", 
        "both", "each", "few", "more", "most", "other", "some", "such", 
        "no", "nor", "not", "only", "own", "same", "so", "than", 
        "too", "very", "s", "t", "can", "will", "just", "don", 
        "should", "now", "d", "ll", "m", "o", "re", "ve", "y", 
        "ain", "aren", "couldn", "didn", "doesn", "hadn", "hasn", "haven", 
        "isn", "ma", "mightn", "mustn", "needn", "shan", "shouldn", "wasn", 
        "weren", "won", "hadn't", "hadn't", "haven't", "isn't", "ma'am", 
        "mightn't", "mustn't", "needn't", "shan't", "shouldn't", "wasn't", 
        "weren't", "won't", "wouldn't"};
    for(int a=0;a<cwdc.size();a++){
        for(int b=0;b<words.size();b++){
            if(words[b]==cwdc[a]){
                words.erase(words.begin()+b);
            }
        }
    }

    Dict *bb = new Dict();
    rf(bb);
    vector<double> bp;
    for (int i = 0; i < words.size(); i++)
    {
        bp.push_back((gd->get_word_count(words[i]) + 1.0) / (bb->get_word_count(words[i]) + 1.0));
    }
    vector<Dict *> kth;
    int f = 0;
    Nigr *tt = head;
    while (tt->next != NULL)
    {
        for (int j = 0; j < words.size(); j++)
        {
            tt->mem->score += (static_cast<double>(tt->mem->get_word_count(words[j])) * bp[j]);
        }
        // if (kth.size() == 0)
        // {
        //     kth.push_back(tt->mem);
        //     f++;
        // }
        // else
        // {
        //     bool fg = true;
        //     for (int j = 0; j < kth.size(); j++)
        //     {
        //         if (kth[j]->score <= tt->mem->score)
        //         {
        //             kth.insert(kth.begin() + j, tt->mem);
        //             f++;
        //             fg = false;
        //             break;
        //         }
        //     }
        //     if (fg && f == k)
        //     {
        //         continue;
        //     }
        //     else if (fg)
        //     {
        //         kth.push_back(tt->mem);
        //         f++;
        //     }
        //     else if (f <= k)
        //     {
        //         continue;
        //     }
        //     else
        //     {
        //         kth.pop_back();
        //         f--;
        //     }
        // }
        // tt = tt->next;
        kth.push_back(tt->mem);
        tt = tt->next;
    }
    for (int j = 0; j < words.size(); j++){
        tt->mem->score += (static_cast<double>(tt->mem->get_word_count(words[j])) * bp[j]);
    }
    kth.push_back(tt->mem);
    sort(kth.rbegin(), kth.rend(), mycom);
    head1->book_code = kth[0]->bkcde;
    head1->page = kth[0]->pge;
    head1->paragraph = kth[0]->para;
    Node *temp = head1;
    for (int i = 1; i < k; i++)
    {
        temp->right = new Node(kth[i]->bkcde, kth[i]->pge, kth[i]->para, 0, 0);
        temp = temp->right;
    }
    // cout<<f<<endl;
    temp->right = NULL;
    delete bb;
    return head1;
}

void QNA_tool::query(string question, string filename)
{
    // Implement your function here
    Node *t = get_top_k_para(question, 10);
    query_llm(filename, t, 10, "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", question);
    // std::cout << "Q: " << question << std::endl;
    // std::cout << "A: " << "Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))
        {
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{

    // first write the k paragraphs into different files

    Node *traverse = root;
    int num_paragraph = 0;

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}