#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"


class MyPrimaryGenerator;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWith3VectorAndUnit;

class MyGeneratorMessenger: public G4UImessenger
{
public:
  MyGeneratorMessenger(MyPrimaryGenerator*);
  ~MyGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);

private:
  MyPrimaryGenerator* fPrimaryGen; 
  
  G4UIdirectory*                    beamParametersDir;
  G4UIdirectory*                    EnergyDir;
  G4UIdirectory*                    particlePositionDir;
  G4UIcmdWithADoubleAndUnit*        meanKineticEnergyCmd;    
  G4UIcmdWithADoubleAndUnit*        sigmaEnergyCmd;  
  G4UIcmdWithADoubleAndUnit*        XpositionCmd;   
  G4UIcmdWithADoubleAndUnit*        YpositionCmd;
  G4UIcmdWithADoubleAndUnit*        ZpositionCmd; 
  G4UIcmdWithADoubleAndUnit*        sigmaXCmd; 
  G4UIcmdWithADoubleAndUnit*        sigmaYCmd; 

  G4UIcmdWithADoubleAndUnit*        momentumXCmd;
  G4UIcmdWithADoubleAndUnit*        momentumYCmd;
  G4UIcmdWithADoubleAndUnit*        momentumZCmd;
 
};

#endif
