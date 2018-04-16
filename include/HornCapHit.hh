#ifndef HornCapHit_h
#define HornCapHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class HornCapHit : public G4VHit
{
  public:

      HornCapHit();
      ~HornCapHit();
      G4int operator==(const HornCapHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      void Draw();
      void Print();

  private:
      G4int         stripNo;
      G4ThreeVector position;
      G4ThreeVector momentum;
      G4double      energy;
      G4ParticleDefinition* particle;

  public:
      inline void SetStripNo(G4int strip)
      { stripNo=strip; }
      inline G4int GetStripNo()
      { return stripNo; }
      inline void SetPosition(G4ThreeVector pos)
      { position=pos; }
      inline G4ThreeVector GetPosition()
      { return position; }
      inline void SetMomentum(G4ThreeVector mom)
      { momentum = mom; }
      inline G4ThreeVector GetMomentum()
      { return momentum; }
      inline void SetEnergy(G4double ene)
      { energy = ene; }
      inline G4double GetEnergy()
      { return energy; }
      inline void SetParticle(G4ParticleDefinition* pdef)
      { particle = pdef; }
      inline G4ParticleDefinition* GetParticle()
      { return particle; }

};

typedef G4THitsCollection<HornCapHit> HornCapHitsCollection;

extern G4Allocator<HornCapHit> HornCapHitAllocator;

inline void* HornCapHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) HornCapHitAllocator.MallocSingle();
  return aHit;
}

inline void HornCapHit::operator delete(void *aHit)
{
  HornCapHitAllocator.FreeSingle((HornCapHit*) aHit);
}

#endif
