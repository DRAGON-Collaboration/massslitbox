#include "PrimaryGeneratorAction.hh"    // Debra's #include packages.
#include <stdio.h>			// Likely not very useful as I'm basically recoding the whole thing.
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "G4RunManager.hh"      // These were my old packages. They seem useful but are probably defined in
#include "G4Event.hh"           // PhysicsList. No matter though, I'll leave them here just in case,
#include "G4ParticleGun.hh"     // as I suspect they're necessary and I don't know where they need to be.
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "global.hh"
#include <cmath>
#include "G4RandomDirection.hh"//

using	 namespace	std;

//#define	SPECTRUM_FILE_NAME	"38K_GS_Spectra.txt"	// name of the input beta energy spectrum

// Number of lines in the spectrum file. Must be changed in the .hh file as well
//#define	SPECTRUM_SIZE		2723	// 38K
//#define SPECTRUM_SIZE           1731  // 15O  
#define SPECTRUM_SIZE           2215  // 19Ne 

extern G4double NaI3_pos_x;
extern G4double NaI3_pos_y;
extern G4double NaI3_pos_z;

extern G4String betaSpectrumName;

extern G4double apexAngle;
extern G4double pe;
extern G4double sourceX;
extern G4double sourceY;
extern G4double sourceZ;
extern G4double sourceDepth;


extern G4double dir_X;
extern G4double dir_Y;
extern G4double dir_Z;
 

extern int particleTypeFlag;

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4String particleType;
  G4int n_particle;

  if(particleTypeFlag == 1)     // means we have a beta
  {
    ReadSpectrum(betaSpectrumName);     // reads an input .txt file, saves info to class property arrays.
                                        // Used to get energy distribution spectrum of betas.
    particleType = "e+";
    n_particle = 1;
  }
  if(particleTypeFlag == 2)
  {
    particleType = "gamma";
    n_particle = 1;
  }
  if(particleTypeFlag == 3) // positron with 1eV energy
  {
    particleType = "e+";
    n_particle = 1;
  }
  if(particleTypeFlag == 4) // two 511keV gamma-rays
  {
    particleType = "gamma";
    n_particle = 1;
  }

  fParticleGun  = new G4ParticleGun(n_particle);

  // setting species of particles, which can be set in the InputParameters.txt file.
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fParticleGun -> SetParticleDefinition(particleTable->FindParticle(particleName= particleType));
	// remaining properties will be set in GeneratePrimaries method.
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{


  //fSourceX = 1*cm;				// Debugging purposes. These geometry parameters fires the particle
  //fSourceY = 40.66*cm;				// directly on NaI2.
  //fSourceY = 0*cm;
  //fSourceZ = -5.01*cm;
  //fSourceZ = -0.5*cm;
  //particleDirection = G4ThreeVector(1, 0, 0);


//  SetSourcePosition(0*cm, 0.5*cm, -0.5*cm - 0.8*cm);	// This is hard-coded; the center of the CALIBRATION source
							// This was ~true center for Co-60 source used.
							// Once calibration is done, can delete.

/*  fSourceX = 0*cm;	// hard coded source position for the Cs-137 source, assumed to be a point source.
  fSourceY = 0*cm;	// To first order this should be ok. Only needed for calibration.
  fSourceZ = -0.5*cm - 1.3*cm; */

/*  fSourceX = 0*cm;	// hard coded source position for Na-22 source measurement run on Aug 20, 2014.
  fSourceY = 0.8*cm;	// Only needed for calibration.
  fSourceZ = -0.5*cm - 1.0*cm; */

  G4double depth = sourceDepth;                 // depth of particle implantation into the slit
  G4double particleEnergy;

  if(particleTypeFlag == 1)
  {
    fSourceX = sourceX;                           
    fSourceY = sourceY;
    fSourceZ = sourceZ + depth; 
    particleEnergy = RejectionSample()*MeV;       // randomly sample from prob. dist. (which gives E in MeV)
    
    GetIsotropic();

    fParticleGun -> SetParticlePosition(G4ThreeVector(fSourceX, fSourceY, fSourceZ));
    fParticleGun -> SetParticleEnergy(particleEnergy);
    fParticleGun -> SetParticleMomentumDirection(particleDirection);
    fParticleGun -> GeneratePrimaryVertex(anEvent);
  }
  if(particleTypeFlag == 2)
  {
    fSourceX = sourceX;
    fSourceY = sourceY;
    fSourceZ = sourceZ + depth;
    particleEnergy = pe;

    GetIsotropic();

    fParticleGun -> SetParticlePosition(G4ThreeVector(fSourceX, fSourceY, fSourceZ));
    fParticleGun -> SetParticleEnergy(particleEnergy);
    fParticleGun -> SetParticleMomentumDirection(particleDirection); 
    fParticleGun -> GeneratePrimaryVertex(anEvent);
  }
  if(particleTypeFlag == 3)
  {
    fSourceX = sourceX;
    fSourceY = sourceY;
    fSourceZ = sourceZ;
    particleEnergy = 1*eV;

    GetIsotropic(); 

    fParticleGun -> SetParticlePosition(G4ThreeVector(fSourceX, fSourceY, fSourceZ));
    fParticleGun -> SetParticleEnergy(particleEnergy);
    fParticleGun -> SetParticleMomentumDirection(G4RandomDirection());
    fParticleGun -> GeneratePrimaryVertex(anEvent);
  }
  if(particleTypeFlag == 4)
  {
    fSourceX = sourceX;
    fSourceY = sourceY;
    fSourceZ = sourceZ;
    particleEnergy = 511*keV;

    GetIsotropic();    

    fParticleGun -> SetParticlePosition(G4ThreeVector(fSourceX, fSourceY, fSourceZ));
    fParticleGun -> SetParticleEnergy(particleEnergy);
    fParticleGun -> SetParticleMomentumDirection(particleDirection);
    fParticleGun -> GeneratePrimaryVertex(anEvent);
  
    fParticleGun -> SetParticlePosition(G4ThreeVector(fSourceX, fSourceY, fSourceZ));
    fParticleGun -> SetParticleEnergy(particleEnergy);
    fParticleGun -> SetParticleMomentumDirection(-particleDirection);
    fParticleGun -> GeneratePrimaryVertex(anEvent);
  }


                                                // set the class direction vector to an isotropic vector
                                                // I'm using class properties because I don't want to use pointers
                                                // GetIsotropic() uses class properties fSource_ set in the previous method.
}

