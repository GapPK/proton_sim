#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4RunManager.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* detConstruction)
: fDetectorConstruction(detConstruction)
{
    // Create directory for detector commands
    fDetectorDir = new G4UIdirectory("/myexp/");
    fDetectorDir->SetGuidance("Detector control commands");

    // Command to set VDetector distance
    fVDetectorDistCmd = new G4UIcmdWithADoubleAndUnit("/myexp/vdetector/setDistance", this);
    fVDetectorDistCmd->SetGuidance("Set VDetector distance from the center");
    fVDetectorDistCmd->SetParameterName("Distance", false);
    fVDetectorDistCmd->SetUnitCategory("Length");
    fVDetectorDistCmd->SetDefaultUnit("cm");
    fVDetectorDistCmd->SetRange("Distance>0");

    // Target thickness command
    fTargetThicknessCmd = new G4UIcmdWithADoubleAndUnit("/myexp/target/setThickness", this);
    fTargetThicknessCmd->SetGuidance("Set target thickness");
    fTargetThicknessCmd->SetParameterName("Thickness", false);
    fTargetThicknessCmd->SetUnitCategory("Length");
    fTargetThicknessCmd->SetDefaultUnit("um");
    fTargetThicknessCmd->SetRange("Thickness>0");

    // Target material command
    fTargetMaterialCmd = new G4UIcmdWithAString("/myexp/target/setMaterial", this);
    fTargetMaterialCmd->SetGuidance("Set target material (Au, Ag, Cu, etc.)");
    fTargetMaterialCmd->SetParameterName("Material", false);
}

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete fVDetectorDistCmd;
    delete fDetectorDir;
    delete fTargetThicknessCmd;
    delete fTargetMaterialCmd;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fVDetectorDistCmd) {
        // Update VDetector distance
        G4double newDist = fVDetectorDistCmd->GetNewDoubleValue(newValue);
        // You'll need to modify DetectorConstruction to support this
        fDetectorConstruction->SetVDetectorDistance(newDist);
    }

    else if (command == fTargetThicknessCmd) {
        G4double newThickness = fTargetThicknessCmd->GetNewDoubleValue(newValue);
        fDetectorConstruction->SetTargetThickness(newThickness);
    }
    
    else if (command == fTargetMaterialCmd) {
        fDetectorConstruction->SetTargetMaterial(newValue);
    }
}