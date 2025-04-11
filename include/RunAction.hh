#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>
#include <fstream>
#include "G4ThreeVector.hh"

#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include <iomanip>

#include <mutex>

struct GeneratorData {
        G4double Ekin;
        G4double sigmaE;
        G4double sigmaX;
        bool isInitialized;
        
        GeneratorData() : Ekin(0), sigmaE(0), sigmaX(0), isInitialized(false) {}
    };

class G4Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);

    void AddScatteringData(const std::vector<G4double>& energies,
                           const std::vector<G4ThreeVector>& positions,
                           const std::vector<G4ThreeVector>& momentums,
                           const std::vector<G4double>& thetas,
                           const std::vector<G4double>& phis,
                           const std::vector<G4int>& parentID,
                           const std::vector<G4int>& pdgNumber);
    void WriteToFile();
    void WriteCombinedDataToFile(const G4Run* run);
    void MergeScatteringData();
    void DeleteTemporaryFiles();

private:
    
    std::vector<G4double> fTotalEnergies;
    std::vector<G4ThreeVector> fTotalPositions;
    std::vector<G4ThreeVector> fTotalMomentums;
    std::vector<G4double> fTotalThetas;
    std::vector<G4double> fTotalPhis;
    std::vector<G4int> fTotalParent;
    std::vector<G4int> fTotalPDG;

    std::ofstream fOutputFile;
    //G4Mutex mutex;
    std::mutex mutex;
    G4int nofEvents;

    static GeneratorData* fGeneratorData;
    static G4Mutex generatorMutex;
    
};

#endif