// This method just reads the input spectrum and saves it to a class property array.
void PrimaryGeneratorAction::ReadSpectrum(G4String filename)
{
  int energy = 0;	// first column of .txt file (will be in keV in my spectrum)
  int intensity = 0;	// second column of spectrum .txt file
  int lineCounter = 1;	// keeps track of which line we're at

  string buf;
  ifstream infile;
  cout << "The file being opened is: " << filename << endl;
  infile.open(filename);

  if(!infile.is_open())	// a check to see if the file opened correctly
    cout << "Problem opening: " << filename << endl;

  while(getline(infile, buf))
  {
    istringstream bufstream(buf);
    if(!bufstream.eof())                //inputs our values from .txt file.
    {
      bufstream >> energy >> intensity;	// In MY spectrum, the energies are keV
    }
  Energy[lineCounter - 1] = energy;	// since lineCounter indexes at 1, need to do -1 to get proper indexing in array
  Intensity[lineCounter - 1] = intensity;

  lineCounter++;			// increments
  }

  cout << "Closing " << filename << ". Done inputting spectrum probability distribution." << endl;
  infile.close();
}

// Performs a rejection sample to get a value of energy described by the probability distribution
double PrimaryGeneratorAction::RejectionSample()
{
  int sizeSpectrum = SPECTRUM_SIZE;
  int max = 0;          // On my computer, int's are saved as 32-bit. That is enough to go up to 4 billion.
                        // Which can store all the numbers in the spectrum. Since you cannot random sample with
                        // double's, you need to ensure that your spectrum is given in int's. To ensure that you
                        // don't lose precision, multiply by a factor of 10^n s.t. you don't have any 0 entries.
  for(int i = 0; i < sizeSpectrum; i++) // loop across size of Energy and Intensity to find the max value of Intensity
  {
    if(Intensity[i] >= max)
    {
      max = Intensity[i];       // max value of hits in beta spectrum.
    }
  }

  int EMax, EMin, r1, r2, randIntense;
  bool goodSample = false;
  EMin = 0;                     // first line in array betaenergy
  EMax = sizeSpectrum - 1;      // last line in array betaenergy - 1
  while(!goodSample)
  {
    r1 = rand() % EMax + 1;     // Random sample an energy value
    r2 = rand() % max + 1;      // Random sample an intensity value
    randIntense = Intensity[r1];// takes value of spectrum's insensity at a random energy
    if (r2 <= randIntense)      // random sampling happens until it picks something inside the prob. distribution
      goodSample = true;
  }

  return 0.001*r1;              // return the sampled energy (MeV)
}

