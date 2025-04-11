#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"

class RunAction;

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddScatteringData(G4double energy, G4ThreeVector position, G4ThreeVector momentum, 
                            G4double theta, G4double phi, G4int parentID, G4int pdgNumber);

private:
    RunAction* fRunAction;
    std::vector<G4double> fEnergies;
    std::vector<G4ThreeVector> fPositions;
    std::vector<G4ThreeVector> fMomentums;
    std::vector<G4double> fTheta;
    std::vector<G4double>fPhi;
    std::vector<G4int> fParent;
    std::vector<G4int> fPDG;

};

#endif

