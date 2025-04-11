#ifndef AnalysisManager_h
#define AnalysisManager_h 1

//#include "G4VAnalysisManager.hh"
#include "G4AnalysisManager.hh"
#include "G4String.hh"
#include "G4Threading.hh"
#include <memory>

class AnalysisManager
{
public:
    static G4ThreadLocal AnalysisManager* fgInstance;
    static AnalysisManager* Instance();
    void OpenFile(const G4String& fileName);
    void Write();
    void CloseFile();
    void BeginEvent();
    void EndEvent();
    void FillH1(G4int id, G4double value);

    

private:
    AnalysisManager();
    ~AnalysisManager(); 

    G4AnalysisManager* fAnalysisManager;
};

#endif
