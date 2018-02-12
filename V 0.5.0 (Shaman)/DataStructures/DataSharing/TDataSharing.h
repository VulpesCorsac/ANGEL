#ifndef TDATASHARING_H_INCLUDED
#define TDATASHARING_H_INCLUDED

#include <vector>
#include <atomic>

#include "../LockFreeQueue/TLockFreeQueue.h"

template < typename T >
class TDataSharing {
private:
    bool working_with_queue = false;
    bool new_data_vector_enable = true;
    size_t divider, last;

public:
    TLockFreeQueue < T > data_queue;
    std::vector < T > data_vector;
    std::vector < T > new_data;

public:
    TDataSharing(size_t new_size = 0) {
        reserve_vector(new_size);
        divider = last = -1;
    }

    ~TDataSharing(void) {
        clear_data();
        data_queue.~TLockFreeQueue();
    }

    void reserve_vector(size_t new_size = 0) {
        data_vector.reserve(new_size);
    }

    void clear_data() {
        data_queue.clearData();
        data_vector.clear();
        new_data.clear();
        divider = last = -1;
    }

    void setWorkingWithQueue(const bool new_working_with_queue) {
        working_with_queue = new_working_with_queue;
    }

    void setClearRead(const bool new_clear_read) {
        data_queue.setClearRead(new_clear_read);
    }

    void setNewDataVectorEnable(const bool _new_data_vector_enable) {
        new_data_vector_enable = _new_data_vector_enable;
    }

    void clearNewData() {
        new_data.clear();
    }

    void push(const T &val) {
        if (working_with_queue) {
            data_queue.push(val);
        } else {
            data_vector.push_back(val);
            last++;
        }
        return;
    }

    bool pop(void) {
        T val;
        bool flag = false;
        if (working_with_queue) {
            while (data_queue.pop(val)) {
                data_vector.push_back(val);
                if (new_data_vector_enable)
                    new_data.push_back(val);
                flag = true;
            }
        } else {
            size_t temp_last = last;
            while (divider != temp_last) {
                new_data.push_back(data_vector[++divider]);
                flag = true;
            }
        }
        return flag;
    }
};

#endif // TDATASHARING_H_INCLUDED
