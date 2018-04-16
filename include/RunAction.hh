#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    G4int fCoincidenceHits;
    G4int fHornHits;
    G4int fApertureHits;

  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void CountSuccessfulHits1() { fCoincidenceHits++; }
    void CountSuccessfulHits2() { fHornHits++; }
    void CountSuccessfulHits3() { fApertureHits++; }
};

#endif

