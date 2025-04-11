#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4Region.hh"
#include "G4UnionSolid.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), 
    fTarget_thickness(10*um), // Default 10 micrometers
    fRegionTarget(nullptr),
    fVDetectorDistance(10.0 * cm),    // Default distance
    fVDetectorThickness(1.0 * mm),     // Default thickness
    fTargetMaterialName("G4_Au"),  // Default gold
    fTargetMaterial(nullptr)
{
    // Create messenger for interactive configuration
    fMessenger = new DetectorConstructionMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
    delete fMessenger;
}

void DetectorConstruction::SetTargetThickness(G4double thickness)
{
    if(fTarget_thickness != thickness){
        if(thickness) {
            fTarget_thickness = thickness;
            //G4RunManager::GetRunManager()->ReinitializeGeometry();
            G4cout << "----> The target thickness is now: " << G4BestUnit(thickness, "Length") << G4endl;
        }else {
        G4cout
          << G4endl
          << "-->  WARNING from SetTargetThickness : "
          << thickness << " can not be defined" << G4endl;
     }
    }
    
    
}

void DetectorConstruction::SetTargetMaterial(G4String materialName)
{
    G4NistManager* nist = G4NistManager::Instance();
    
    G4Material* newMaterial = nist->FindOrBuildMaterial(materialName);

    if (fTargetMaterial != newMaterial) {
     if ( newMaterial ) {
        fTargetMaterial = newMaterial;
        if (logicTarget) logicTarget->SetMaterial(fTargetMaterial);
        //G4RunManager::GetRunManager()->ReinitializeGeometry();
        G4cout
          << G4endl
          << "----> The target is made of " << materialName << G4endl;
     } else {
        G4cout
          << G4endl
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
    // Predefined materials from NIST database
    /*if (materialName == "Au") materialName = "G4_Au";
    else if (materialName == "Ag") materialName = "G4_Ag";
    else if (materialName == "Cu") materialName = "G4_Cu";
    
    fTargetMaterial = nist->FindOrBuildMaterial(materialName);
    fTargetMaterialName = materialName;
    
    G4RunManager::GetRunManager()->ReinitializeGeometry();*/
}

void DetectorConstruction::SetVDetectorDistance(G4double dist)
{
    //fVDetectorDistance = dist;
    // If geometry is already constructed, you might need to rebuild it
    //G4RunManager::GetRunManager()->ReinitializeGeometry();

    if(fVDetectorDistance != dist){
        if(dist) {
            fVDetectorDistance = dist;
            //G4RunManager::GetRunManager()->ReinitializeGeometry();
            G4cout << "----> The target-detector distance is now: " << G4BestUnit(dist, "Length") << G4endl;
        }else {
        G4cout
          << G4endl
          << "-->  WARNING from SetVDetectorDistance : "
          << dist << " can not be defined" << G4endl;
     }
    }
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Define materials
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* stainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    
    G4Material* worldMat = new G4Material("Vacuum", 1.0, 1.01*g/mole, 6.66612e-5*pascal, kStateGas, 298*kelvin);
    G4Material* SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

    // If no material is set, default to gold
    if (!fTargetMaterial) {
        fTargetMaterial = nist->FindOrBuildMaterial("G4_Au");
    }

    // World
    G4double world_sizeXY = 1.2*m;
    G4double world_sizeZ = 1.2*m;
    G4Box* solidWorld = new G4Box("World", world_sizeXY/2, world_sizeXY/2, world_sizeZ/2);

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");

    physWorld = new G4PVPlacement(0,                       // no rotation
                                                      G4ThreeVector(),         // at (0,0,0)
                                                      logicWorld,              // its logical volume
                                                      "World",                 // its name
                                                      0,                       // its mother  volume
                                                      false,                   // no boolean operations
                                                      0,                       // copy number
                                                      checkOverlaps);                   // checking overlaps

    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Experimental chamber
    G4double outerRadiusChamber = 250.*mm;
    G4double chamberThick = 15.*mm;
    G4double height = 160.*mm;
    G4double pipeOuterDiameter = 63.5*mm;

    G4Tubs* solidChamber = new G4Tubs("Chamber",
                                      outerRadiusChamber,
                                      outerRadiusChamber + chamberThick,
                                      height/2,
                                      0.*deg,
                                      360.*deg);

    G4Tubs* solidPipeHole = new G4Tubs("PipeHole",
                                      0.,
                                      pipeOuterDiameter/4,
                                      2*outerRadiusChamber,
                                      0.*deg,
                                      360.*deg);

    // Create two holes, one for each side of the chamber
    G4RotationMatrix* rotm_hole = new G4RotationMatrix();
    rotm_hole->rotateX(90*deg);

    G4Tubs* solidVD = new G4Tubs("VDetector", fVDetectorDistance, fVDetectorDistance + fVDetectorThickness,
                                      height/2, 0.*deg, 180.*deg);

    G4SubtractionSolid* VD_with_hole = new G4SubtractionSolid("VDetectorWithHole", 
        solidVD, solidPipeHole, rotm_hole, G4ThreeVector(0, height/2, 0));

    logicVD = new G4LogicalVolume(VD_with_hole, worldMat, "VDetector");

    physVDetector = new G4PVPlacement(rotm_hole,                         
                      G4ThreeVector(0, 0, 0),     // at (0,0,0)
                      logicVD,               // its logical volume
                      "VDetector",                  // its name
                      logicWorld,                 // its mother  volume
                      false,                      // no boolean operations
                      0,                          // copy number
                      checkOverlaps);                      // checking overlaps

    // Gold target
    G4double targetSizeXY = 5.*cm;
    
    // substrate
    G4double substrateThickness = 1.0*mm;

    G4Box* solidTarget = new G4Box("Target", targetSizeXY/2, targetSizeXY/2, fTarget_thickness/2);

    logicTarget  = new G4LogicalVolume(solidTarget, fTargetMaterial, "Target");

    G4RotationMatrix* rotateTarget = new G4RotationMatrix();
    rotateTarget->rotateY(-90.*deg);

    physTarget = new G4PVPlacement(0, //rotateTarget,
                      G4ThreeVector(0, 0, (fTarget_thickness/2)),
                      logicTarget,
                      "Target",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);

    G4VisAttributes* targetVis = new G4VisAttributes(G4Colour(0.83, 0.7, 0.2)); // Red color (RGB)
    targetVis->SetVisibility(true);  // Make sure the volume is visible
    targetVis->SetForceSolid(true);
    logicTarget->SetVisAttributes(targetVis);


    G4Box* solidSubstrate = new G4Box("GlassSubstrate", targetSizeXY/2, targetSizeXY/2, substrateThickness/2);

    logicSubstrate = new G4LogicalVolume(solidSubstrate, SiO2, "GlassSubstrate");
    physSubstrate = new G4PVPlacement(0,
                      G4ThreeVector(0, 0, (fTarget_thickness + substrateThickness/2)),
                      logicSubstrate,
                      "GlassSubstrate",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);

    G4VisAttributes* substrateVis = new G4VisAttributes(G4Colour(0.65, 0.65, 0.95));
    substrateVis->SetVisibility(true);  // Make sure the volume is visible
    substrateVis->SetForceSolid(true);
    logicSubstrate->SetVisAttributes(substrateVis);

    // Return the physical world
    return physWorld;
}
