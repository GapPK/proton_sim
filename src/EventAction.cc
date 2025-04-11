#include "EventAction.hh"
#include "G4Event.hh"
#include "AnalysisManager.hh"
#include "RunAction.hh"

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(), fRunAction(runAction)
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event*event)
{
    fEnergies.clear();
    fPositions.clear();
    // Initialize event data collection
    AnalysisManager::Instance()->BeginEvent();
    //G4cout << "Beginning of event " << event->GetEventID() << G4endl;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    // Finalize event data collection
    AnalysisManager::Instance()->EndEvent();
    //G4cout << "End of event " << event->GetEventID() << G4endl;

    fRunAction->AddScatteringData(fEnergies, fPositions, fMomentums, fTheta, fPhi, fParent, fPDG);
}

void EventAction::AddScatteringData(G4double energy, G4ThreeVector position, G4ThreeVector momentum, G4double theta, 
                                    G4double phi, G4int parentID, G4int pdgNumber)
{
  fEnergies.push_back(energy);
  fPositions.push_back(position);
  fMomentums.push_back(momentum);
  fTheta.push_back(theta);
  fPhi.push_back(phi);
  fParent.push_back(parentID);
  fPDG.push_back(pdgNumber);
  
}
