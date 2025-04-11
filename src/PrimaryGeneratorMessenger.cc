#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

MyGeneratorMessenger::MyGeneratorMessenger(MyPrimaryGenerator* MyGun)
:fPrimaryGen(MyGun)
{ 
  //
  // Definition of the interactive commands to modify the parameters of the
  // generation of primary particles
  // 
 beamParametersDir = new G4UIdirectory("/beam/");
 beamParametersDir -> SetGuidance("set parameters of beam");
 
 EnergyDir = new G4UIdirectory("/beam/energy/");  
 EnergyDir -> SetGuidance ("set energy of beam");  

 particlePositionDir = new G4UIdirectory("/beam/position/");  
 particlePositionDir -> SetGuidance ("set position of particle");  

 meanKineticEnergyCmd = new G4UIcmdWithADoubleAndUnit("/beam/energy/meanEnergy",this);
 meanKineticEnergyCmd -> SetGuidance("set mean Kinetic energy");
 meanKineticEnergyCmd -> SetParameterName("Energy",false);
 meanKineticEnergyCmd -> SetDefaultUnit("MeV");
 meanKineticEnergyCmd -> SetUnitCandidates("eV keV MeV GeV TeV");
 meanKineticEnergyCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);   
 
 sigmaEnergyCmd = new G4UIcmdWithADoubleAndUnit("/beam/energy/sigmaEnergy",this);
 sigmaEnergyCmd -> SetGuidance("set sigma energy");
 sigmaEnergyCmd -> SetParameterName("Energy",false);
 sigmaEnergyCmd -> SetDefaultUnit("keV");
 sigmaEnergyCmd -> SetUnitCandidates("eV keV MeV GeV TeV");
 sigmaEnergyCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);   
 
 XpositionCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Xposition",this);
 XpositionCmd -> SetGuidance("set x coordinate of particle");
 XpositionCmd -> SetParameterName("position",false);
 XpositionCmd -> SetDefaultUnit("mm");
 XpositionCmd -> SetUnitCandidates("mm cm m");
 XpositionCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);   

 YpositionCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Yposition",this);
 YpositionCmd -> SetGuidance("set y coordinate of particle");
 YpositionCmd -> SetParameterName("position",false);
 YpositionCmd -> SetDefaultUnit("mm");
 YpositionCmd -> SetUnitCandidates("mm cm m");
 YpositionCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

 ZpositionCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Zposition",this);
 ZpositionCmd -> SetGuidance("set z coordinate of particle");
 ZpositionCmd -> SetParameterName("position",false);
 ZpositionCmd -> SetDefaultUnit("mm");
 ZpositionCmd -> SetUnitCandidates("mm cm m");
 ZpositionCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);    

 sigmaXCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Xposition/sigmaX",this);
 sigmaXCmd -> SetGuidance("set sigma x");
 sigmaXCmd -> SetParameterName("position",false);
 sigmaXCmd -> SetDefaultUnit("mm");
 sigmaXCmd -> SetUnitCandidates("mm cm m");
 sigmaXCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);   

 sigmaYCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Yposition/sigmaY",this);
 sigmaYCmd -> SetGuidance("set sigma y");
 sigmaYCmd -> SetParameterName("position",false);
 sigmaYCmd -> SetDefaultUnit("mm");
 sigmaYCmd -> SetUnitCandidates("mm cm m");
 sigmaYCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);  

 G4UIcmdWithADouble* momentumXCmd;
 G4UIcmdWithADouble* momentumYCmd;
 G4UIcmdWithADouble* momentumZCmd;

 momentumXCmd = new G4UIcmdWithADouble("/beam/momentum/momentumX", this);
 momentumYCmd = new G4UIcmdWithADouble("/beam/momentum/momentumY", this);
 momentumZCmd = new G4UIcmdWithADouble("/beam/momentum/momentumZ", this);

}

MyGeneratorMessenger::~MyGeneratorMessenger()
{
  delete beamParametersDir;
  delete EnergyDir;
  delete meanKineticEnergyCmd;  
  delete sigmaEnergyCmd;
  delete particlePositionDir;
  delete XpositionCmd; 
  delete YpositionCmd;
  delete ZpositionCmd; 
  delete sigmaXCmd; 
  delete sigmaYCmd; 
 
}  

void MyGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)                
{
  if ( command == meanKineticEnergyCmd )                                                                        
    { fPrimaryGen -> SetmeanKineticEnergy(meanKineticEnergyCmd
				-> GetNewDoubleValue(newValue));}

  if ( command == sigmaEnergyCmd )                                                                        
    { fPrimaryGen -> SetsigmaEnergy(sigmaEnergyCmd
			    -> GetNewDoubleValue(newValue));}

  if ( command == XpositionCmd )                                                                        
    { 
      fPrimaryGen -> SetXposition(XpositionCmd->GetNewDoubleValue(newValue));
    }

  if ( command == YpositionCmd )                                                                        
    { 
      fPrimaryGen -> SetYposition(YpositionCmd->GetNewDoubleValue(newValue));
    }

  if ( command == ZpositionCmd )                                                                        
    { 
      fPrimaryGen -> SetZposition(ZpositionCmd->GetNewDoubleValue(newValue));
    }

  if ( command == sigmaXCmd )                                                                        
    { fPrimaryGen -> SetsigmaX(sigmaXCmd
				-> GetNewDoubleValue(newValue));}

  if ( command == sigmaYCmd )                                                                        
    { fPrimaryGen -> SetsigmaY(sigmaYCmd
				-> GetNewDoubleValue(newValue));}

  if (command == momentumXCmd) 
    {
      fPrimaryGen->SetMomentumX(momentumXCmd->GetNewDoubleValue(newValue));
    }
  
  if (command == momentumYCmd) 
    {
      fPrimaryGen->SetMomentumY(momentumYCmd->GetNewDoubleValue(newValue));
    }
  
  if (command == momentumZCmd) 
    {
      fPrimaryGen->SetMomentumZ(momentumZCmd->GetNewDoubleValue(newValue));
    }

}


