// Двойное хеширование

#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

using std::string;
using std::vector;

namespace NodeStatus {
    int data = 0;
    int del = 1;
    int empty = 2;
};

struct Hasher1 {
    unsigned int operator()( const string& str ) const {
        unsigned int hash = 0;
        unsigned int hash_coefficient = 7;
        for( unsigned int i = 0; i < str.size(); ++i )
            hash = hash * hash_coefficient + str[i];
        return hash;
    }
};

struct Hasher2 {
    unsigned int operator()(const string& str) const {
        unsigned int hash = 0;
        unsigned int hash_coefficient = 17;
        for( unsigned int i = 0; i < str.size(); ++i) {
            hash = hash * hash_coefficient + str[i];
        }
        return hash;
    }
};


template<class T, class H1, class H2>
class HashTable {
public:
    explicit HashTable( const H1& _hasher1, const H2& _hasher2);
    HashTable( const HashTable& table ) = delete;
    HashTable& operator=( const HashTable& table ) = delete;
    ~HashTable() = default;

    bool Add(const T& data);
    bool Has(const T& data) const;
    bool Delete(const T& data);

private:
    H1 mHasher;
    H2 mExtraHasher;
    struct HashTableNode {
        T data;
        int status = NodeStatus::empty;
    };
    vector<HashTableNode> table;
    unsigned int keysCount;
    void growTable();
};

template<class T, class H1, class H2>
HashTable<T, H1, H2>::HashTable( const H1& _hasher1, const H2& _hasher2) :
        mHasher(_hasher1), mExtraHasher(_hasher2), table(8), keysCount(0) {}


template<class T, class H1, class H2>
void HashTable<T, H1, H2>::growTable() {
    vector<HashTableNode> newTable(2 * table.size());
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].status == NodeStatus::del || table[i].status == NodeStatus::empty) {  // очистка от ненужных данных (удаленных)
            continue;
        }

        HashTableNode tmp = table[i];
        unsigned int newHash = mHasher(tmp.data);
        unsigned int step = (mExtraHasher(tmp.data) * 2) + 1;

        for (size_t j = 0; j < newTable.size(); ++j) {
            if (newTable[(newHash + j * step) % newTable.size()].status == NodeStatus::empty) {  //  вставка в новую таблицу
                newTable[(newHash + j * step) % newTable.size()].status = NodeStatus::data;
                newTable[(newHash + j * step) % newTable.size()].data = tmp.data;
                break;
            }
        }
    }
    table = std::move(newTable);
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Add(const T& data) {
    if (keysCount > table.size() * 3 / 4) {  // проверка на заполненность
        growTable();
    }
    unsigned int first_hash = mHasher(data);
    unsigned int step = (mExtraHasher(data) * 2) + 1;
    int free_spot = -1;
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::del && free_spot < 0) {
            free_spot = (first_hash + i * step) % table.size();
            continue;
        }
        
        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::data && data == table[(first_hash + i * step)% table.size()].data) {
            return false;
        } 

        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::empty) {
            if (free_spot >= 0) {
                table[free_spot].status = NodeStatus::data;
                table[free_spot].data = data;
            } else {
                table[(first_hash + i * step) % table.size()].status = NodeStatus::data;
                table[(first_hash + i * step) % table.size()].data = data;
            }
            keysCount++;
            return true;
        }
    }
    if (free_spot >= 0) {  //  если нет empty, но есть del
        table[free_spot].status = NodeStatus::data;
        table[free_spot].data = data;
        keysCount++;
        return true;
    }
    return false;
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Delete(const T& data) {
    unsigned int first_hash = mHasher(data);
    unsigned int step = (mExtraHasher(data) * 2) + 1;
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::del) {
            continue;
        }

        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::data && data == table[(first_hash + i * step)% table.size()].data) {
            table[(first_hash + i * step) % table.size()].status = NodeStatus::del;
            keysCount--;
            return true;
        }

        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::empty) {
            return false;
        }
    }
    return false;
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Has(const T& data) const {
    unsigned int first_hash = mHasher(data);
    unsigned int step = (mExtraHasher(data) * 2) + 1;
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::del) {
            continue;
        }

        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::data && data == table[(first_hash + i * step)% table.size()].data) {
            return true;
        }

        if (table[(first_hash + i * step) % table.size()].status == NodeStatus::empty) {
            return false;
        }
    }
    return false;
}

int main() {
    Hasher1 sh;
    Hasher2 exh;
    HashTable<string, Hasher1, Hasher2> table(sh, exh);
    char operation = 0;
    string data;
    while( std::cin >> operation >> data ) {
        switch( operation ) {
            case '+':
                std::cout << (table.Add( data ) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Delete( data ) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has( data ) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
        }
    }

    return 0;
}
