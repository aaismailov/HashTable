#ifndef DICTIONARY_HASH_H
#define DICTIONARY_HASH_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <list>
#include <QString>

using namespace std;

template <typename K>
class dictionary_hash
{
    public:
        dictionary_hash();
        dictionary_hash(const dictionary_hash &other);
        ~dictionary_hash();
        int clear();
        int size() const;
        int input_from_file(const string &name);
        int output_to_file(const string &name) const;
        int operator==(const dictionary_hash &other);
        int operator!=(const dictionary_hash &other);
        int delete_word(const K &key);
        int in_dictionary(const K &key) const;
        void insert(const K &key, string aa);
        string get_aa(const K &key) const;
        int hash(const K &key) const;
        string operator[](const K &key){ return get_aa(key);}
        list<string> count(list<string> queue);
        const vector<vector<pair<K, string>>> get() const
        {return m_map;}
    private:
        vector<vector<pair<K, string>>> m_map = vector<vector<pair<K, string>>>(512);
};

template<typename K>
dictionary_hash<K>::dictionary_hash()
{
    for(auto &i: this->m_map)
        for(auto &j: i)
            j.second = "\0";
}


template<typename K>
dictionary_hash<K>::dictionary_hash(const dictionary_hash &other)
{

    for(auto &i: other.m_map)
        for(auto &j: i)
            this->insert(j.first, j.second);
}


template<typename K>
dictionary_hash<K>::~dictionary_hash()
{
    for(auto &i: this->m_map)
        i.clear();
}

template<typename K>
int dictionary_hash<K>::hash(const K &key) const
{
    int count = 0;
    for(const auto i: key)
        count += i;
    return count % 512;
}

template<typename K>
void dictionary_hash<K>::insert(const K &key, string aa)
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    for (auto& c: vector)
        if (c.first == key) {
            c.second = aa;
            return;
        }
        vector.push_back({key, aa});

}

template<typename K>
string dictionary_hash<K>::get_aa(const K &key) const
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    for (auto& c:vector)
        if (c.first == key)
            return c.second;
    return "\0";
}

template<typename K>
int dictionary_hash<K>::size() const
{
    int count = 0;

    for(auto &i: this->m_map)
        for(auto &j: i)
            if(j.second != "\0")
                count++;
    return (count);

}

template<typename K>
int dictionary_hash<K>::clear()
{
    for(auto &i: this->m_map)
        for(auto &j: i)
        {
            j.first = "\0";
            j.second = "\0";
        }
    return 1;
}

template<typename K>
int dictionary_hash<K>::input_from_file(const string &name)
{
    string codon;
    string aa;
    string aa2;
    string s;
    ifstream file(name);

    if(this->size() > 0)
        this->clear();
    if (file.is_open())
    {
        while(getline(file, s))
        {
            istringstream ss(s);
            ss >> codon >> aa >> aa2;
            if (aa2 != "\0") aa+=" "+aa2, aa2="\0";
            if (codon != "\0" && aa != "\0")
                this->insert(codon, aa);
        }
        file.close();
    }

    return 1;
}

template<typename K>
int dictionary_hash<K>::output_to_file(const string &name) const
{
    ofstream fout(name);
    for(auto &i: this->m_map)
        for(auto &j: i)
        {
            if (j.second != "\0" && j.first != "\0")
            {
                fout << j.first << " " << j.second << "\n";
            }
        }

    fout.close();
    return 1;
}

template<typename K>
int dictionary_hash<K>::operator==(const dictionary_hash &other)
{
    if(this->m_map != other.m_map)
        return 0;
    return 1;
}

template<typename K>
int dictionary_hash<K>::delete_word(const K &key)
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    int i = 0;
    for (auto& c:vector)
    {
        if (c.first == key)
            vector.erase(vector.begin() + i);
        i++;
    }
    return 1;
}

template<typename K>
int dictionary_hash<K>::in_dictionary(const K &key) const
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    for (auto& c:vector)
        if (c.first == key)
            return 1;
    return 0;
}

template<typename K>
list<string> dictionary_hash<K>::count(list<string> queue)
{
    list<string> vqueue;
    for (string k: queue) {
        int flag = 0;
        for(auto &i: this->m_map)
            for(auto &j: i)
                if(j.first == k)
                    vqueue.push_back(j.second), flag = 1;
        if (!flag){
            stringstream ss;
            ss << "Codon " << k << " doesn't exist";
            vqueue.push_back(ss.str());
        }
    }
    return vqueue;
}

template<typename K>
int dictionary_hash<K>::operator!=(const dictionary_hash &other)
{
    if(this->m_map == other.m_map)
        return 0;
    return 1;
}
#endif // DICTIONARY_HASH_H
