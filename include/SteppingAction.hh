#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "EventAction.hh"

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
    

private:
    EventAction* fEventAction;
    G4int fProtonHitsInTarget;
    G4int fTotalProtons;
    
};

#endif

