#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;

class EventAction : public G4UserEventAction
{
  private:
    RunAction* fRunAct;
    G4int hitsCollID; //
    G4int fCollID_scint[4];
    //G4int fCollID_scint[3];	// the arrays of 3 means we're using 3 crystals in detector
    G4double eDepCryst[4];
    //G4double eDepCryst[3];	// that have been declared as sensitive detectors
    
    G4int fPrintModulo;
    bool fHitOne;		// records whether or not energy was deposited in any NaI
    bool fHitTwo;
    bool fHitThree;
    bool fHitFour;
    

  public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    // these are my own methods that I made.
    void Reset();
    void CheckIncidences();
    void PrintDataToFile(int flag);	// flag will tell us which NaI we're in
};

#endif
