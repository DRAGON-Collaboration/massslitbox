#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "global.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <time.h>
#include <math.h> 
#include <cmath> 

using 	namespace 	std;

extern G4String filename1;
extern G4String filename2;
extern G4String filename3;
extern G4String filename4;
extern G4String filename5;
extern G4double apexAngle;

RunAction::RunAction()
 : G4UserRunAction(),
   fCoincidenceHits(0),
   fHornHits(0),
   fApertureHits(0)
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
  // Sets random engine seed to current time. This will change so long as each run takes longer than 1 second, which it always will.
  G4int seed = time(NULL);
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);	// Choose the Random engine
  CLHEP::HepRandom::setTheSeed(seed);

  ofstream outfile1;						// pipes in a header line
  outfile1.open(filename1, ios::app);
  outfile1 << "Energy (keV) \n";
  outfile1.close();
  ofstream outfile2;
  outfile2.open(filename2, ios::app);
  outfile2 << "Energy (keV) \n";
  outfile2.close();
  ofstream outfile3;
  outfile3.open(filename3, ios::app);
  outfile3 << "E in NaI1 (keV) \t E in NaI2 (keV) \n";
  outfile3.close();
  ofstream outfile4;
  outfile4.open(filename4, ios::app);
  outfile4 << "Energy (keV) \n";
  outfile4.close();
  ofstream outfile5;
  outfile5.open(filename5, ios::app);
  outfile5 << "X-Position (cm) \t Y-Position (cm) \t Z-Position (cm) \n";
  outfile5.close();

  fCoincidenceHits = 0;
  fHornHits = 0;
  fApertureHits = 0;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4cout
     << "\n--------------------Begin of Run------------------------------\n"
     << G4endl;

}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int NbOfEvents = run -> GetNumberOfEvent();
  if (NbOfEvents == 0)
    return;

  ofstream outfile1;			// writes the number of events simulated at end
  outfile1.open(filename1, ios::app);
  outfile1 << "Total number of events simulated: " << NbOfEvents << "\n";
  outfile1.close();
  ofstream outfile2;
  outfile2.open(filename2, ios::app);
  outfile2 << "Total number of events simulated: " << NbOfEvents << "\n";
  outfile2.close();
  ofstream outfile3;
  outfile3.open(filename3, ios::app);
  outfile3 << "Total number of events simulated: " << NbOfEvents << "\n";
  outfile3.close();
  ofstream outfile4;
  outfile4.open(filename4, ios::app);
  outfile4 << "Total number of events simulated: " << NbOfEvents << "\n";
  outfile4.close();
  ofstream outfile5;
  outfile5.open(filename5, ios::app);
  outfile5 << "Total number of positions: " << fHornHits << "\n";
  outfile5.close();
  
  G4double aper = fApertureHits;
  G4double coin = fCoincidenceHits;
  G4double NofE = NbOfEvents;
  
  G4double theta;
  G4double Omega_4pi; // solid angle divided by 4pi [Omega/4pi = (2pi(1-cos(theta)))/4pi]

  G4double frac;
  G4double eff;

  G4int power1;
  G4int power2;
  G4double num1;
  G4double num2;

  theta = apexAngle/2;
  Omega_4pi = (1 - cos(theta))/2;
  frac = (aper/NofE)*Omega_4pi;
  eff = (coin/NofE)*Omega_4pi;
  
  power1 = 0;

  if (frac > 0)
    power1 = -(log10(frac) - 1);
    num1 = pow(10,(power1));
    frac = frac*num1;

  power2 = 0;
     
  if (eff > 0)
    power2 = -(log10(eff) - 1);
    num2 = pow(10,(power2));
    eff = eff*num2;

  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << " Total number of events: " << NofE << "\n"
     << " Total number of coincidences: " << coin << "\n"
     << " Total number of positrons passing through the aperture: " << aper << "\n"
     << " Total number of positrons hitting the horn cap: " << fHornHits << "\n"
     << "\n"
     << " Fractional Solid Angle = " << frac << " E-" << power1 << "\n"
     << " Coincidence Efficiency = " << eff << " E-" <<power2 << "\n"
    
     << G4endl;
}
