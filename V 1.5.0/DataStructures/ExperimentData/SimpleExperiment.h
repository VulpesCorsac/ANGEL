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
    double X;
    double XSD;
    double Y;
    double YSD;
    double Fext;
    double FextSD;
    double Fgen;
    double Time;
    int    IntervalNumber;
    int    RoundNumber;
};

class SimpleExperimentData {
private:
    DataVector < double > R;
    DataVector < double > RSD;
    DataVector < double > Theta;
    DataVector < double > ThetaSD;
    DataVector < double > X;
    DataVector < double > XSD;
    DataVector < double > Y;
    DataVector < double > YSD;
    DataVector < double > Fext;
    DataVector < double > FextSD;
    DataVector < double > Fgen;
    DataVector < double > Time;
    DataVector <  int   > IntervalNumber;
    DataVector <  int   > RoundNumber;

    int size;

    bool includeXY = false;
    bool includeIntervalNumber = false;
    bool includeRoundNumber = false;

public:
    SimpleExperimentData(const bool &new_includeXY = false, const bool &new_includeIntervalNumber = false, const bool &new_includeRoundNumber = false) {
        setIncludeXY(new_includeXY);
        setIncludeIntervalNumber(new_includeIntervalNumber);
        setIncludeRoundNumber(new_includeRoundNumber);

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

    DataVector < double > getXDataVector() const {
        return X;
    }

    std::vector < double > getXVector() const {
        return X.getVector();
    }

    double getXat(const int &i) const {
        return X.at(i);
    }

    double getXMin() const {
        return X.getMin();
    }

    double getXMax() const {
        return X.getMax();
    }

    DataVector < double > getXSDDataVector() const {
        return XSD;
    }

    std::vector < double > getXSDVector() const {
        return XSD.getVector();
    }

    double getXSDat(const int &i) const {
        return XSD.at(i);
    }

    double getXSDMin() const {
        return XSD.getMin();
    }

    double getXSDMax() const {
        return XSD.getMax();
    }

    DataVector < double > getYDataVector() const {
        return Y;
    }

    std::vector < double > getYVector() const {
        return Y.getVector();
    }

    double getYat(const int &i) const {
        return Y.at(i);
    }

    double getYMin() const {
        return Y.getMin();
    }

    double getYMax() const {
        return Y.getMax();
    }

    DataVector < double > getYSDDataVector() const {
        return YSD;
    }

    std::vector < double > getYSDVector() const {
        return YSD.getVector();
    }

    double getYSDat(const int &i) const {
        return YSD.at(i);
    }

    double getYSDMin() const {
        return YSD.getMin();
    }

    double getYSDMax() const {
        return YSD.getMax();
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

    DataVector < int > getIntervalNumberDataVector() const {
        return IntervalNumber;
    }

    std::vector < int > getIntervalNumberVector() const {
        return IntervalNumber.getVector();
    }

    int getIntervalNumberat(const int &i) const {
        return IntervalNumber.at(i);
    }

    int getIntervalNumberMin() const {
        return IntervalNumber.getMin();
    }

    int getIntervalNumberMax() const {
        return IntervalNumber.getMax();
    }

    DataVector < int > getRoundNumberDataVector() const {
        return RoundNumber;
    }

    std::vector < int > getRoundNumberVector() const {
        return RoundNumber.getVector();
    }

    int getRoundNumberat(const int &i) const {
        return RoundNumber.at(i);
    }

    int getRoundNumberMin() const {
        return RoundNumber.getMin();
    }

    int getRoundNumberMax() const {
        return RoundNumber.getMax();
    }

    void push_back(const SimpleExperimentPoint &experimentData) {
        R.push_back(experimentData.R);
        RSD.push_back(experimentData.RSD);
        Theta.push_back(experimentData.Theta);
        ThetaSD.push_back(experimentData.ThetaSD);
        if (includeXY) {
            X.push_back(experimentData.X);
            XSD.push_back(experimentData.XSD);
            Y.push_back(experimentData.Y);
            YSD.push_back(experimentData.YSD);
        }
        Fext.push_back(experimentData.Fext);
        FextSD.push_back(experimentData.FextSD);
        Fgen.push_back(experimentData.Fgen);
        Time.push_back(experimentData.Time);
        if (includeIntervalNumber)
            IntervalNumber.push_back(experimentData.IntervalNumber);
        if (includeRoundNumber)
            RoundNumber.push_back(experimentData.RoundNumber);

        size++;

        return;
    }

    SimpleExperimentPoint getPointat(const int &i) const {
        SimpleExperimentPoint ans;

        ans.R = getRat(i);
        ans.RSD = getRSDat(i);
        ans.Theta = getThetaat(i);
        ans.ThetaSD = getThetaSDat(i);
        if (includeXY) {
            ans.X = getXat(i);
            ans.XSD = getXSDat(i);
            ans.Y = getYat(i);
            ans.YSD = getYSDat(i);
        } else {
            ans.X = 0;
            ans.XSD = 0;
            ans.Y = 0;
            ans.YSD = 0;
        }
        ans.Fext = getFextat(i);
        ans.FextSD = getFextSDat(i);
        ans.Fgen = getFgenat(i);
        ans.Time = getTimeat(i);
        if (includeIntervalNumber)
            ans.IntervalNumber = getIntervalNumberat(i);
        else
            ans.IntervalNumber = 0;
        if (includeRoundNumber)
            ans.RoundNumber = getRoundNumberat(i);
        else
            ans.RoundNumber = 0;

        return ans;
    }

    void clear() {
        R.clear();
        RSD.clear();
        Theta.clear();
        ThetaSD.clear();
        X.clear();
        XSD.clear();
        Y.clear();
        YSD.clear();
        Fext.clear();
        FextSD.clear();
        Fgen.clear();
        Time.clear();
        IntervalNumber.clear();
        RoundNumber.clear();

        size = 0;
    }

    bool isEmpty() const {
        return (size == 0);
    }

    int getSize() const {
        return size;
    }

    void setIncludeXY(const bool &new_includeXY = false) {
        includeXY = new_includeXY;

        return;
    }

    bool getIncludeXY() const {
        return includeXY;
    }

    void setIncludeIntervalNumber(const bool &new_includeIntervalNumber = false) {
        includeIntervalNumber = new_includeIntervalNumber;

        return;
    }

    bool getIncludeIntervalNumberY() const {
        return includeIntervalNumber;
    }

    void setIncludeRoundNumber(const bool &new_includeRoundNumber = false) {
        includeRoundNumber = new_includeRoundNumber;

        return;
    }

    bool getIncludeRoundNumber() const {
        return includeRoundNumber;
    }

};

#endif // SIMPLEEXPERIMENT

