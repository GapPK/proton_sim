#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Region.hh"

class G4LogicalVolume;
class DetectorConstructionMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    // New methods for target configuration
    void SetTargetThickness(G4double thickness);
    void SetTargetMaterial(G4String materialName);

    inline G4double GetTargetThickness()const {return fTarget_thickness;}
    inline G4double GetVDetectorDistance() const {return fVDetectorDistance;}

    // New methods for VDetector configuration
    void SetVDetectorDistance(G4double dist);

private:
    G4bool checkOverlaps = true;

    G4LogicalVolume *logicTarget, *logicWorld, *logicVD, *logicSubstrate; 

    G4VPhysicalVolume *physChamber, *physTarget, *physVDetector, *physCase, *physSubstrate, *physWorld; 

    G4double fTarget_thickness;
    G4Region* fRegionTarget;

    // Messenger for interactive configuration
    DetectorConstructionMessenger* fMessenger;

    // VDetector parameters
    G4double fVDetectorDistance;
    G4double fVDetectorThickness;

    // Target-specific parameters
    G4String fTargetMaterialName;
    G4Material* fTargetMaterial;

};

#endif

