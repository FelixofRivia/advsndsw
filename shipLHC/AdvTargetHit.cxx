#include "AdvTargetHit.h"
#include "AdvTargetPoint.h"
#include "digitisation/AdvSignal.h"
#include "digitisation/AdvDigitisation.h"
#include "FairLogger.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TGeoNavigator.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TVector3.h"
#include "TStopwatch.h"
#include "TNtuple.h"

#include <TDatabasePDG.h>
#include <iomanip>
#include <typeinfo>
#include <iostream>
#include <string>
#include <map>
using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
AdvTargetHit::AdvTargetHit()
    : SndlhcHit()
{
    flag = true;
    for (Int_t i = 0; i < 16; i++) {
        fMasked[i] = kFALSE;
    }
}
// -----   Standard constructor   ------------------------------------------
AdvTargetHit::AdvTargetHit(Int_t detID)
    : SndlhcHit(detID)
{
    flag = true;
    for (Int_t i = 0; i < 16; i++) {
        fMasked[i] = kFALSE;
    }
}

// -----   constructor from AdvTargetPoint   ------------------------------------------
AdvTargetHit::AdvTargetHit(Int_t detID, const std::vector<AdvTargetPoint*>& V, TNtuple* dat, std::vector<EnergyFluctUnit>* ChargeDivisionPoint, std::vector<SurfaceSignal>* ChargeDriftPoint, AdvSignal* InducedChargePoint, AdvSignal* FEDResponsePoint)
    : SndlhcHit(detID)
{
    AdvDigitisation advdigi{};
    std::vector<EnergyFluctUnit> EnergyLossVector;
    std::vector<SurfaceSignal> DiffusionSignal; 
    AdvSignal TotalSignal;
    AdvSignal FEDResponseSignal;

    //std::unordered_map<std::string, std::vector<Int_t>> fDigitisedHit;
    fDigitisedHit = advdigi.digirunoutput(detID, V, EnergyLossVector, DiffusionSignal, TotalSignal, FEDResponseSignal);
    flag = true;

    size = V.size(); 


    *ChargeDivisionPoint = EnergyLossVector;
    *ChargeDriftPoint = DiffusionSignal;  
    *InducedChargePoint = TotalSignal; 
    *FEDResponsePoint = FEDResponseSignal;

    for (Int_t i = 0; i < 16; i++) {
        fMasked[i] = kFALSE;
    }
    LOG(DEBUG) << "signal created";
}

// -----   constructor from AdvTargetPoint   ------------------------------------------
AdvTargetHit::AdvTargetHit(Int_t detID, const std::vector<const AdvTargetPoint*>& V)
    : SndlhcHit(detID)
{
    AdvDigitisation advdigi{};
    fDigitisedHit = advdigi.digirunoutput(detID, V);
    flag = true;
    size = V.size(); 

    for (Int_t i = 0; i < 16; i++) {
        fMasked[i] = kFALSE;
    }
    LOG(DEBUG) << "signal created";
}

// -----   Public method Print   -------------------------------------------
void AdvTargetHit::Print() const
{
    LOG(INFO) << " AdvTargetHit: AdvMuFilter hit "
              << " in detector " << fDetectorID;
}