// Create an isotropic vector in a cone of some apex angle and save it as class property particleDirection
void PrimaryGeneratorAction::GetIsotropic()
{
  G4ThreeVector newUz;
  G4double theta, phi, apex;


// NaI3 Detector Direction (Used for particleTypeFlag 1 & 2)
  //G4double deltaX = NaI3_pos_x - fSourceX;
  //G4double deltaY = NaI3_pos_y - fSourceY;
  //G4double deltaZ = NaI3_pos_z - fSourceZ;

// NaI2 Detector Direction (Used for particleTypeFlag 3 & 4)
  //G4double deltaX = 19.15*cm - fSourceX;
  //G4double deltaY = 40.66*cm - fSourceY;
  //G4double deltaZ = -5.01*cm - fSourceZ;

// Direction (Used for all particleTypeFlags)
  G4double deltaX = dir_X - fSourceX;
  G4double deltaY = dir_Y - fSourceY;
  G4double deltaZ = dir_Z - fSourceZ; 

  // phi and theta below are set so that they point from the source to the pmt
  phi = (180 + atan(deltaY/deltaX)*(180.0/3.1416))*deg;	// basic geometry and then converted to degrees.
  theta = acos(deltaZ/sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ))*(180.0/3.1416)*deg;

  apex = apexAngle;	// 45 degrees was what I used for debugging
			// 180 degrees gives full isotropic firing.

  newUz = G4ThreeVector(std::sin(theta)*std::cos(phi), std::sin(theta)*std::sin(phi), std::cos(theta));

  // All of this code below, including comments, was lifted directly from an out-of-the-box
  // geant4 example. It's sole purpose was to fire events isotropically in a shape. It included
  // the option to fire in a cone of defined apex angle. It works so leave it.

  //particle direction uniform around fNewUz axis
  //1- in World frame
  //cosAlpha uniform in [cos(0), cos(fAlphaMax)]
  G4double cosAlpha = 1. - G4UniformRand()*(1.- std::cos(apex));
  G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
  G4double psi      = twopi*G4UniformRand();  //psi uniform in [0, 2*pi]
  G4ThreeVector dir(sinAlpha*std::cos(psi), sinAlpha*std::sin(psi), cosAlpha);
  //2- rotate dir   (rotateUz transforms uz to fNewUz)
  dir.rotateUz(newUz);


  particleDirection = dir;

}

// ****** This method is not useful for actual simulation. It was used in Co-60 calibrations ******
void PrimaryGeneratorAction::SetSourcePosition(G4double centerX, G4double centerY, G4double centerZ)
{
  int r1, r2, r3;	// the random numbers we will generate
  G4double randAngle, randRadius, randHeight;	// The variables defined by random numbers with units.

  G4double radius = 0.32*cm;	// measured diameter (divided by 2) and length of the barrel of the Co-60 source
  G4double length = 1.94*cm;

  r1 = rand() % 194 + 1;	// picks a random number along the length. Numbers found from measurements given above.
  r2 = rand() % 32 + 1;		// picks a random number along the radius.
  r3 = rand() % 360 + 1;	// picks a random angle.

  randAngle = r3*deg;
  randRadius = (r2/100.0)*cm;
  randHeight = (r1/100.0)*cm - length/2.0;

  fSourceZ = centerZ + randRadius*cos(randAngle);
  fSourceY = centerY + randRadius*sin(randAngle);
  fSourceX = centerX + randHeight;
}
