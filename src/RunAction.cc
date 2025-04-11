#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>
#include <filesystem>  // For file deletion (optional)

#include "G4MTRunManager.hh"
#include "G4Threading.hh"  // For thread-related functions

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#define width 17L
#define width2 15L

GeneratorData* RunAction::fGeneratorData = nullptr;
G4Mutex RunAction::generatorMutex = G4MUTEX_INITIALIZER;

RunAction::RunAction() 
    : G4UserRunAction(), mutex()
{
    if (!fGeneratorData) {
        G4AutoLock lock(&generatorMutex);
        if (!fGeneratorData) {
            fGeneratorData = new GeneratorData();
        }
    }
    // Initialize the mutex
    G4MUTEXINIT(mutex);
}

RunAction::~RunAction()
{
    if (fOutputFile.is_open()) {
        fOutputFile.close();
    }

    G4AutoLock lock(&generatorMutex);
    if (fGeneratorData) {
        delete fGeneratorData;
        fGeneratorData = nullptr;
    }
    // Destroy the mutex
    G4MUTEXDESTROY(mutex);
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    if (G4Threading::G4GetThreadId() == 1)
    {
        const G4VUserPrimaryGeneratorAction* generatorActionBase = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
        const MyPrimaryGenerator* generatorAction = dynamic_cast<const MyPrimaryGenerator*>(generatorActionBase);

        if (generatorAction) {
            G4AutoLock lock(&generatorMutex);
            fGeneratorData->Ekin = generatorAction->GetmeanKineticEnergy();
            fGeneratorData->sigmaE = generatorAction->GetsigmaEnergy();
            fGeneratorData->sigmaX = generatorAction->GetsigmaPosx();
            fGeneratorData->isInitialized = true;

            G4cout << "Thread 1: Initialized generator data - Ekin: " 
                  << G4BestUnit(fGeneratorData->Ekin, "Energy") 
                  << " (" << (fGeneratorData->sigmaE/fGeneratorData->Ekin)*100 << "%)" 
                  << G4endl;
        }
    }

    fTotalEnergies.clear();
    fTotalPositions.clear();
    fTotalMomentums.clear();
    fTotalThetas.clear();
    fTotalPhis.clear();
    fTotalParent.clear();
    fTotalPDG.clear();

    // Get the thread ID
    G4int threadID = G4Threading::G4GetThreadId();

    // Open the output file
    G4String fileName = "scattering_data_thread_" + std::to_string(threadID) + ".txt";
    
    // Consider using a mutex for file operations if needed
    G4AutoLock lock(&mutex);
    
    fOutputFile.open(fileName);
    if (fOutputFile.is_open()) {
        fOutputFile << "Energy(MeV)\tPositionX(mm)\tPositionY(mm)\tPositionZ(mm)\tMomX(MeV/c)\tMomY(MeV/c)\tMomZ(MeV/c)\tTheta(deg.)\tPhi(deg.)\tParentID\tPDGnumber" << G4endl;
    } else {
        G4cerr << "Unable to open file: " << fileName << G4endl;
    }
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Print a summary of the run
  G4cout << "--- End of Run ---" << G4endl;
  G4cout << "Number of events: " << nofEvents << G4endl;
  G4cout << "Total scattering events: " << fTotalEnergies.size() << G4endl;


    // Write data to file
    WriteToFile();

    // Merge data across threads
    MergeScatteringData();

    // Write combined data if this is the master thread
    G4MTRunManager* runManager = dynamic_cast<G4MTRunManager*>(G4RunManager::GetRunManager());
    if (runManager && runManager->GetMasterThreadId() == std::this_thread::get_id()) {
        WriteCombinedDataToFile(run);

        // Optionally delete temporary thread-specific files
        DeleteTemporaryFiles();
    }

    // Close the output file
    if (fOutputFile.is_open()) {
        fOutputFile.close();
    }
}

void RunAction::AddScatteringData(const std::vector<G4double>& energies,
                                  const std::vector<G4ThreeVector>& positions,
                                  const std::vector<G4ThreeVector>& momentums,
                                  const std::vector<G4double>& thetas,
                                  const std::vector<G4double>& phis,
                                  const std::vector<G4int>& parentID,
                                  const std::vector<G4int>& pdgNumber)
{
    G4AutoLock lock(&mutex);
    fTotalEnergies.insert(fTotalEnergies.end(), energies.begin(), energies.end());
    fTotalPositions.insert(fTotalPositions.end(), positions.begin(), positions.end());
    fTotalMomentums.insert(fTotalMomentums.end(), momentums.begin(), momentums.end());
    fTotalThetas.insert(fTotalThetas.end(), thetas.begin(), thetas.end());
    fTotalPhis.insert(fTotalPhis.end(), phis.begin(), phis.end());
    fTotalParent.insert(fTotalParent.end(), parentID.begin(), parentID.end());
    fTotalPDG.insert(fTotalPDG.end(), pdgNumber.begin(), pdgNumber.end());
}

