#include         <iostream>
#include         <fstream>
#include         <sstream>
#include         <stdlib.h>
#include         <math.h>
#include         <string.h>
#include         <string>
#include         <assert.h>

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

#define	INPUT_PARAMETERS	"InputParameters.txt"

using namespace std;

extern G4String filename1;	// InputParameters.txt defined
extern G4String filename2;
extern G4String filename3;
extern G4String filename4;
extern G4String filename5;      // HC.txt file
extern G4String betaSpectrumName;

extern G4double apexAngle;      // InputParameters.txt defined
extern G4double pe;		// this is particle energy. I just ran out of name ideas.
extern G4double sourceX;
extern G4double sourceY;
extern G4double sourceZ;
extern G4double sourceDepth;


extern G4double dir_X; // particle direction 
extern G4double dir_Y;
extern G4double dir_Z;

extern int particleTypeFlag;

int main(int argc,char** argv)
{
  G4String name1, name2, name3, name4, name5;  
  G4String betaSpec;// All these parameters are defined externally as code variables.
  double sx, sy, sz, sd, apan, pEnergy, dx, dy, dz;		// Also with values in InputParameters.txt file.
  int ptflag;					// Parameters used by other classes.

  string buf;
  ifstream infile;
  cout << "The file being opened is: " << INPUT_PARAMETERS << endl;
  infile.open(INPUT_PARAMETERS);

  //a check to make sure the file is open
  if(!infile.is_open())
    cout << "Problem opening " << INPUT_PARAMETERS << endl;

  while(getline(infile, buf))
  {
    istringstream bufstream(buf);
    if(!bufstream.eof())
    {
      bufstream >> name1 >> name2 >> name3 >> name4 >> name5
                >> ptflag >> pEnergy >> betaSpec
		>> sx >> sy >> sz >> sd
		>> apan >> dx >> dy >> dz;
    }
  }
  cout << "Data from " << INPUT_PARAMETERS << " has been filled into all arrays successfully." << endl;

  filename1 = name1;	// This sets the values we input as the externally declared variables.
  filename2 = name2;	// Additionally, the code is done this way to add the units to the G4 primitives afterwards.
  filename3 = name3;	// This is because using the .txt file, I wasn't able to get the parameters correctly with units.
  filename4 = name4;	// This is the work-around.
  filename5 = name5;

  betaSpectrumName = betaSpec;

  particleTypeFlag = ptflag;

  sourceX = sx*cm;
  sourceY = sy*cm;
  sourceZ = sz*cm;
  sourceDepth = sd*1E-6*m;
  pe = pEnergy*keV;
  apexAngle = apan*deg;

  dir_X = dx*cm;
  dir_Y = dy*cm;
  dir_Z = dz*cm;


  if(particleTypeFlag == 1)
    cout << "You selected beta particles as the particle type" << endl;
  else if(particleTypeFlag == 2)
    cout << "You selected gamma rays as the particle type" << endl;
  else
    cout << "Error: particleTypeFlag can only take values of 1 or 2" << endl;

  G4cout << "Input parameters: \n"
	 << "File name of output file for pmt1: " << filename1 << "\n"
	 << "File name of output file for pmt2: " << filename2 << "\n"
	 << "File name of output file for pmt3: " << filename3 << "\n"
	 << "File name of output file for pmt4: " << filename4 << "\n"
         << "File name of output file for horn cap: " << filename5 << "\n"
	 << "Value of particle type flag: " << particleTypeFlag << "\n"
	 << "Name of beta spectrum file to be used if beta: " << betaSpectrumName << "\n"
	 << "Particle energy to be used if gamma: " << pe/keV << "keV \n"
	 << "Source x position: " << sourceX/cm << "cm \n"
	 << "Source y position: " << sourceY/cm << "cm \n"
	 << "Source z position: " << sourceZ/cm << "cm \n"
	 << "Source implantation depth: " << sourceDepth/m << "m \n"
	 << "Isotropic cone apex angle: " << apexAngle/deg << "degrees \n" //<<
         << "X Direction: " << dir_X/cm << "cm \n" 
         << "Y Direction: " << dir_Y/cm << "cm \n"
         << "Z Direction: " << dir_Z/cm << "cm \n" <<
  G4endl;

  G4int seed = time(NULL);
  // Choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes below
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list that we will define
  runManager -> SetUserInitialization(new PhysicsList());

  // Primary generator action
  runManager->SetUserAction(new PrimaryGeneratorAction());

  // Set user action classes
  RunAction* runAction = new RunAction();			// Declares run action object to be passed later
  runManager->SetUserAction(runAction);				// Run action
  runManager->SetUserAction(new EventAction(runAction));	// Event action

  // Initialize G4 kernel
  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  //  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1)
  {
    // enter batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);

#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    //#else
    //UImanager->ApplyCommand("/control/execute init.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
