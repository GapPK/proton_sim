#include "PhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4NeutronTrackingCut.hh"

PhysicsList::PhysicsList()
{
    // Add Decay Physics
    RegisterPhysics(new G4DecayPhysics());

    // Add Radioactive Decay Physics
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // Add Standard EM Physics
    //RegisterPhysics(new G4EmStandardPhysics());
    //RegisterPhysics(new G4EmStandardPhysics_option1());
    //RegisterPhysics(new G4EmStandardPhysics_option2());
    //RegisterPhysics(new G4EmStandardPhysics_option3());
    RegisterPhysics(new G4EmStandardPhysics_option4());

    // Add Hadronic Physics
    RegisterPhysics(new G4HadronElasticPhysics());
    RegisterPhysics(new G4HadronPhysicsQGSP_BERT());
    RegisterPhysics(new G4IonPhysics());
    RegisterPhysics(new G4StoppingPhysics());
    RegisterPhysics(new G4NeutronTrackingCut());
}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
