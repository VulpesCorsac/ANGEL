#ifndef EXPERIMENTDATA_H
#define EXPERIMENTDATA_H

#include <vector>

#include "../../Functions/AllFunctions.h"

typedef double T_expData;

typedef std::vector < T_expData > datavec;

class ExperimentDataPoint
{
public:
    T_expData R;
    T_expData RSD;
    T_expData Theta;
    T_expData ThetaSD;
    T_expData X;
    T_expData XSD;
    T_expData Y;
    T_expData YSD;
    T_expData AmplitudeGenerator;
    T_expData AmplitudeGeneratorSD;
    T_expData FrequencyGenerator;
    T_expData FrequencyGeneratorSD;
    T_expData ExternalFrequencyLockInAmplifier;
    T_expData ExternalFrequencyLockInAmplifierSD;
    T_expData InternalFrequencyLockInAmplifier;
    T_expData InternalFrequencyLockInAmplifierSd;
    T_expData Wavelength;
    T_expData WavelengthSD;
    T_expData Time;
    T_expData TimeSD;
};

class ExperimentDataVector
{
public:
    datavec R;
    T_expData minR;
    T_expData maxR;

    datavec Theta;
    T_expData minTheta;
    T_expData maxTheta;

    datavec AmplitudeGenerator;
    T_expData minAmplitudeGenerator;
    T_expData maxAmplitudeGenerator;

    datavec FrequencyGenerator;
    T_expData minFrequencyGenerator;
    T_expData maxFrequencyGenerator;


    datavec ExternalFrequencyLockInAmplifier;
    T_expData minExternalFrequencyLockInAmplifier;
    T_expData maxExternalFrequencyLockInAmplifier;

    datavec InternalFrequencyLockInAmplifier;
    T_expData minInternalFrequencyLockInAmplifier;
    T_expData maxInternalFrequencyLockInAmplifier;

    datavec Wavelength;
    T_expData minWavelength;
    T_expData maxWavelength;

    datavec Time;
    T_expData minTime;
    T_expData maxTime;

    void allClear() {
        R.clear();
        Theta.clear();
        AmplitudeGenerator.clear();
        FrequencyGenerator.clear();
        ExternalFrequencyLockInAmplifier.clear();
        InternalFrequencyLockInAmplifier.clear();
        Wavelength.clear();
        Time.clear();

        return;
    }

    void reserve(const int &n) {
        R.reserve(n);
        Theta.reserve(n);
        AmplitudeGenerator.reserve(n);
        FrequencyGenerator.reserve(n);
        ExternalFrequencyLockInAmplifier.reserve(n);
        InternalFrequencyLockInAmplifier.reserve(n);
        Wavelength.reserve(n);
        Time.reserve(n);

        return;
    }

    size_t getSize() const {
        return R.size();
    }

    void push_back(const ExperimentDataPoint &point) {
        if (R.size() == 0) {
            minR = point.R;
            maxR = point.R;
        } else {
            minR = _min(minR, point.R);
            maxR = _max(maxR, point.R);
        }
        R.push_back(point.R);

        if (Theta.size() == 0) {
            minTheta = point.Theta;
            maxTheta = point.Theta;
        } else {
            minTheta = _min(minTheta, point.Theta);
            maxTheta = _max(maxTheta, point.Theta);
        }
        Theta.push_back(point.Theta);

        if (AmplitudeGenerator.size() == 0) {
            minAmplitudeGenerator = point.AmplitudeGenerator;
            maxAmplitudeGenerator = point.AmplitudeGenerator;
        } else {
            minAmplitudeGenerator = _min(minAmplitudeGenerator, point.AmplitudeGenerator);
            maxAmplitudeGenerator = _max(maxAmplitudeGenerator, point.AmplitudeGenerator);
        }
        AmplitudeGenerator.push_back(point.AmplitudeGenerator);

        if (FrequencyGenerator.size() == 0) {
            minFrequencyGenerator = point.FrequencyGenerator;
            maxFrequencyGenerator = point.FrequencyGenerator;
        } else {
            minFrequencyGenerator = _min(minFrequencyGenerator, point.FrequencyGenerator);
            maxFrequencyGenerator = _max(maxFrequencyGenerator, point.FrequencyGenerator);
        }
        FrequencyGenerator.push_back(point.FrequencyGenerator);

        if (ExternalFrequencyLockInAmplifier.size() == 0) {
            minExternalFrequencyLockInAmplifier = point.ExternalFrequencyLockInAmplifier;
            maxExternalFrequencyLockInAmplifier = point.ExternalFrequencyLockInAmplifier;
        } else {
            minExternalFrequencyLockInAmplifier = _min(minExternalFrequencyLockInAmplifier, point.ExternalFrequencyLockInAmplifier);
            maxExternalFrequencyLockInAmplifier = _max(maxExternalFrequencyLockInAmplifier, point.ExternalFrequencyLockInAmplifier);
        }
        ExternalFrequencyLockInAmplifier.push_back(point.ExternalFrequencyLockInAmplifier);

        if (InternalFrequencyLockInAmplifier.size() == 0) {
            minInternalFrequencyLockInAmplifier = point.InternalFrequencyLockInAmplifier;
            maxInternalFrequencyLockInAmplifier = point.InternalFrequencyLockInAmplifier;
        } else {
            minInternalFrequencyLockInAmplifier = _min(minInternalFrequencyLockInAmplifier, point.InternalFrequencyLockInAmplifier);
            maxInternalFrequencyLockInAmplifier = _max(maxInternalFrequencyLockInAmplifier, point.InternalFrequencyLockInAmplifier);
        }
        InternalFrequencyLockInAmplifier.push_back(point.InternalFrequencyLockInAmplifier);

        if (Wavelength.size() == 0) {
            minWavelength = point.Wavelength;
            maxWavelength = point.Wavelength;
        } else {
            minWavelength = _min(minWavelength, point.Wavelength);
            maxWavelength = _max(maxWavelength, point.Wavelength);
        }
        Wavelength.push_back(point.Wavelength);

        if (Time.size() == 0) {
            minTime = point.Time;
            maxTime = point.Time;
        } else {
            minTime = _min(minTime, point.Time);
            maxTime = _max(maxTime, point.Time);
        }
        Time.push_back(point.Time);

        return;
    }

};

#endif // EXPERIMENTDATA_H
