#ifndef DATAVECTOR
#define DATAVECTOR

#include <vector>
#include <algorithm>

template < typename T >
class DataVector {
private:
    std::vector < T > data;
    T minData;
    T maxData;

public:
    explicit DataVector(const int &n = 0) {
        clear();
        data.reserve(n);
    }

    ~DataVector() {
        clear();
    }

    void clear() {
        minData = maxData = 0;
        data.clear();

        return;
    }

    void push_back(const T &item) {
        if (data.empty()) {
            minData = maxData = item;
        } else {
            minData = std::min(minData, item);
            maxData = std::max(maxData, item);
        }

        data.push_back(item);

        return;
    }

    std::vector < T > getVector() const {
        return data;
    }

    T at(const int &i) const {
        return data.at(i);
    }

    T getMin() const {
        return minData;
    }

    T getMax() const {
        return maxData;
    }

    int getSize() const {
        return data.size();
    }

};

#endif // DATAVECTOR
