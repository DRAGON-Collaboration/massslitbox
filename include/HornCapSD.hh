#ifndef HornCapSD_h
#define HornCapSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HornCapHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class HornCapSD : public G4VSensitiveDetector
{

  public:
      HornCapSD(G4String name);
      ~HornCapSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      G4int HCID;
      HornCapHitsCollection *hitsCollection;

  public:
};




#endif

