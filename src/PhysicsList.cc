#include "PhysicsList.hh"
#include "G4ParticleTypes.hh"	// These things until the break are Debra's packages.
#include "G4ProcessManager.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

PhysicsList::PhysicsList()
{;}

PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.
  G4Geantino::GeantinoDefinition();
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  // Define transportation process
  AddTransportation();
  ConstructEM();
  ConstructDecay();
  AddStepMax();
}

// gamma
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"

// e+
#include "G4eplusAnnihilation.hh"

// e-
#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

// msc models
#include "G4UrbanMscModel93.hh"
#include "G4WentzelVIModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4CoulombScattering.hh"

// hadrons
#include "G4hMultipleScattering.hh"
#include "G4MscStepLimitType.hh"

#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4alphaIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

// particles
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

// Other shit.

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"

void PhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    G4double LivermoreHighEnergyLimit = GeV;

    if (particleName == "gamma") {

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4LivermorePhotoElectricModel* theLivermorePhotoElectricModel = 
	new G4LivermorePhotoElectricModel();
      theLivermorePhotoElectricModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0, theLivermorePhotoElectricModel);
      pmanager->AddDiscreteProcess(thePhotoElectricEffect);

      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4LivermoreComptonModel* theLivermoreComptonModel = 
	new G4LivermoreComptonModel();
      theLivermoreComptonModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theComptonScattering->AddEmModel(0, theLivermoreComptonModel);
      pmanager->AddDiscreteProcess(theComptonScattering);

      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4LivermoreGammaConversionModel* theLivermoreGammaConversionModel = 
	new G4LivermoreGammaConversionModel();
      theLivermoreGammaConversionModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theGammaConversion->AddEmModel(0, theLivermoreGammaConversionModel);
      pmanager->AddDiscreteProcess(theGammaConversion);

      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      G4LivermoreRayleighModel* theRayleighModel = new G4LivermoreRayleighModel();
      theRayleighModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theRayleigh->AddEmModel(0, theRayleighModel);
      pmanager->AddDiscreteProcess(theRayleigh);

    } else if (particleName == "e-") {

      G4eMultipleScattering* msc = new G4eMultipleScattering();
      //msc->AddEmModel(0, new G4UrbanMscModel93());
      msc->AddEmModel(0, new G4GoudsmitSaundersonMscModel());
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,                   -1, 1, 1);

      // Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4LivermoreIonisationModel* theIoniLivermore = new
        G4LivermoreIonisationModel();
      theIoniLivermore->SetHighEnergyLimit(1*MeV); 
      eIoni->AddEmModel(0, theIoniLivermore, new G4UniversalFluctuation() );
      eIoni->SetStepFunction(0.2, 100*um);
      pmanager->AddProcess(eIoni,                 -1, 2, 2);

      // Bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4LivermoreBremsstrahlungModel* theBremLivermore = new
        G4LivermoreBremsstrahlungModel();
      theBremLivermore->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      eBrem->AddEmModel(0, theBremLivermore);
      pmanager->AddProcess(eBrem, -1,-3, 3);

    }  else if (particleName == "e+") {

      // Identical to G4EmStandardPhysics_option3
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      //msc->AddEmModel(0, new G4UrbanMscModel93());
      msc->AddEmModel(0, new G4GoudsmitSaundersonMscModel());
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,                   -1, 1, 1);

      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetStepFunction(0.2, 100*um);

      pmanager->AddProcess(eIoni,                 -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung, -1,-3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation,0,-1, 4);

    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon
      pmanager->AddProcess(new G4MuMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation,       -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,   -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction,   -1, 4, 4);

    } else if( particleName == "proton" ||
               particleName == "pi-" ||
               particleName == "pi+"    ) {
      //proton
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);

    } else if( particleName == "alpha" || 
	       particleName == "He3" )     {
      //alpha
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);

    } else if( particleName == "GenericIon" ) {

      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);

      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 10*um);
      pmanager->AddProcess(ionIoni,                   -1, 2, 2);
      pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);

    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      pmanager->AddProcess(new G4hMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,        -1, 2, 2);
    }
  }
}

#include "G4Decay.hh"

void PhysicsList::ConstructDecay()
{
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  ////G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();

  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (particle->GetPDGCharge() != 0.0)
        {
  pmanager ->AddDiscreteProcess(stepLimiter);
  ////pmanager ->AddDiscreteProcess(userCuts);
        }
  }
}

void PhysicsList::SetCuts()
{
  // suppress error messages even in case e/gamma/proton do not exist
  G4int temp = 0; /*GetVerboseLevel();	HERE I hardcode the verbosity level to 0*/
  SetVerboseLevel(0);
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  SetCutsWithDefault();

  // Retrieve verbose level
  SetVerboseLevel(temp);
}
