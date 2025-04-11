#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4StepPoint.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4LogicalVolume.hh"
#include "AnalysisManager.hh"
#include <vector>
#include <iostream>
#include <fstream>
#include "G4Gamma.hh"
#include <iomanip>

#define width 15L

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(), fEventAction(eventAction),  // Initialize the vector in the constructor
  fTotalProtons(0),
  fProtonHitsInTarget(0)
{
    
}

SteppingAction::~SteppingAction()
{
    
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    // Retrieve the logical volume of the current step
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int parentID = track->GetParentID();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();

    G4ParticleDefinition* particle = track->GetDefinition();
    //G4String particleName = particle->GetParticleName();
    G4int PDGnumber = particle->GetPDGEncoding();

    // Get the volume of the current step
    G4VPhysicalVolume* preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    G4VPhysicalVolume* postVolume = postStepPoint->GetTouchableHandle()->GetVolume();
        if(!postVolume){
            //G4cerr << "postVolume is null!!!" << G4endl;
            return;
        }
    static std::set<G4int> interactedProtons;

    
        // Check if the proton is entering the target volume
        if (postVolume->GetName() == "VDetector") {
            if(track->GetDefinition() == G4Gamma::GammaDefinition()){
                return;
            }else{           
                    // Record the kinetic energy of the proton at the interaction
                    G4double energy = postStepPoint->GetKineticEnergy();
                    G4ThreeVector position = postStepPoint->GetPosition();
                    G4ThreeVector momentum = track->GetMomentum(); // Get the momentum vector
                    G4double theta = track->GetMomentumDirection().theta();
                    G4double phi = track->GetMomentumDirection().phi();

                    // Check if the particle was scattered
                    //if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "CoulombScat") {
                    // Increment the number of protons scattered
                    if(energy != 0) fEventAction->AddScatteringData(energy, position, momentum, theta, phi, parentID, PDGnumber);
                    //}
                    //G4cout << "Hey!!! " << G4BestUnit(momentum, "Momentum") << " from eventID: " 
                    //       << eventID << " hit!!!" << G4endl;
                    track->SetTrackStatus(fStopAndKill);
            }    

                //}
            //}

            
        }
}