#ifndef SHIPLHC_ADVTARGETHIT_H_
#define SHIPLHC_ADVTARGETHIT_H_

#include "SiSensor.h"
#include "SndlhcHit.h"
#include "TArrayD.h"
#include "TVector3.h"
#include "TNtuple.h"
#include "digitisation/AdvSignal.h"
#include "digitisation/EnergyFluctUnit.h"
#include "digitisation/SurfaceSignal.h"
#include <map>

class AdvTargetPoint;

class TArrayD;

class AdvTargetHit : public SndlhcHit
{
  public:
    /** Default constructor **/
    AdvTargetHit();
    explicit AdvTargetHit(Int_t detID);

    // Constructor from AdvTargetPoint
    AdvTargetHit(Int_t detID, const std::vector<AdvTargetPoint*>& V, TNtuple* dat, std::vector<EnergyFluctUnit>* ChargeDivisionPoint, std::vector<SurfaceSignal>* ChargeDriftPoint, AdvSignal* InducedChargePoint, AdvSignal* FEDResponsePoint);

    /** Destructor **/
    ~AdvTargetHit() = default;

    /** Output to screen **/
    void Print() const;
    bool isValid() const { return flag; }
    bool isMasked(Int_t i) const { return fMasked[i]; }
    void SetMasked(Int_t i) { fMasked[i] = kTRUE; }
    std::unordered_map<std::string, std::vector<Int_t>> GetHit() const { return fDigitisedHit; }
    int constexpr GetSize() const {return size; }
    int constexpr GetLayer() const { return fDetectorID >> 17; }
    int constexpr GetPlane() const { return (fDetectorID >> 16) % 2; }   // 0 is X-plane, 1 is Y-pane
    int constexpr GetRow() const { return (fDetectorID >> 13) % 8; }
    int constexpr GetColumn() const { return (fDetectorID >> 11) % 4; }
    int constexpr GetSensor() const { return (fDetectorID >> 10) % 2; }
    int constexpr GetStrip() const { return (fDetectorID) % 1024; }
    int constexpr GetModule() const { return advsnd::target::columns * GetRow() + 1 + GetColumn(); }
    bool constexpr isVertical() const { return GetPlane() == 1; };

  private:
    bool flag;          ///< flag
    bool fMasked[16];   /// masked signal
    std::unordered_map<std::string, std::vector<Int_t>> fDigitisedHit; 
    Int_t size ; 
    ClassDef(AdvTargetHit, 1);
};

#endif   // SHIPLHC_ADVTARGETHIT_H_
