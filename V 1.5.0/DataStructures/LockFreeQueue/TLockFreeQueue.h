#ifndef TLOCKFREEQUEUE
#define TLOCKFREEQUEUE

#include <atomic>

template < typename T >
class TLockFreeQueue {
private:
    struct Node {
        Node(const T &val) {
            data = val;
            next = nullptr;
        }
        T data;
        Node *next;
    };
    bool clear_read = true;
    Node *first;
    std::atomic < Node* > divider, last;

public:
    explicit TLockFreeQueue() {
        init();
    }

    ~TLockFreeQueue() {
        clearData();
    }

    void init() {
        first = divider = last = new Node( T() );
    }

    void clearData() {
        while (first != nullptr) {
            Node *temp = first;
            first = first->next;
            delete temp;
        }
        init();
    }

    void setClearRead(bool new_clear_read) {
        clear_read = new_clear_read;
    }

    bool nothingToRead(void) const {
        return divider == last;
    }

    bool empty(void) const {
        return first == last;
    }

    void clearRead() {
        while (first != divider) {
            Node *temp = first;
            first = first->next;
            delete temp;
        }
    }

    void push(const T &val) {
        (*last).next = new Node(val);
        last = (*last).next;
        if (clear_read)
            clearRead();
        return;
    }

    bool pop(T &val) {
        if (!nothingToRead()) {
            val = (*divider).next->data;
            divider = (*divider).next;
            return true;
        }
        return false;
    }
};

#endif // TLOCKFREEQUEUE
