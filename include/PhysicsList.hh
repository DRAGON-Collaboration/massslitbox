#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VUserPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

  protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
    void AddStepMax();

private:
  void ConstructDecay();
  void ConstructEM();

};
#endif


