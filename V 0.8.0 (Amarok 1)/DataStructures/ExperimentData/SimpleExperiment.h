#ifndef SIMPLEEXPERIMENT
#define SIMPLEEXPERIMENT

#include <vector>
#include <algorithm>

#include "DataVector.h"

struct SimpleExperimentPoint {
    double R;
    double RSD;
    double Theta;
    double ThetaSD;
    double Fext;
    double FextSD;
    double Fgen;
    double Time;
};

class SimpleExperimentData {
private:
    DataVector < double > R;
    DataVector < double > RSD;
    DataVector < double > Theta;
    DataVector < double > ThetaSD;
    DataVector < double > Fext;
    DataVector < double > FextSD;
    DataVector < double > Fgen;
    DataVector < double > Time;

    int size;

public:
    SimpleExperimentData() {
        clear();
    }

    ~SimpleExperimentData() {
        clear();
    }

    DataVector < double > getRDataVector() const {
        return R;
    }

    std::vector < double > getRVector() const {
        return R.getVector();
    }

    double getRat(const int &i) const {
        return R.at(i);
    }

    double getRMin() const {
        return R.getMin();
    }

    double getRMax() const {
        return R.getMax();
    }

    DataVector < double > getRSDDataVector() const {
        return RSD;
    }

    std::vector < double > getRSDVector() const {
        return RSD.getVector();
    }

    double getRSDat(const int &i) const {
        return RSD.at(i);
    }

    double getRSDMin() const {
        return RSD.getMin();
    }

    double getRSDMax() const {
        return RSD.getMax();
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

    DataVector < double > getThetaSDDataVector() const {
        return ThetaSD;
    }

    std::vector < double > getThetaSDVector() const {
        return ThetaSD.getVector();
    }

    double getThetaSDat(const int &i) const {
        return ThetaSD.at(i);
    }

    double getThetaSDMin() const {
        return ThetaSD.getMin();
    }

    double getThetaSDMax() const {
        return ThetaSD.getMax();
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

    DataVector < double > getFextSDDataVector() const {
        return FextSD;
    }

    std::vector < double > getFextSDVector() const {
        return FextSD.getVector();
    }

    double getFextSDat(const int &i) const {
        return FextSD.at(i);
    }

    double getFextSDMin() const {
        return FextSD.getMin();
    }

    double getFextSDMax() const {
        return FextSD.getMax();
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

    void push_back(const SimpleExperimentPoint &experimentData) {
        R.push_back(experimentData.R);
        RSD.push_back(experimentData.RSD);
        Theta.push_back(experimentData.Theta);
        ThetaSD.push_back(experimentData.ThetaSD);
        Fext.push_back(experimentData.Fext);
        FextSD.push_back(experimentData.FextSD);
        Fgen.push_back(experimentData.Fgen);
        Time.push_back(experimentData.Time);

        size++;

        return;
    }

    SimpleExperimentPoint getPointat(const int &i) const {
        SimpleExperimentPoint ans;

        ans.R = getRat(i);
        ans.RSD = getRSDat(i);
        ans.Theta = getThetaat(i);
        ans.ThetaSD = getThetaSDat(i);
        ans.Fext = getFextat(i);
        ans.FextSD = getFextSDat(i);
        ans.Fgen = getFgenat(i);
        ans.Time = getTimeat(i);

        return ans;
    }

    void clear() {
        R.clear();
        RSD.clear();
        Theta.clear();
        ThetaSD.clear();
        Fext.clear();
        FextSD.clear();
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

#endif // SIMPLEEXPERIMENT

