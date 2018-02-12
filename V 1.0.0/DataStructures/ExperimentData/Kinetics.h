#ifndef KINETICS_H
#define KINETICS_H

#include <vector>
#include <algorithm>

#include "DataVector.h"

struct KineticThetaPoint {
    double Theta;
    double R;
    double TimeRound;
    double TimeExperiment;
};

class KineticThetaData {
private:
    DataVector < double > Theta;
    DataVector < double > R;
    DataVector < double > TimeRound;
    DataVector < double > TimeExperiment;

    int size;

    bool includeTimeExperiment;

public:
    explicit KineticThetaData(const bool &new_includeTimeExperiment = true) {
        setIncludeTimeExperiment(new_includeTimeExperiment);

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

    DataVector < double > getTimeRoundDataVector() const {
        return TimeRound;
    }

    std::vector < double > getTimeRoundVector() const {
        return TimeRound.getVector();
    }

    double getTimeRoundat(const int &i) const {
        return TimeRound.at(i);
    }

    double getTimeRoundMin() const {
        return TimeRound.getMin();
    }

    double getTimeRoundMax() const {
        return TimeRound.getMax();
    }

    DataVector < double > getTimeExperimentDataVector() const {
        return TimeExperiment;
    }

    std::vector < double > getTimeExperimentVector() const {
        return TimeExperiment.getVector();
    }

    double getTimeExperimentat(const int &i) const {
        return TimeExperiment.at(i);
    }

    double getTimeExperimentMin() const {
        return TimeExperiment.getMin();
    }

    double getTimeExperimentMax() const {
        return TimeExperiment.getMax();
    }

    void push_back(const KineticThetaPoint &experimentData) {
        Theta.push_back(experimentData.Theta);
        R.push_back(experimentData.R);
        TimeRound.push_back(experimentData.TimeRound);
        TimeExperiment.push_back(experimentData.TimeExperiment);

        size++;

        return;
    }

    KineticThetaPoint getPointat(const int &i) const {
        KineticThetaPoint ans;

        ans.Theta = getThetaat(i);
        ans.R = getRat(i);
        ans.TimeRound = getTimeRoundat(i);
        ans.TimeExperiment = getTimeExperimentat(i);

        return ans;
    }

    void clear() {
        Theta.clear();
        R.clear();
        TimeRound.clear();
        TimeExperiment.clear();

        size = 0;
    }

    bool isEmpty() const {
        return (size == 0);
    }

    int getSize() const {
        return size;
    }

    void setIncludeTimeExperiment(const bool &new_includeTimeExperiment = true) {
        includeTimeExperiment = new_includeTimeExperiment;

        return;
    }

    bool getIncludeTimeExperiment() const {
        return includeTimeExperiment;
    }

};

struct KineticFrequencyPoint {
    double Fgen;
    double Fext;
    double Time;
    double ThetaPeak;
};

class KineticFrequencyData {
private:
    DataVector < double > Fext;
    DataVector < double > Fgen;
    DataVector < double > Time;
    DataVector < double > ThetaPeak;

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

    DataVector < double > getThetaPeakDataVector() const {
        return ThetaPeak;
    }

    std::vector < double > getThetaPeakVector() const {
        return ThetaPeak.getVector();
    }

    double getThetaPeakat(const int &i) const {
        return ThetaPeak.at(i);
    }

    double getThetaPeakMin() const {
        return ThetaPeak.getMin();
    }

    double getThetaPeakMax() const {
        return ThetaPeak.getMax();
    }

    void push_back(const KineticFrequencyPoint &experimentData) {
        Fext.push_back(experimentData.Fext);
        Fgen.push_back(experimentData.Fgen);
        Time.push_back(experimentData.Time);
        ThetaPeak.push_back(experimentData.ThetaPeak);

        size++;

        return;
    }

    KineticFrequencyPoint getPointat(const int &i) const {
        KineticFrequencyPoint ans;

        ans.Fext = getFextat(i);
        ans.Fgen = getFgenat(i);
        ans.Time = getTimeat(i);
        ans.ThetaPeak = getThetaPeakat(i);

        return ans;
    }

    void clear() {
        Fext.clear();
        Fgen.clear();
        Time.clear();
        ThetaPeak.clear();

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

