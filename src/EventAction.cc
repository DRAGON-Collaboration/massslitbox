#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "HornCapHit.hh" 

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
using   namespace       std;

//#define	NUMBER_CRYSTALS		3	// # of SD's
#define NUMBER_CRYSTALS		4	// positron detectors added

extern G4String filename1;
extern G4String filename2;
extern G4String filename3;
extern G4String filename4;
extern G4String filename5;

EventAction::EventAction(RunAction* runAction)
 : G4UserEventAction(),		//initializes class properties
   fRunAct(runAction),
   fHitOne(false), fHitTwo(false), fHitThree(false), fHitFour(false),
   fPrintModulo(100000)		// Print Modulo just prints to screen the event # were on when we get a multiple of it
{
  for(int t = 0; t < NUMBER_CRYSTALS; t++)
  {
    eDepCryst[t] = 0;		//also initializes class properties, just does it as a loop inside constructor
    fCollID_scint[t] = 0;
  }

  hitsCollID = -1;
 
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* evt )
{
  G4int evtNb = evt->GetEventID();

  if (evtNb == 0)
  {
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();
    fCollID_scint[0] = SDMan->GetCollectionID("crystal1/edep");
    fCollID_scint[1] = SDMan->GetCollectionID("crystal2/edep");
    fCollID_scint[2] = SDMan->GetCollectionID("crystal3/edep");
    fCollID_scint[3] = SDMan->GetCollectionID("apertureVac/edep"); // positron detector in the center of the aperture
    if(hitsCollID<0)
    {
      G4String colNam;
      hitsCollID = SDMan->GetCollectionID(colNam="hitsCollection"); // positron detector in the horn cap
    }
  }

  if (evtNb%fPrintModulo == 0)
    G4cout << "\n------------ Begin event: " << evtNb << G4endl;

}

void EventAction::EndOfEventAction(const G4Event* evt )
{
  //Hits collections
  
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if(!HCE)
    return;
  

// positron detector in the horn cap
  HornCapHitsCollection* HC = 0;
  if(HCE)
  {
    HC = (HornCapHitsCollection*)(HCE->GetHC(hitsCollID));
  }

  if(HC)
  {
    int n_hit = HC->entries();
    for (int k = 0; k < n_hit; k++)
    {
      G4ThreeVector position = (*HC)[k]->GetPosition();
      G4String pid = (*HC)[k]->GetParticle()->GetParticleName();
      
      if(pid == "e+")     
      {
        fRunAct -> CountSuccessfulHits2();
        ofstream outfile;
        outfile.open(filename5, ios::app);
        outfile << (position.x() - 17.5)/cm << "\t" << (position.y() - 406.6)/cm << "\t" << (position.z() + 50.1)/cm << "\n";
        outfile.close();
      }
    }
  }

 
  //creates a HitsMap object called evtMap that we can use to find our how much energy was deposited.
  //also creates an iterator that we use to loop over and actually store the energy
  G4THitsMap<G4double>* evtMap;
  std::map<G4int,G4double*>::iterator itr;

  //goes through our evtMap and saves the energy deposited in the particular scintillator named above ^^
  //no need to worry too much about what's happening and just know that we're only getting energy deposited in SD
  for(int i = 0; i < NUMBER_CRYSTALS; i++)	//this loop does it for all scintillating crystals
  {
    evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID_scint[i]));
    for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++)
      eDepCryst[i] = *(itr->second);
  } 
  
  CheckIncidences();			// calls method to see if we have hits in the two SD's

  if(fHitOne && fHitTwo)
    fRunAct -> CountSuccessfulHits1();	// Go to RunAction.hh and call instance of CountSuccessfulHits()
  					// This increments our run's class property fCoincidenceHits
  					// Which keeps track of how many coincidences we have.
  if(fHitFour)				
    fRunAct -> CountSuccessfulHits3();


  if(fHitOne)
  {
    PrintDataToFile(1);
  }
  if(fHitTwo)
  {
    PrintDataToFile(2);
  }
  if(fHitOne && fHitTwo)
  {
    PrintDataToFile(3);
  }
  if(fHitThree)
  {
    PrintDataToFile(4);
  }

  Reset();					// At end of event, reset everything manually
}

void EventAction::CheckIncidences()
{
  if(eDepCryst[0] > 0)		// checks if any energy was deposited into the crystals
    fHitOne = true;		// if so, then there was a trigger in the PMT/NaI
  if(eDepCryst[1] > 0)		// So set our boolean check to true in the corresponding NaI
    fHitTwo = true;
  if(eDepCryst[2] > 0)
    fHitThree = true;
  if(eDepCryst[3] > 0)
    fHitFour = true;
}

void EventAction::Reset()
{
  fHitOne = false;	// I don't trust destructor. This is explicitly resetting all the values at end of event.
  fHitTwo = false;
  fHitThree = false;
  fHitFour = false;

  for(int i = 0; i < NUMBER_CRYSTALS; i++)
    eDepCryst[i] = 0;
}

void EventAction::PrintDataToFile(int flag)	// 1 = NaI1, 2 = NaI2, 3 = NaI1 and NaI2 both hit
{						// flag = 4 means NaI3 which is the one we're really interested in
  ofstream outfile;

  if(flag == 1)
  {
    outfile.open(filename1, ios::app);		// Need to divide by keV since eDepCryst has units (G4double)
    outfile << eDepCryst[0]/keV << "\n";	// eDepCryst[0] stores energy in NaI1
  }
  if(flag == 2)
  {
    outfile.open(filename2, ios::app);
    outfile << eDepCryst[1]/keV << "\n";	// " " " " NaI2
  }
  if(flag == 3)
  {
    outfile.open(filename3, ios::app);		// flag == 3 means we want energies deposited in both NaI's
    outfile << eDepCryst[0]/keV << "\t" << eDepCryst[1]/keV << "\n";
  }
  if(flag == 4)
  {
    outfile.open(filename4, ios::app);
    outfile << eDepCryst[2]/keV << "\n"; 	// " == 4 means the trigger is in the NaI on the table
  }

  outfile.close();
}
