#ifndef KINETICS_H
#define KINETICS_H

#include <vector>
#include <algorithm>

#include "DataVector.h"

struct KineticThetaPoint {
    double Theta;
    double Time;
};

class KineticThetaData {
private:
    DataVector < double > Theta;
    DataVector < double > Time;

    int size;

public:
    KineticThetaData() {
        clear();
    }

    ~KineticThetaData() {
        clear();
    }

    DataVector < double > getThetaDataVector() const {
        return Theta;
    }

    std::vector < double > getThetaVector() const {
        return Theta.getVector();
    }

    double getThetaat(const int &i) const {
        return Theta.at(i);
    }

    double getThetaMin() const {
        return Theta.getMin();
    }

    double getThetaMax() const {
        return Theta.getMax();
    }

    DataVector < double > getTimeDataVector() const {
        return Time;
    }

    std::vector < double > getTimeVector() const {
        return Time.getVector();
    }

    double getTimeat(const int &i) const {
        return Time.at(i);
    }

    double getTimeMin() const {
        return Time.getMin();
    }

    double getTimeMax() const {
        return Time.getMax();
    }

    void push_back(const KineticThetaPoint &experimentData) {
        Theta.push_back(experimentData.Theta);
        Time.push_back(experimentData.Time);

        size++;

        return;
    }

    KineticThetaPoint getPointat(const int &i) const {
        KineticThetaPoint ans;

        ans.Theta = getThetaat(i);
        ans.Time = getTimeat(i);

        return ans;
    }

    void clear() {
        Theta.clear();
        Time.clear();

        size = 0;
    }

    bool isEmpty() const {
        return (size == 0);
    }

    int getSize() const {
        return size;
    }

};

struct KineticFrequencyPoint {
    double Fgen;
    double Fext;
    double Time;
};

class KineticFrequencyData {
private:
    DataVector < double > Fext;
    DataVector < double > Fgen;
    DataVector < double > Time;

    int size;

public:
    KineticFrequencyData() {
        clear();
    }

    ~KineticFrequencyData() {
        clear();
    }

    DataVector < double > getFextDataVector() const {
        return Fext;
    }

    std::vector < double > getFextVector() const {
        return Fext.getVector();
    }

    double getFextat(const int &i) const {
        return Fext.at(i);
    }

    double getFextMin() const {
        return Fext.getMin();
    }

    double getFextMax() const {
        return Fext.getMax();
    }

    DataVector < double > getFgenDataVector() const {
        return Fgen;
    }

    std::vector < double > getFgenVector() const {
        return Fgen.getVector();
    }

    double getFgenat(const int &i) const {
        return Fgen.at(i);
    }

    double getFgenMin() const {
        return Fgen.getMin();
    }

    double getFgenMax() const {
        return Fgen.getMax();
    }

    DataVector < double > getTimeDataVector() const {
        return Time;
    }

    std::vector < double > getTimeVector() const {
        return Time.getVector();
    }

    double getTimeat(const int &i) const {
        return Time.at(i);
    }

    double getTimeMin() const {
        return Time.getMin();
    }

    double getTimeMax() const {
        return Time.getMax();
    }

    void push_back(const KineticFrequencyPoint &experimentData) {
        Fext.push_back(experimentData.Fext);
        Fgen.push_back(experimentData.Fgen);
        Time.push_back(experimentData.Time);

        size++;

        return;
    }

    KineticFrequencyPoint getPointat(const int &i) const {
        KineticFrequencyPoint ans;

        ans.Fext = getFextat(i);
        ans.Fgen = getFgenat(i);
        ans.Time = getTimeat(i);

        return ans;
    }

    void clear() {
        Fext.clear();
        Fgen.clear();
        Time.clear();

        size = 0;
    }

    bool isEmpty() const {
        return (size == 0);
    }

    int getSize() const {
        return size;
    }

};

#endif // KINETICS_H

