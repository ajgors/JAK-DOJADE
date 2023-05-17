#pragma once

template <typename K, typename V, typename F>
class Hashmap {
private:
    const int INITIAL_CAPACITY = 16;
    const float LOAD_FACTOR = 1.0f;

    struct Entry {
        K key;
        V value;
        Entry(const K& key, const V& value) : key(key), value(value) {}
        Entry(const K& key, V&& value) : key(key), value(std::move(value)) {}
    };

    List<Entry>* m_table;
    size_t m_size = 0;
    size_t m_capacity = INITIAL_CAPACITY;

    size_t getBucketIndex(const K& key) const {
        size_t hashCode = m_hashingFunction(key);
        hashCode %= m_capacity;
        return hashCode;
    }

    bool shouldRehash() const {
        float loadFactor = (float)m_size / m_capacity;
        return loadFactor > LOAD_FACTOR;
    }

    void rehash() {
        size_t newCapacity = m_capacity * 2;
        List<Entry>* newTable = new List<Entry>[newCapacity];

        for (size_t i = 0; i < m_capacity; i++) {
            for (auto&& entry : m_table[i]) {
                size_t newIndex = m_hashingFunction(entry.key) % newCapacity;
                newTable[newIndex].pushBack((Entry&&)entry);
            }
        }
        delete[] m_table;
        m_capacity = newCapacity;
        m_table = newTable;
    }

    F m_hashingFunction;


public:

    Hashmap() {
        m_table = new List<Entry>[INITIAL_CAPACITY];
    }

    Hashmap(size_t capacity) : m_capacity(capacity) {
        m_table = new List<Entry>[capacity];
    }

    ~Hashmap() {
        delete[] m_table;
    }

    Hashmap(const Hashmap& other) {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_table = new List<Entry>[m_capacity];
        for (size_t i = 0; i < m_capacity; i++) {
            m_table[i] = other.m_table[i];
        }
    }

    Hashmap(Hashmap&& other) {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_table = other.m_table;
        other.m_table = nullptr;
    }

    void put(const K& key, const V& value) {

        if (shouldRehash()) {
            rehash();
        }
        
        Entry entry(key, value);
        size_t bucketIndex = getBucketIndex(key);
        List<Entry>& bucket = m_table[bucketIndex];

        for (auto& e : bucket) {
            if (e.key == key) {
                e.value = value;
                return;
            }
        }

        bucket.pushBack(entry);
        ++m_size;
    }

    V& operator[](const K& key) {

        if (shouldRehash()) {
            rehash();
        }

        size_t bucketIndex = getBucketIndex(key);
        List<Entry>& bucket = m_table[bucketIndex];

        for (auto& e : bucket) {
            if (e.key == key) {
                return e.value;
            }
        }

        bucket.pushBack({ key, V() });
        ++m_size;

        return bucket.back().data.value;
    }

    void clear() {
        for (size_t i = 0; i < m_capacity; i++) {
            m_table[i].clear();
        }
        m_size = 0;
    }

    size_t size() const {
        return m_size;
    }

    List<Entry>* getTable() {
        return m_table;
    }
    size_t getCapacity() {
        return m_capacity;
    }
};