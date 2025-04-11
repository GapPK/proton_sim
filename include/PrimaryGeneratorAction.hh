#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "DetectorConstruction.hh"

class G4ParticleGun;
class G4Event;

class MyGeneratorMessenger;

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    void GeneratePrimaries(G4Event*); // used to be virtual void

    void SetsigmaEnergy(G4double);
    void SetmeanKineticEnergy(G4double);
    void SetXposition(G4double);
    void SetYposition(G4double);
    void SetZposition(G4double);
    void SetsigmaX(G4double);
    void SetsigmaY(G4double);
    void SetMomentumX(G4double);
    void SetMomentumY(G4double);
    void SetMomentumZ(G4double);
  
    G4double GetmeanKineticEnergy(void) const;
    G4double GetsigmaEnergy(void) const;
    G4double GetsigmaPosx(void) const;
    G4double GetZposition(void) const;

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; } 

private:
  void SetDefaultPrimaryParticle();

private:
    G4ParticleGun* fParticleGun;
    MyGeneratorMessenger* gunMessenger; 
 
    G4double meanKineticEnergy;
    G4double sigmaEnergy;
    G4double X0;
    G4double Y0;
    G4double Z0;

    G4double sigmaX;
    G4double sigmaY;

    G4double momentumX;
    G4double momentumY;
    G4double momentumZ;

};

#endif


