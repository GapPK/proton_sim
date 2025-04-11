#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "QGSP_BIC_HP.hh"
#include "G4ScoringManager.hh"
#include "DetectorConstruction.hh"

#include "PhysicsList.hh"
#include "ActionInitialization.hh"


int main(int argc,char** argv)
{
    G4UIExecutive* ui = nullptr;
    if (argc==1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // Use G4MTRunManager instead of G4RunManager
    #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    #else
    G4RunManager* runManager = new G4RunManager;
    #endif

    runManager->SetNumberOfThreads(6);
    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(new ActionInitialization());
    
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();  

    if (!ui)
    {
        //batch mode
        UImanager->ApplyCommand("/control/macroPath ./macros");
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else
    {
        // interactive mode
        UImanager->ApplyCommand("/control/execute ./macros/vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}

