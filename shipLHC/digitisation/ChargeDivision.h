#ifndef SHIPLHC_CHARGEDIVISION_H_
#define SHIPLHC_CHARGEDIVISION_H_

#include "AdvTargetPoint.h"
#include "EnergyFluctUnit.h"
#include "TVector3.h"

#include <iostream>
#include <vector>

class ChargeDivision
{
  public:
    ChargeDivision();
    void Divide(Int_t detID, const std::vector<AdvTargetPoint*>& V, std::vector<EnergyFluctUnit>& ELossVector);
    void Divide(Int_t detID, const std::vector<const AdvTargetPoint*>& V, std::vector<EnergyFluctUnit>& ELossVector);
    TVector3 DriftDir(TVector3 EntryPoint, TVector3 ExitPoint, float length);
    TVector3 getLocal(Int_t detID,TVector3 point);

  private:
    std::vector<double> PulseValues;
    Double_t ParticleCharge;
    Double_t ParticleMass;
    Int_t NumberofSegments;
    float segLen;
};

#endif   // SHIPLHC_CHARGEDIVISION_H_
