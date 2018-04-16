#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  private:
    G4ParticleGun* fParticleGun;
    //double Energy[5022];	// # of lines of the .txt spectrum file
    //double Intensity[5022];	// has to be same number as SPECTRUM_SIZE from PrimaryGeneratorAction.hh
    double Energy[2215];        // # of lines of the .txt spectrum file
    double Intensity[2215];     // has to be same number as SPECTRUM_SIZE from PrimaryGeneratorAction.hh
    //double Energy[1731];        // # of lines of the .txt spectrum file
    //double Intensity[1731];     // has to be same number as SPECTRUM_SIZE from PrimaryGeneratorAction.hh

    G4ThreeVector particleDirection;
    G4double fSourceX;		// (x,y,z) position of source relative to (0,0,0)
    G4double fSourceY;
    G4double fSourceZ;


  public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);

    // These methods below are all ones I made myself. They will be discussed in the source code.
    void ReadSpectrum(G4String filename);
    double RejectionSample();
    void GetIsotropic();
    void SetSourcePosition(G4double centerX, G4double centerY, G4double centerZ);

    const G4ParticleGun* GetParticleGun() const
    {
      return fParticleGun;
    }
};

#endif


