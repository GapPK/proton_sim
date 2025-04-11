#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "globals.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    //Define the messenger
    gunMessenger = new MyGeneratorMessenger(this);

    //G4int n_particle = 1;
    fParticleGun = new G4ParticleGun();

    SetDefaultPrimaryParticle();
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
    delete gunMessenger;

    //G4cout << "MyPrimaryGeneratorAction deleted" << G4endl;
}

void MyPrimaryGenerator::SetDefaultPrimaryParticle()
{
    // Default primary particle

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");
    fParticleGun->SetParticleDefinition(particle);
    //fParticleGun->SetParticleEnergy(220*MeV);

    G4double defaultMeanKineticEnergy = 70 * CLHEP::MeV;   
    meanKineticEnergy = defaultMeanKineticEnergy;

    G4double defaultsigmaEnergy = 0 * CLHEP::MeV;   
    sigmaEnergy = defaultsigmaEnergy;

    G4double defaultX0 = 0.0 * CLHEP::mm; 
    X0 = defaultX0;
    G4double defaultY0 = 0.0 * CLHEP::mm;
    Y0 = defaultY0;
    G4double defaultZ0 = -250. * CLHEP::mm; //the beam propagates in the z direction from half of extraction tube
    Z0 = defaultZ0;

    G4double defaultsigmaX = 4.0 * CLHEP::mm; // initial sigma to get sigma output 3.5 mm of proton 220 MeV
    sigmaX = defaultsigmaX;
    G4double defaultsigmaY = 4.0 * CLHEP::mm;
    sigmaY = defaultsigmaY;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    G4ThreeVector mom(0.,0.,1.);
    fParticleGun->SetParticleMomentumDirection(mom);

    
    // ********************************************
    // Set the position of the primary particles
    // ********************************************
    G4double x = X0; //the beam has a gaussian shape
    G4double y = Y0;
    G4double z = Z0;

    if (sigmaX > 0.0) {
        x += G4RandGauss::shoot(X0, sigmaX);
    }

    if (sigmaY > 0.0) {
        y += G4RandGauss::shoot(Y0, sigmaY);
    }

    fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
    //G4cout << "x: " << x << " y: " << y << " z: " << z << G4endl;

    // ********************************************
    // Set the beam energy and energy spread
    // ********************************************
    G4double kineticEnergy = 0;

    /*if (sigmaEnergy > 0) {
        kineticEnergy += G4RandGauss::shoot( meanKineticEnergy, sigmaEnergy );
    }*/

    kineticEnergy += G4RandGauss::shoot( meanKineticEnergy, sigmaEnergy );

    fParticleGun->SetParticleEnergy ( kineticEnergy );
    //G4cout << "Mean energy: " << kineticEnergy << G4endl;
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

void MyPrimaryGenerator::SetmeanKineticEnergy (G4double val )  
{
  meanKineticEnergy = val;
  //G4cout << "The mean Kinetic energy of the incident beam has been changed to (MeV):" << meanKineticEnergy/MeV << G4endl; 
} 

void MyPrimaryGenerator::SetsigmaEnergy (G4double val )  
{ 
 sigmaEnergy = val;
 //G4cout << "The sigma of the kinetic energy of the incident beam has been changed to (MeV):" << sigmaEnergy/MeV << G4endl; 
}

void MyPrimaryGenerator::SetXposition (G4double val )  
{ 
    X0 = val;
    //G4cout << "The x-position of the beam particle has been changed to (mm):" << X0/mm << G4endl;
}

void MyPrimaryGenerator::SetYposition (G4double val )  
{ 
    Y0 = val;
    //G4cout << "The y-position of the beam particle has been changed to (mm):" << Y0/mm << G4endl;
}

void MyPrimaryGenerator::SetZposition (G4double val )  
{ 
    Z0 = val;
    //G4cout << "The z-position of the beam particle has been changed to (mm):" << Z0/mm << G4endl;
}

void MyPrimaryGenerator::SetsigmaX (G4double val )  
{ sigmaX = val;
  //G4cout << "The sigma x of the beam size has been changed to (mm):" << sigmaX/mm << G4endl; 
}

void MyPrimaryGenerator::SetsigmaY (G4double val )  
{ sigmaY = val;
  //G4cout << "The sigma y of the beam size has been changed to (mm):" << sigmaY/mm << G4endl;
}

void MyPrimaryGenerator::SetMomentumX(G4double val) 
{ 
    momentumX = val; 
    G4cout << "Momentum X set to: " << momentumX << G4endl; 
}

void MyPrimaryGenerator::SetMomentumY(G4double val) 
{ 
    momentumY = val; 
    G4cout << "Momentum Y set to: " << momentumY << G4endl; 
}

void MyPrimaryGenerator::SetMomentumZ(G4double val) 
{ 
    momentumZ = val; 
    G4cout << "Momentum Z set to: " << momentumZ << G4endl; 
}

G4double MyPrimaryGenerator::GetmeanKineticEnergy(void) const
{
    return meanKineticEnergy;
}

G4double MyPrimaryGenerator::GetsigmaEnergy(void) const
{
    return sigmaEnergy;
}

G4double MyPrimaryGenerator::GetsigmaPosx(void) const
{
    return sigmaX;
}

