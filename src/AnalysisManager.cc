#include "AnalysisManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal AnalysisManager* AnalysisManager::fgInstance = 0;

AnalysisManager* AnalysisManager::Instance()
{
    //static AnalysisManager instance;
    //return &instance;
    if (!fgInstance)
        fgInstance = new AnalysisManager();
    return fgInstance;
}

AnalysisManager::AnalysisManager()
{
    fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager->SetDefaultFileType("root");
    fAnalysisManager->CreateH1("Edep", "Energy Deposition", 20000, 0., 20.*MeV);
}

AnalysisManager::~AnalysisManager()
{
}

void AnalysisManager::OpenFile(const G4String& fileName)
{
    //G4String fullFileName = fileName + ".root";
    //fAnalysisManager->OpenFile(fileName);

    if (fAnalysisManager->IsOpenFile()) {
        G4cout << "Warning: A file is already open. Closing it before opening a new one." << G4endl;
        CloseFile();
    }
    G4String fullFileName = fileName + ".root";
    fAnalysisManager->OpenFile(fullFileName);
}

void AnalysisManager::Write()
{
    fAnalysisManager->Write();
}

void AnalysisManager::CloseFile()
{
    //fAnalysisManager->CloseFile();
    if (fAnalysisManager->IsOpenFile()) {
        fAnalysisManager->Write();
        fAnalysisManager->CloseFile();
    }
}

void AnalysisManager::BeginEvent()
{
    //fAnalysisManager->SetH1Activation(0, true);
    fAnalysisManager->FillH1(0, 0.); // Reset histogram at the beginning of each event
}

void AnalysisManager::EndEvent()
{
    //fAnalysisManager->SetH1Activation(0, false);
}

void AnalysisManager::FillH1(G4int id, G4double value)
{
    fAnalysisManager->FillH1(id, value);
}
