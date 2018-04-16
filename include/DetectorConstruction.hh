#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  private:
    G4LogicalVolume* experimentalHall_log;	// Logical Volumes
    G4LogicalVolume* vacBox_log;
    G4LogicalVolume* vacBoxInside_log;
    G4LogicalVolume* XSlitM_log;
    G4LogicalVolume* hornTube_log;
    //G4LogicalVolume* hornVac_log;
    G4LogicalVolume* slitGap_log;
    G4LogicalVolume* ellipseHole_log;
    G4LogicalVolume* hornBevel_log;
    G4LogicalVolume* NaI1_log;
    G4LogicalVolume* NaI2_log;
    G4LogicalVolume* NaI3_log;
    G4LogicalVolume* NaI1_case_log;
    G4LogicalVolume* NaI2_case_log;
    G4LogicalVolume* NaI3_case_log;
    G4LogicalVolume* aperture_log;
    G4LogicalVolume* apertureVac_log;

    G4VPhysicalVolume* experimentalHall_phys;	// Physical Volumes
    G4VPhysicalVolume* vacBox_phys;		// I actually don't need these physical volumes
    G4VPhysicalVolume* vacBoxInside_phys;	// G4PVPlacement doesn't need to create the object
    G4VPhysicalVolume* XSlitM_phys;		// We're leaving them here just because, but
    G4VPhysicalVolume* hornTube_phys;		// in the actual code these properties are not allocated.
    //G4VPhysicalVolume* hornVac_phys;
    G4VPhysicalVolume* slitGap_phys;
    G4VPhysicalVolume* ellipseHole_phys;
    G4VPhysicalVolume* hornBevel_phys;
    G4VPhysicalVolume* NaI1_phys;
    G4VPhysicalVolume* NaI2_phys;
    G4VPhysicalVolume* NaI3_phys;		// I need the NaI physical volumes because
    G4VPhysicalVolume* NaI1_case_phys;		// they get registered as SD's
    G4VPhysicalVolume* NaI2_case_phys;
    G4VPhysicalVolume* NaI3_case_phys;
    G4VPhysicalVolume* aperture_phys;
    G4VPhysicalVolume* apertureVac_phys;

  //these are all the functions that belong to our class.
  // all of them are the standard defined ones. I didn't add any special functions.
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

  private:
    void DefineMaterials();
    void CreateScorers();
};

#endif

