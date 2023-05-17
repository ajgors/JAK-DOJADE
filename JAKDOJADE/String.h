#pragma once
#include <iostream>

class String {

private:
    size_t m_length;
    char* m_string;
    size_t m_capacity;
    void resize();

public:

    String();
    ~String();
    String(const char* string);
    String(const String& other);
    String(String&& other);
    String(size_t size);

    char&               operator[](size_t index);
    String&             operator=(const String& string);
    String&             operator=(const char* string);
    bool                operator==(String& other);
    bool                operator==(const String& other) const;
    bool                operator==(const char* other);
    bool                operator<(const String& other) const;
    bool                operator!=(const char* other);
    bool                operator!=(const String& other);
    String&             append(const char* to_append);
    String&             operator+(const char* to_append);
    String&             operator+=(const char* to_append);
    String&             operator+=(char to_append);

    size_t              length() const;
    size_t              length();
    void                popBack();
    void                print();
    String&             append(char to_append);
    int                 find(const char* str);
    bool                contains(const char* substring);
    void                toUpperCase();
    void                toLowerCase();
    //from index to end
    String              slice(size_t index);
    //from 0 to index
    String              cut(size_t index);
    String              substr(size_t start, size_t end);
    char                at(size_t index);
    char*               c_str();
    char*               c_str() const;
    void                clear();
    int                 countChar(char c);
    void                trim();

    friend std::ostream& operator<<(std::ostream& os, const String& string);
    friend std::istream& operator>>(std::istream& is, String& out);
};


struct hashString {
    size_t operator()(const String& k) const {
        size_t hash = 5381;
        int c;
        char* cc = k.c_str();
        while ((c = *cc++))
            hash = ((hash << 5) + hash) + c;

        return hash;
    }
};