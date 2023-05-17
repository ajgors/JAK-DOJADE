#pragma once

template<typename T>
struct ListNode {
    T data;
    ListNode* next;
    ListNode* before;
    ListNode()
        : next(nullptr), before(nullptr)
    {
    }

    ListNode(const T& data)
        : data(data), next(nullptr), before(nullptr)
    {
    }

    ListNode(T&& data)
        : data(static_cast<T&&>(data)), next(nullptr), before(nullptr)
    {
    }
};


template<typename T>
class List {

public:
    ListNode<T>* m_head;
    ListNode<T>* m_tail;
    size_t m_size;
public:

    List()
        : m_head(nullptr), m_tail(nullptr), m_size(0)
    {
    }

    List(const List& other)
        : m_head(nullptr), m_tail(nullptr), m_size(0)
    {
        ListNode<T>* temp = other.m_head;
        while (temp != nullptr) {
            pushBack(temp->data);
            temp = temp->next;
        }
    }

	List& operator=(const List& other) {
		if (this != &other) {
			ListNode<T>* temp = other.m_head;
			while (temp != nullptr) {
				pushBack(temp->data);
				temp = temp->next;
			}
		}
		return *this;
	}

    List& operator=(List&& other) {
		if (this != &other) {
			m_head = other.m_head;
			m_tail = other.m_tail;
			m_size = other.m_size;
			other.m_head = nullptr;
			other.m_tail = nullptr;
			other.m_size = 0;
		}
		return *this;
    }


    List(List&& other) noexcept
        : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size)
    {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }

    ~List() {
        ListNode<T>* temp = m_head;
        while (temp != nullptr) {
            ListNode<T>* next = temp->next;
            delete temp;
            temp = next;
        }
        m_head = nullptr;
        m_tail = nullptr;
    }

    ListNode<T>& back() {
        return *m_tail;
    }

    void clear() {
        ListNode<T>* temp = m_head;
        while (temp != nullptr) {
            ListNode<T>* next = temp->next;
            delete temp;
            temp = next;
        }
        m_head = nullptr;
        m_tail = nullptr;
        m_size = 0;
    }

    void pushBack(const T& data) {
        ListNode<T>* temp = new ListNode<T>(data);
        if (m_head == nullptr) {
            m_head = temp;
            m_tail = temp;
        }
        else {
            m_tail->next = temp;
            temp->before = m_tail;
            m_tail = temp;
        }
        m_size++;
    }

    void pushBack(T&& data) {
        ListNode<T>* temp = new ListNode<T>(static_cast<T&&>(data));
        if (m_head == nullptr) {
            m_head = temp;
            m_tail = temp;
        }
        else {
            m_tail->next = temp;
            temp->before = m_tail;
            m_tail = temp;
        }
        m_size++;
    }

    void print() {
        ListNode<T>* temp = m_head;
        while (temp != nullptr) {
            std::cout << temp->data << ", ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    size_t size() {
        return m_size;
    }

    T* getElementByNumber(size_t number) {
        ListNode<T>* temp = m_head;
        size_t i = 0;
        while (temp != nullptr) {
            if (i == number) {
                return &temp->data;
            }
            temp = temp->next;
            i++;
        }
        return nullptr;
    }

    class Iterator {
    private:
        ListNode<T>* current;

    public:
        Iterator(ListNode<T>* node) : current(node) {};

        Iterator& operator=(const Iterator& other) {
            current = other.current;
            return *this;
        }
        Iterator(Iterator&& other) noexcept : current(other.current) {
            other.current = nullptr;
        }
        Iterator& operator=(Iterator&& other) noexcept {
            if (this != &other) {
                current = other.current;
                other.current = nullptr;
            }
            return *this;
        }
        const T& operator*() const {
            return current->data;
        }

        T& operator*() {
            return current->data;
        }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        Iterator& operator=(ListNode<T>* node) {
            this->current = node;
            return *this;
        }

        Iterator operator++(int) {
            Iterator iterator = *this;
            ++(*this);
            return iterator;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() const {
        return Iterator(m_head);
    }


    Iterator end() const {
        return Iterator(nullptr);
    }

    class ReverseIterator {
    private:
        ListNode<T>* current;

    public:
        ReverseIterator(ListNode<T>* node) : current(node) {}

        T operator*() const {
            return current->data;
        }

        ReverseIterator& operator++() {
            current = current->before;
            return *this;
        }

        bool operator!=(const ReverseIterator& other) const {
            return current != other.current;
        }
    };

    ReverseIterator rbegin() const {
        return ReverseIterator(m_tail);
    }

    ReverseIterator rend() const {
        return ReverseIterator(nullptr);
    }

    bool empty() {
        return m_size == 0;
    }
};