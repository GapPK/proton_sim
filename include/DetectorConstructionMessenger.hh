#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class DetectorConstructionMessenger : public G4UImessenger
{
public:
    DetectorConstructionMessenger(DetectorConstruction* detConstruction);
    virtual ~DetectorConstructionMessenger();

    virtual void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    DetectorConstruction* fDetectorConstruction;
    G4UIdirectory* fDetectorDir;
    G4UIcmdWithADoubleAndUnit* fVDetectorDistCmd;
    G4UIcmdWithADoubleAndUnit* fVDetectorSizeCmd;
    
    // New commands for target
    G4UIcmdWithADoubleAndUnit* fTargetThicknessCmd;
    G4UIcmdWithAString* fTargetMaterialCmd;
};

#endif