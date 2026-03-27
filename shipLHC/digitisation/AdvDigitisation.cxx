#include "AdvDigitisation.h"
#include "AdvTargetPoint.h"
#include "ChargeDivision.h"
#include "ChargeDrift.h"
#include "InducedCharge.h"
#include "FrontendDriver.h"
#include "StripNoise.h"
#include "EnergyFluctUnit.h"
#include "SurfaceSignal.h"
#include "AdvSignal.h"
#include "SiDigiParameters.h"

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TStopwatch.h"
#include "TNtuple.h"
#include "TObject.h"

#include <TSystem.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

// Running the digitisation 

/* To be included :
    Add FED modes (FED mode implemented essentially ZS applied on top of VR mode)
    FED does not merge clusters at chip boundaries, needs to be implemented 
    APV killer for HIP needs to be implemented*/

AdvDigitisation::AdvDigitisation() {}

std::unordered_map<std::string, std::vector<Int_t>> AdvDigitisation::digirunoutput(Int_t detID, const std::vector<AdvTargetPoint *> &V, std::vector<EnergyFluctUnit>& EnergyLossVector, std::vector<SurfaceSignal>& DiffusionSignal, AdvSignal& TotalSignal, AdvSignal& FEDResponseSignal)
{
    // Charge Division
    ChargeDivision chargedivision{};
    chargedivision.Divide(detID, V, EnergyLossVector);
     
    // //Charge Drift
    ChargeDrift chargedrift{};
    chargedrift.Drift(EnergyLossVector, DiffusionSignal);

    //Induced Charge on strips
    InducedCharge inducedcharge{};
    inducedcharge.IntegrateCharge(DiffusionSignal, TotalSignal);

    //Frontend Driver Response 
    FrontendDriver frontenddriver{};
    frontenddriver.FEDResponse(TotalSignal, FEDResponseSignal);

    //Creating map of hit 
    std::unordered_map<std::string, std::vector<Int_t>> DigitisedHit; 
    std::vector<Double_t> Charge = FEDResponseSignal.getIntegratedSignal();
    std::vector<Int_t> Strips = FEDResponseSignal.getStrips();
    std::vector<Int_t> ADC(Charge.size()); 

    std::transform(Charge.begin(), Charge.end(), ADC.begin(), [](Double_t x) { 
        if (x>0){return (int)x;}
        else { return 0;}
        });

    DigitisedHit["Strips"] = Strips; 
    DigitisedHit["ADC"] = ADC;

    return DigitisedHit;
}

std::unordered_map<std::string, std::vector<Int_t>> AdvDigitisation::digirunoutput(Int_t detID, const std::vector<const AdvTargetPoint *> &V)
{
    std::vector<EnergyFluctUnit> EnergyLossVector;
    std::vector<SurfaceSignal> DiffusionSignal; 
    AdvSignal TotalSignal; 
    AdvSignal FEDResponseSignal;
    // Charge Division
    ChargeDivision chargedivision{};
    chargedivision.Divide(detID, V, EnergyLossVector);
     
    // //Charge Drift
    ChargeDrift chargedrift{};
    chargedrift.Drift(EnergyLossVector, DiffusionSignal);

    //Induced Charge on strips
    InducedCharge inducedcharge{};
    inducedcharge.IntegrateCharge(DiffusionSignal, TotalSignal);

    //Frontend Driver Response 
    FrontendDriver frontenddriver{};
    frontenddriver.FEDResponse(TotalSignal, FEDResponseSignal);

    //Creating map of hit 
    std::unordered_map<std::string, std::vector<Int_t>> DigitisedHit; 
    std::vector<Double_t> Charge = FEDResponseSignal.getIntegratedSignal();
    std::vector<Int_t> Strips = FEDResponseSignal.getStrips();
    std::vector<Int_t> ADC(Charge.size()); 

    std::transform(Charge.begin(), Charge.end(), ADC.begin(), [](Double_t x) { 
        if (x>0){return (int)x;}
        else { return 0;}
        });

    DigitisedHit["Strips"] = Strips; 
    DigitisedHit["ADC"] = ADC;

    return DigitisedHit;
}