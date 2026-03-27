#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace advsnd;
#pragma link C++ defined_in namespace advsnd;

#pragma link C++ class Floor + ;
#pragma link C++ class boxTarget + ;
#pragma link C++ class EmulsionDet + ;
#pragma link C++ class EmulsionDetPoint + ;
#pragma link C++ class EmulsionDetContFact + ;
#pragma link C++ class Scifi + ;
#pragma link C++ class ScifiPoint + ;
#pragma link C++ class MuFilter + ;
#pragma link C++ class MuFilterPoint + ;
#pragma link C++ class MuFilterHit + ;
#pragma link C++ class AdvTargetHit + ;
#pragma link C++ class AdvMuFilterHit + ;
#pragma link C++ class sndScifiHit + ;
#pragma link C++ class sndCluster;
#pragma link C++ class SNDLHCEventHeader + ;
#pragma link C++ class sndRecoTrack + ;
#pragma link C++ class Magnet + ;
#pragma link C++ class MagnetPoint + ;
#pragma link C++ class AdvTarget + ;
#pragma link C++ class AdvTargetPoint + ;
#pragma link C++ class AdvMuFilter + ;
#pragma link C++ class AdvMuFilterPoint + ;
#pragma link C++ class digitisation/ChargeDivision + ;
#pragma link C++ class digitisation/ChargeDrift +;
#pragma link C++ class digitisation/InducedCharge +;
#pragma link C++ class digitisation/FrontendDriver +;
#pragma link C++ class digitisation/StripNoise +;
#pragma link C++ class digitisation/SiG4UniversalFluctuation + ;
#pragma link C++ class digitisation/EnergyFluctUnit + ;
#pragma link C++ class digitisation/SurfaceSignal + ;
#pragma link C++ class digitisation/AdvDigitisation + ;
#pragma link C++ class advsnd::DigitizePoints<AdvTargetPoint, AdvTargetHit>+;
#pragma link C++ class advsnd::DigitizePoints<AdvMuFilterPoint, AdvMuFilterHit>+;
#pragma link C++ class advsnd::LinkPointsToDigi<AdvTargetPoint>+;
#pragma link C++ class advsnd::LinkPointsToDigi<AdvMuFilterPoint>+;

#pragma link C++ function advsnd::GetStripId<AdvTargetPoint>;
#pragma link C++ function advsnd::GetStripId<AdvMuFilterPoint>;
#pragma link C++ function advsnd::Digitize<AdvTargetPoint, AdvTargetHit>;
#pragma link C++ function advsnd::Digitize<AdvMuFilterPoint, AdvMuFilterHit>;
#pragma link C++ function advsnd::McLink<AdvTargetPoint>;
#pragma link C++ function advsnd::McLink<AdvMuFilterPoint>;
#endif