void RunAction::WriteToFile()
{
    if (!fOutputFile.is_open()) return;

    for (size_t i = 0; i < fTotalEnergies.size(); ++i)
    {
        fOutputFile 
                    << fTotalEnergies[i] / MeV
                    << std::setw(width) << fTotalPositions[i].x() / mm
                    << std::setw(width) << fTotalPositions[i].y() / mm
                    << std::setw(width) << fTotalPositions[i].z() / mm
                    << std::setw(width2) << fTotalMomentums[i].x() / MeV
                    << std::setw(width2) << fTotalMomentums[i].y() / MeV
                    << std::setw(width2) << fTotalMomentums[i].z() / MeV
                    << std::setw(width2) << fTotalThetas[i] / deg
                    << std::setw(width2) << fTotalPhis[i] / deg 
                    << std::setw(width2) << fTotalParent[i] 
                    << std::setw(width2) << fTotalPDG[i] << G4endl;
    }
}

void RunAction::MergeScatteringData()
{
    // Merge operation would typically be handled by Geant4's thread synchronization
    // This is a placeholder for potential custom merging logic
}

void RunAction::WriteCombinedDataToFile(const G4Run* run)
{
    //const G4RunManager* runManager = G4RunManager::GetRunManager();
    G4MTRunManager* runManager = dynamic_cast<G4MTRunManager*>(G4RunManager::GetRunManager());
    const DetectorConstruction* detectorConstruction = dynamic_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());

    G4double distance = detectorConstruction->GetVDetectorDistance();
    
    G4double thickness = detectorConstruction->GetTargetThickness();
    
    G4String detPos = G4BestUnit(distance, "Length");
    G4String detThick = G4BestUnit(thickness, "Length");
    G4String path = std::to_string(int(nofEvents/1000000))+ "Mproton/" + std::to_string(int(fGeneratorData->Ekin)) +"MeV/" + std::to_string(int(thickness/um)) 
                    + "um/" + std::to_string(int(fGeneratorData->Ekin)) +"MeV" + std::to_string(int(thickness/um)) + "um"
                    + std::to_string(int(distance/cm)) +"cm/";
    // Access the generator data safely
    G4String fileName;
    {
        G4AutoLock lock(&generatorMutex);
        if (fGeneratorData && fGeneratorData->isInitialized) {
            std::ostringstream fileNameStream;
            fileNameStream << path << "combined_data_of_" << nofEvents 
                         << "_at_t" << detThick << "_" << detPos 
                         << "_" << fGeneratorData->Ekin
                         << "MeV(" << (fGeneratorData->sigmaE/fGeneratorData->Ekin)*100 << "%)"
                         << "_sigmaX_" << fGeneratorData->sigmaX << "mm.txt";
            fileName = fileNameStream.str();
            
            G4cout << "Master thread: Using generator data - Ekin: " 
                  << fGeneratorData->Ekin << ", sigmaE: " 
                  << fGeneratorData->sigmaE << ", sigmaX: " 
                  << fGeneratorData->sigmaX << G4endl;
        } else {
            fileName = "combined_data_default.txt";
            G4cerr << "Warning: Generator data not initialized!" << G4endl;
        }
    }

    std::ofstream combinedFile(fileName);
    
    if (!combinedFile.is_open()) {
        G4cerr << "Unable to open combined file: " << fileName << G4endl;
        return;
    }

    combinedFile << "Energy(MeV)\tPositionX(mm)\tPositionY(mm)\tPositionZ(mm)\tMomX(MeV/c)\tMomY(MeV/c)\tMomZ(MeV/c)\tTheta(deg.)\tPhi(deg.)\tParentID\tPDGnumber" << G4endl;

    // Get the number of threads
    
    if (!runManager) {
        G4cerr << "Run manager is not multi-threaded!" << G4endl;
        return;
    }

    G4int numThreads = runManager->GetNumberOfThreads();

    // Loop through all thread files and combine their data
    for (G4int i = 0; i < numThreads; ++i) {
        G4String threadFileName = "scattering_data_thread_" + std::to_string(i) + ".txt";
        std::ifstream threadFile(threadFileName);

        if (!threadFile.is_open()) {
            G4cerr << "Unable to open thread file: " << threadFileName << G4endl;
            continue;
        }

        // Skip the header line of the thread file
        std::string line;
        std::getline(threadFile, line);

        // Read and write the data
        while (std::getline(threadFile, line)) {
            combinedFile << line << G4endl;
        }

        threadFile.close();
    }

    combinedFile.close();
}

void RunAction::DeleteTemporaryFiles()
{
    // Delete temporary thread-specific files
    G4MTRunManager* runManager = dynamic_cast<G4MTRunManager*>(G4RunManager::GetRunManager());
    if (runManager) {
        G4int numThreads = runManager->GetNumberOfThreads();
        
        // Delete worker thread files
        for (G4int i = -1; i < numThreads; ++i) {
            G4String fileName = "scattering_data_thread_" + std::to_string(i) + ".txt";
            if (std::filesystem::remove(fileName.c_str())) {  // Convert G4String to const char*
                G4cout << "Deleted temporary file: " << fileName << G4endl;
            } else {
                G4cerr << "Failed to delete temporary file: " << fileName << G4endl;
            }
        }
    }
}