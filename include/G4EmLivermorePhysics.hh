#ifndef G4EmLivermorePhysics_h
#define G4EmLivermorePhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

// Not necessary to change anything here. Most likely taken from a database and the other person
// just needed the extra physics packages.
class G4EmLivermorePhysics : public G4VPhysicsConstructor
{
public:

  G4EmLivermorePhysics(G4int ver = 1);

  // obsolete
  G4EmLivermorePhysics(G4int ver, const G4String& name);

  virtual ~G4EmLivermorePhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;	// This is the class property verbosity. Make sure it's set to 0 or 1 in the code.
};

#endif







