#include "DetectorConstruction.hh"

#include "G4Material.hh"        // These are Debra's #include packages.
#include "G4Box.hh"
#include "G4Tubs.hh"
//#include "G4CutTubs.hh"
#include "G4EllipticalTube.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4NistManager.hh"     // These were my own #include packages.
#include "G4LogicalVolume.hh"   // Obviously, I deleted the ones here that overlapped with Debra's.
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4SDParticleFilter.hh"
#include "HornCapSD.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
using	namespace	std;


extern G4double NaI3_pos_x;	// These are externally defined so PrimaryGeneratorAction.cc can access
extern G4double NaI3_pos_y;	// them to do the reduced angle particle generation. Shouldn't be needed
extern G4double NaI3_pos_z;	// for main simulation but they'll be left here for future reduced-angle.


DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  experimentalHall_log(0),			// Note: the physical volumes (_phys)
  vacBox_log(0), vacBox_phys(0),		// actually aren't necessary.
  vacBoxInside_log(0), vacBoxInside_phys(0),	// We don't need to save them, just place them.
  XSlitM_log(0), XSlitM_phys(0),
  hornTube_log(0), hornTube_phys(0),
  //hornVac_log(0), hornVac_phys(0),
  slitGap_log(0), slitGap_phys(0),
  ellipseHole_log(0), ellipseHole_phys(0),
  hornBevel_log(0), hornBevel_phys(0),
  NaI1_log(0), NaI1_phys(0),
  NaI2_log(0), NaI2_phys(0),			// Need NaI_phys since the NaI's get registered as SD's
  NaI3_log(0), NaI3_phys(0),
  NaI1_case_log(0), NaI1_case_phys(0),
  NaI2_case_log(0), NaI2_case_phys(0),
  NaI3_case_log(0), NaI3_case_phys(0)
{
  DefineMaterials();
}

DetectorConstruction::~DetectorConstruction()
{
}

void DetectorConstruction::DefineMaterials()
{
  G4double a;           // atomic mass
  G4double z;           // atomic number
  G4double density;
  G4double nComponents; // how many elements make up a material
  G4String symbol;      // string represents the symbol of element
  G4double fractionalMass;

  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);		// general elements
  G4Element* O = new G4Element("Oxygen",symbol="O",z=8.,a=16.*g/mole);
  G4Element* Na = new G4Element("Sodium",symbol="Na",z=11.,a=23.*g/mole);
  G4Element* I = new G4Element("Iodine", symbol="I",z=53.,a=127.*g/mole);
  G4Element* Cr = new G4Element("Chromium", symbol="Cr",z=24.,a=52.*g/mole);
  G4Element* Fe = new G4Element("Iron", symbol="Fe",z=26.,a=55.8*g/mole);
  G4Element* Ni = new G4Element("Nickel", symbol="I",z=28.,a=58.7*g/mole);
  G4Element* C = new G4Element("Carbon", symbol="C",z=6.,a=12.01*g/mole);
  G4Element* F = new G4Element("Flourine", symbol="F",z=9.,a=19.0*g/mole);

  G4Material* NaI = new G4Material("NaI",density=3.67*g/cm3, nComponents=2);	// making the materials
  NaI->AddElement(Na,fractionalMass=0.1533);
  NaI->AddElement(I, fractionalMass=0.8467);
  G4Material* PTFE = new G4Material("PTFE",density=2.2*g/cm3, nComponents=2);
  PTFE->AddElement(C,fractionalMass=0.24);
  PTFE->AddElement(F,fractionalMass=0.76);
  G4Material* StSt316L = new G4Material("StSt316L",density=8.0*g/cm3, nComponents=3);
  StSt316L->AddElement(Fe,fractionalMass=0.7);
  StSt316L->AddElement(Cr,fractionalMass=0.18);
  StSt316L->AddElement(Ni,fractionalMass=0.12);
  G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nComponents=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);
  G4Material* Vacuum = new G4Material("Vacuum", density= 0.*mg/cm3, nComponents=2);
  Vacuum->AddElement(N, 70*perCent);
  Vacuum->AddElement(O, 30*perCent);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();			// Go to Nist database
  G4Material* NaI = nist -> FindOrBuildMaterial("NaI");
  G4Material* PTFE = nist -> FindOrBuildMaterial("PTFE");
  G4Material* StSt316L = nist -> FindOrBuildMaterial("StSt316L");
  G4Material* Air = nist -> FindOrBuildMaterial("Air");
  G4Material* Vacuum = nist -> FindOrBuildMaterial("Vacuum");

  G4Material* Al = new G4Material("Aluminum", 13., 26.98*g/mole, 2.7*g/cm3);	// Should go in DefineMaterials
  G4Material* Pb = new G4Material("Lead", 82., 207.19*g/mole, 11.35*g/cm3);	// but for some reason not working. Put here.


  // ---------------------------- Experimental hall ----------------------------------
  G4double expHall_x = 2.0*m;	// beam length along z-axis (from negative to positive).
  G4double expHall_y = 2.0*m;	// half lengths of the experimental hall i.e. the World Volume
  G4double expHall_z = 2.0*m;
  G4Box* experimentalHall_Box = new G4Box("expHall_Box", expHall_x, expHall_y, expHall_z);	// Solid
  experimentalHall_log = new G4LogicalVolume(experimentalHall_Box, Air, "expHall_log");		// Logical volume
  experimentalHall_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), experimentalHall_log, "expHall", 0, false, 0);// Phys Vol. Also "World"
														// hence pointer to world=0
  //--------------------------- The lead box ------------------------------
  G4double leadBox_x = 38.1*cm; // half lengths already. That's the input that Solids take
  G4double leadBox_y = 38.1*cm;
  G4double leadBox_z = 45.085*cm;
  G4Box* leadBox = new G4Box("leadBox", leadBox_x, leadBox_y, leadBox_z);

  G4double leadBoxInt_x = leadBox_x - (2.54*2)*cm;
  G4double leadBoxInt_y = leadBox_y - (2.54*1)*cm;
  G4double leadBoxInt_z = leadBox_z - (2.54*2)*cm;
  G4Box* leadBoxInt = new G4Box("leadBoxInt", leadBoxInt_x, leadBoxInt_y, leadBoxInt_z);

  G4double vacBoxOffset_z = -8.125*cm;	// Presumably some offset measurement
  G4double vacBoxOffset_y = 3.28*cm;	// These values are used much later

  // ------------------- Subtraction 1: interior happens s.t. there is no bottom to the box. --------------------
  G4ThreeVector bottomTranslation_y(0, -2.54*cm, 0);
											// (... no rotation, translation vector above);
  G4SubtractionSolid* leadBoxSub1 = new G4SubtractionSolid("leadBoxSub1", leadBox, leadBoxInt, 0, bottomTranslation_y);

  // ------------------- Subtraction 2: Top of lead box ---------------------------
  G4double leadBoxTop_x = 19.5*cm;	// half lengths again
  G4double leadBoxTop_y = 2.54*cm;
  G4double leadBoxTop_z = 33.655*cm;
  G4Box* leadBoxTop = new G4Box("leadBoxTop", leadBoxTop_x, leadBoxTop_y, leadBoxTop_z);
  G4ThreeVector topTranslation_y(0, (38.1-2.54)*cm, 0);

  G4SubtractionSolid* leadBoxSub2 = new G4SubtractionSolid("leadBoxSub2", leadBoxSub1, leadBoxTop, 0, topTranslation_y);

  // ------------------- Subtraction 3 & 4: Cylinders in front of lead box ----------------
  G4Tubs* leadBoxFront = new G4Tubs("leadBoxFront", 0, 9.525*cm, 3.175*cm, 0, 360*deg);

  G4ThreeVector frontTranslation_3(-34.925*cm, vacBoxOffset_y, 5.08*cm);
  G4ThreeVector frontTranslation_4(-34.925*cm, vacBoxOffset_y, -22.225*cm);
  G4RotationMatrix* rotation_y = new G4RotationMatrix;	// declare a rotation matrix
  rotation_y -> rotateY((0.5*M_PI)*rad);		// rotate our cylinders into position

  G4SubtractionSolid* leadBoxSub3 = new G4SubtractionSolid("leadBoxSub3", leadBoxSub2, leadBoxFront, rotation_y, frontTranslation_3);
  G4SubtractionSolid* leadBoxSub4 = new G4SubtractionSolid("leadBoxSub4", leadBoxSub3, leadBoxFront, rotation_y, frontTranslation_4);

  // ------------------- Subtraction 5 & 6: sides of lead box ----------------------------
  G4double leadBoxSide_x = 13.335*cm;
  G4double leadBoxSide_y = 26.67*cm;	// half lengths again
  G4double leadBoxSide_z = 3.175*cm;
  G4Box* leadBoxSide = new G4Box("leadBoxSide", leadBoxSide_x, leadBoxSide_y, leadBoxSide_z);

  G4ThreeVector sideTranslation_5(1.905*cm, -11.43*cm, 41.91*cm);
  G4ThreeVector sideTranslation_6(1.905*cm, -11.43*cm, -41.91*cm);

  G4SubtractionSolid* leadBoxSub5 = new G4SubtractionSolid("leadBoxSub5", leadBoxSub4, leadBoxSide, 0, sideTranslation_5);

  G4SubtractionSolid* leadBoxSub6 = new G4SubtractionSolid("leadBoxSub6", leadBoxSub5, leadBoxSide, 0, sideTranslation_6);
  
  G4LogicalVolume* leadBoxSub6_log = new G4LogicalVolume(leadBoxSub6, Pb, "leadBoxSub6");

  // At this point, the old student made a physical volume and then placed it. There is no need to make any
  // phys. vol. - all we need to do is place the logical volume somewhere (i.e. G4PVPlacement).
  // The only phys vol.'s we absolutely need is the World (experimental hall) and probably the NaI
  // since they are sensitive detectors (SD's). So I will delete all physical volume variables that the compiler tells
  // me aren't in use by the program.
    
 new G4PVPlacement(0, G4ThreeVector(0, -4*cm, vacBoxOffset_z), leadBoxSub6_log, "leadBoxSub6", experimentalHall_log, false, 0);
  // --------------------- The stainless steel vacuum box ----------------------
  G4double vacBox_x = 24.4475*cm;
  G4double vacBox_y = 19.685*cm;	// probably half lengths.
  G4double vacBox_z = 34.925*cm;
  G4Box* vacBox = new G4Box("vacBox", vacBox_x, vacBox_y, vacBox_z);
  vacBox_log = new G4LogicalVolume(vacBox, StSt316L, "vacBox");
  vacBox_phys = new G4PVPlacement(0, G4ThreeVector(0, vacBoxOffset_y, vacBoxOffset_z), vacBox_log, "vacBox",
					experimentalHall_log, false, 0);

  // --------------------- The interior of vacuum box (vacuum) ------------------------
  G4double vacBoxInside_x = vacBox_x - 0.8*cm;	// Instead of "subtracting", the interior,
  G4double vacBoxInside_y = vacBox_y - 0.8*cm;	// we place a new solid inside made of Vacuum
  G4double vacBoxInside_z = vacBox_z - 0.8*cm;	// which is the same thing
  G4Box* vacBoxInside = new G4Box("vacBoxInside", vacBoxInside_x, vacBoxInside_y, vacBoxInside_z);
  vacBoxInside_log = new G4LogicalVolume(vacBoxInside, Vacuum, "vacBoxInside");
  vacBoxInside_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), vacBoxInside_log, "vacBoxInside", vacBox_log, false, 0);

  // --------------------- The steel tube used for calibrating Co-60 source --------------
/*  G4Tubs* sourceCylinder = new G4Tubs("sourceCylinder",	0, 0.32*cm, 1*cm, 0, 360*deg);
  G4LogicalVolume* sourceCylinder_log =	new G4LogicalVolume(sourceCylinder, StSt316L, "sourceCylinder");
  new G4PVPlacement(rotation_y, G4ThreeVector(0, 0.5*cm - vacBoxOffset_y, -1.3*cm - vacBoxOffset_z),
			sourceCylinder_log, "sourceCylinder", vacBoxInside_log, false, 0);

	// you can comment out or delete this. It was only useful to get a more accurate representation
	// of the steel barrel that was the Co-60 source. It's not useful once you move to point sources.
*/

  // --------------------- Holes in front of steel vacuum box -------------------------
  G4Tubs* vacBoxCylinder = new G4Tubs("vacBoxCylinder", 0, 5.08*cm, 0.4*cm, 0, 360*deg);
  G4LogicalVolume* vacBoxCylinder_log = new G4LogicalVolume(vacBoxCylinder, Vacuum, "vacBoxCylinder");
  new G4PVPlacement(rotation_y, G4ThreeVector(-24.0475*cm, -vacBoxOffset_y, -22.225*cm),
			vacBoxCylinder_log, "vacBoxCylinder", vacBox_log, false, 0);
  new G4PVPlacement(rotation_y, G4ThreeVector(-24.0475*cm, -vacBoxOffset_y, 5.08*cm),
			vacBoxCylinder_log, "vacBoxCylinder", vacBox_log, false, 0);

  // ---------------------- Aluminum ports -------------------------------
  // The old measurements were not correct! They are ones I've carefully checked over.
  // These new measurements are all correct RELATIVE to the old positioning of the holes in the
  // steel box. i.e. the y was assumed to be the same, the x offset due to box was assumed correct,
  // and the z positioning of the center of the holes was assumed to be correct. Everything else
  // (dimensions, composition, and new positioning due to new dimensions) I've checked and confirmed.
  // 			- Xuan Sun, summer 2014
  G4Tubs* vacBoxAl = new G4Tubs("vacBoxAl", 0, 7.6*cm, 0.9*cm, 0, 360*deg);
  G4LogicalVolume* vacBoxAl_log = new G4LogicalVolume(vacBoxAl, Al, "vacBoxAl");
  new G4PVPlacement(rotation_y, G4ThreeVector(-28.8875*cm, 0, vacBoxOffset_z + 5.08*cm),
			vacBoxAl_log, "vacBoxAl", experimentalHall_log, false, 0);
  new G4PVPlacement(rotation_y, G4ThreeVector(-28.8875*cm, 0, vacBoxOffset_z -22.225*cm),
			vacBoxAl_log, "vacBoxAl", experimentalHall_log, false, 0);

  G4Tubs* vacBoxRing = new G4Tubs("vacBoxRing", 4.8*cm, 7.5*cm, 1.2*cm, 0, 360*deg);
  G4LogicalVolume* vacBoxRing_log = new G4LogicalVolume(vacBoxRing, StSt316L, "vacBoxRing");
  new G4PVPlacement(rotation_y, G4ThreeVector(-26.7475*cm, 0, vacBoxOffset_z + 5.08*cm),
                        vacBoxRing_log, "vacBoxRing", experimentalHall_log, false, 0);
  new G4PVPlacement(rotation_y, G4ThreeVector(-26.7475*cm, 0, vacBoxOffset_z -22.225*cm),
                        vacBoxRing_log, "vacBoxRing", experimentalHall_log, false, 0);

  G4Tubs* vacBoxConnector = new G4Tubs("vacBoxConnector", 4.8*cm, 5.3*cm, 0.5*cm, 0, 360*deg);
  G4LogicalVolume* vacBoxConnector_log = new G4LogicalVolume(vacBoxConnector, StSt316L, "vacBoxConnector");
  new G4PVPlacement(rotation_y, G4ThreeVector(-24.9575*cm, 0, vacBoxOffset_z + 5.08*cm),
			vacBoxConnector_log, "vacBoxConnector", experimentalHall_log, false, 0);
  new G4PVPlacement(rotation_y, G4ThreeVector(-24.9575*cm, 0, vacBoxOffset_z - 22.225*cm),
                        vacBoxConnector_log, "vacBoxConnector", experimentalHall_log, false, 0);

  // ---------------------- X Slit M  ---------------------------
  G4double XSlitM_x = 13.0*cm;	// Not half lengths. These are full lengths. *edit Annika: measured slits in x: 13cm (including 15mm gap), previous value: 15cm
  G4double XSlitM_y = 6.5*cm; // *edit Annika: y=6.5cm, previous value 10.0cm
  G4double XSlitM_z = 1.0*cm;	// Hence in Solid declaration we multiply by 0.5
  G4Box* XSlitM = new G4Box("XSlitM", 0.5*XSlitM_x, 0.5*XSlitM_y, 0.5*XSlitM_z);
  XSlitM_log = new G4LogicalVolume(XSlitM, Al, "XSlitM");
  XSlitM_phys = new G4PVPlacement(0, G4ThreeVector(0, -vacBoxOffset_y, -vacBoxOffset_z),
				XSlitM_log, "XSlitM", vacBoxInside_log, false, 0);
  G4double slitGapWidth = 15*mm; //full length
  G4Box* slitGap = new G4Box("slitGap", 0.5*slitGapWidth, 0.5*XSlitM_y, 2*mm + 0.5*XSlitM_z);
  slitGap_log = new G4LogicalVolume(slitGap, Vacuum, "slitGap");
  slitGap_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), slitGap_log, "slitGap", XSlitM_log, false, 0);

  // ---------------------- The horn, main tube ---------------------------------------
  G4double hornTubeInnerDiam = 0*cm;	// Implies this whole thing is full
  G4double hornTubeOuterDiam = 5.08*cm;
  G4double hornTubeLength = 2.54*8*cm;
  G4double hornTubeAngle = 6.7*deg;	// angle to the vertical
  G4double hornBevelAngle = 45.0*deg;	// angle of the horn beveled part
  G4double hornXOffset = 1.75*cm;
  G4double hornTube_x = hornXOffset;
  G4double hornTube_y = 32.5*cm;
  G4double hornTube_z = -hornTube_y*tan(hornTubeAngle);
  G4RotationMatrix* tubeRotation = new G4RotationMatrix();
  tubeRotation -> rotateX(90*deg + hornTubeAngle);
  

  //G4Tubs* hornTubeMain1 = new G4Tubs("hornTubeMain1", 0.5*hornTubeInnerDiam, 0.5*hornTubeOuterDiam, 0.5*hornTubeLength, 0, 360*deg);
  G4Tubs* hornTubeMain1 = new G4Tubs("hornTubeMain1", 0.5*4.445*cm, 0.5*hornTubeOuterDiam, 0.5*hornTubeLength, 0, 360*deg);
  
  // Make two boxes below. Rotate them. Then subtract them from the cylinder (defined in line above).
  G4Box* sub1 = new G4Box("sub1", hornTubeOuterDiam, hornTubeOuterDiam*sin(hornTubeAngle), hornTubeOuterDiam/cos(hornTubeAngle));
  G4Box* sub2 = new G4Box("sub2", hornTubeOuterDiam*cos(hornBevelAngle), hornTubeOuterDiam, hornTubeOuterDiam*cos(hornBevelAngle));
  G4RotationMatrix* subRot1 = new G4RotationMatrix();
  G4RotationMatrix* subRot2 = new G4RotationMatrix();
  subRot1 -> rotateX(90*deg-hornTubeAngle);
  subRot2 -> rotateY(hornBevelAngle);
  G4SubtractionSolid* hornTubeMain2 = new G4SubtractionSolid("hornTubeMain2", hornTubeMain1, sub1, subRot1,
  								G4ThreeVector(0, 0, -0.5*hornTubeLength));
  G4SubtractionSolid* hornTube = new G4SubtractionSolid("hornTube", hornTubeMain2, sub2, subRot2,
								G4ThreeVector(-0.5*hornTubeOuterDiam, 0, 0.5*hornTubeLength));

  hornTube_log = new G4LogicalVolume(hornTube, StSt316L, "hornTube");
  hornTube_phys = new G4PVPlacement(tubeRotation, G4ThreeVector(hornTube_x, hornTube_y, hornTube_z),
				hornTube_log, "hornTube", experimentalHall_log, false, 0);

  //The vacuum cylinder (hornVac) placed at the center of the horn was upside down. To correct this, hornTubeMain1 was made into a tube and hornVac was removed.

  //G4double hornVacInnerDiam = 0;	// 0 diameter means tube is actually a solid cylinder
  //G4double hornVacOuterDiam = 4.445*cm;	// but this is defining the vacuum/ empty part of the cylinder, so it's ok.
  //G4Tubs* hornVacMain1 = new G4Tubs("HornVacMain1", 0.5*hornVacInnerDiam, 0.5*hornVacOuterDiam, 0.5*hornTubeLength, 0, 360*deg);
 
  //G4SubtractionSolid* hornVacMain2 = new G4SubtractionSolid("hornVacMain2", hornVacMain1, sub1, subRot1,
  //							G4ThreeVector(0, 0, -0.5*hornTubeLength));
  
  //G4SubtractionSolid* hornVac = new G4SubtractionSolid("hornVac", hornVacMain2, sub2, subRot2,
  //							G4ThreeVector(-0.5*hornTubeOuterDiam, 0, -0.5*hornTubeLength));
  //hornVac_log = new G4LogicalVolume(hornVac, Vacuum, "hornVac");

 // hornVac_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), hornVac_log, "hornVac", hornTube_log, false, 0);

  // ------------------------------- Elliptical hole in steel box -----------------------------------------
  G4double ellipseHole_x = 0.5*hornTubeOuterDiam;
  G4double ellipseHole_y = 0.5*hornTubeOuterDiam/cos(hornTubeAngle);	// cos(angle*deg) is right. I checked.
  G4double ellipseHole_z = 0.5*(vacBox_y - vacBoxInside_y);		// If you specific *deg, cos will compute correctly.
  G4EllipticalTube* ellipseHole = new G4EllipticalTube("ellipseHole", ellipseHole_x, ellipseHole_y, ellipseHole_z);
  ellipseHole_log = new G4LogicalVolume(ellipseHole, Vacuum, "ellipseHole");

  G4double ellipseHolePos_x = hornXOffset;
  G4double ellipseHolePos_y = vacBoxInside_y + ellipseHole_z;
  G4double ellipseHolePos_z = -ellipseHolePos_y*tan(hornTubeAngle);
  G4RotationMatrix* ellipseHoleRotation = new G4RotationMatrix();
  ellipseHoleRotation -> rotateX(90.0*deg);
  ellipseHole_phys = new G4PVPlacement(ellipseHoleRotation, G4ThreeVector(ellipseHolePos_x, ellipseHolePos_y, ellipseHolePos_z - vacBoxOffset_z),
					ellipseHole_log, "ellipseHole", vacBox_log, false, 0);

  // ------------------------------- The horn bevelled cap (ellipse) ---------------------------------------
  G4double hornBevel_x = 0.5*hornTubeOuterDiam/cos(hornBevelAngle);
  G4double hornBevel_y = 0.5*hornTubeOuterDiam;
  G4double hornBevel_z = 0.2375*cm;
  G4EllipticalTube* hornBevel = new G4EllipticalTube("hornBevel", hornBevel_x, hornBevel_y, hornBevel_z);
  G4RotationMatrix* hornBevelRotation = new G4RotationMatrix();
  hornBevelRotation -> rotateZ(-45.0*deg);
  hornBevelRotation -> rotateX(90*deg + hornTubeAngle);
  hornBevel_log = new G4LogicalVolume(hornBevel, StSt316L, "hornBevel");
  G4double hornBevelPos_x = hornXOffset;
  G4double hornBevelPos_y = hornTube_y + 0.5*hornTubeLength - 2.0*cm;
  G4double hornBevelPos_z = -(hornTube_y + 0.5*hornTubeLength)*tan(hornTubeAngle);
  hornBevel_phys = new G4PVPlacement(hornBevelRotation, G4ThreeVector(hornBevelPos_x, hornBevelPos_y, hornBevelPos_z),
					hornBevel_log, "hornBevel", experimentalHall_log, false, 0);

  // ----------------------------- The NaI detectors on top -------------------------------
  G4double NaI_CaseDiam = 8.04*cm;
  G4double NaI_CaseDepth = 8.04*cm;	// The Logical Volumes are what get registered as sensitive detectors.
  G4double NaI_CoatingDiam = 7.94*cm;	// See CreateScorers() method to see that they get registered there
  G4double NaI_CoatingDepth = 7.94*cm;
  G4double NaI_CrystalDiam = 7.62*cm;	// 3x3 inches crystal size.
  G4double NaI_CrystalDepth = 7.62*cm;

  // Here I'm noting that the old student used track-level analysis to get the total energy deposited in each NaI.
  // Instead of that, I'm using event-level analysis. This means I'm registering each NaI crystal as a
  // sensitive detector (SD) which stores basic information in it. At the end of each event, I access the SD's
  // and pull out how much energy was deposited in each SD. I think under the covers, the code is doing the exact
  // same things, but this conceptually easier for me to understand and implement when I did it the first time.
  //			- Xuan Sun, summer 2014

  G4Tubs* NaI_case = new G4Tubs("NaI_case", 0, 0.5*NaI_CaseDiam, 0.5*NaI_CaseDepth, 0, 360*deg);
  G4Tubs* NaI_crystal = new G4Tubs("NaI_crystal", 0, 0.5*NaI_CrystalDiam, 0.5*NaI_CrystalDepth, 0, 360*deg);
  G4Tubs* reflector = new G4Tubs("Reflector", 0, 0.5*NaI_CoatingDiam, 0.5*NaI_CoatingDepth, 0, 360*deg);

  NaI1_case_log = new G4LogicalVolume(NaI_case, Al, "NaI1_case");
  NaI2_case_log = new G4LogicalVolume(NaI_case, Al, "NaI2_case");
  NaI1_log = new G4LogicalVolume(NaI_crystal, NaI, "crystal1");
  NaI2_log = new G4LogicalVolume(NaI_crystal, NaI, "crystal2");
  G4LogicalVolume* reflector1_log = new G4LogicalVolume(reflector, PTFE, "reflector1");
  G4LogicalVolume* reflector2_log = new G4LogicalVolume(reflector, PTFE, "reflector2");

  G4double NaI1_pos_x = -17.4*cm - hornXOffset; //edit Annika: I think the hornXOffset had to be substracted and not added to the negative x-value. Otherwise NaI1 would have been closer to the horn than NaI2!! Also changed distance from -16.4cm to -17.4cm to see how it affects efficiency ratio from NaI1/NaI2
  G4double NaI2_pos_x = 17.4*cm + hornXOffset;
  G4double NaI_pos_y = hornBevelPos_y;
  G4double NaI_pos_z = hornBevelPos_z;

  G4RotationMatrix* NaI1_rotation = new G4RotationMatrix();
  G4RotationMatrix* NaI2_rotation = new G4RotationMatrix();  
  NaI1_rotation -> rotateY(90.0*deg);
  NaI2_rotation -> rotateY(-90.0*deg);

  NaI1_case_phys = new G4PVPlacement(NaI1_rotation, G4ThreeVector(NaI1_pos_x, NaI_pos_y, NaI_pos_z),
					NaI1_case_log, "NaI2", experimentalHall_log, false, 0);
  NaI2_case_phys = new G4PVPlacement(NaI2_rotation, G4ThreeVector(NaI2_pos_x, NaI_pos_y, NaI_pos_z),
					NaI2_case_log, "NaI2", experimentalHall_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), reflector1_log, "reflector1", NaI1_case_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), reflector2_log, "reflector2", NaI2_case_log, false, 0);
  NaI1_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), NaI1_log, "crystal1", reflector1_log, false, 0);
  NaI2_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), NaI2_log, "crystal2", reflector2_log, false, 0);

  // ----------------------- The NaI detector on the side table -----------------------------
  G4double NaI3_CaseDiam = 5.13*cm;	 // 0.5mm thickness on aluminum. Specs found in /DRAGON/NotesAboutModifications.txt file.
  G4double NaI3_CaseDepth = 5.13*cm;     // The 3 means that this NaI detector is the one laying on the table
  G4double NaI3_CrystalDiam = 5.08*cm;   // 2x2 inch crystals
  G4double NaI3_CrystalDepth = 5.08*cm; 
  NaI3_pos_x = -53*cm;
  NaI3_pos_y = -4*cm;
  NaI3_pos_z = -45*cm;
  G4double NaI3_rotation_angle = 136*deg;

  G4Tubs* NaI3_case = new G4Tubs("NaI3_case", 0, 0.5*NaI3_CaseDiam, 0.5*NaI3_CaseDepth, 0, 360*deg);
  G4Tubs* NaI3_crystal = new G4Tubs("NaI3_crystal", 0, 0.5*NaI3_CrystalDiam, 0.5*NaI3_CrystalDepth, 0, 360*deg);

  NaI3_case_log = new G4LogicalVolume(NaI3_case, Al, "NaI3_case");
  NaI3_log = new G4LogicalVolume(NaI3_crystal, NaI, "crystal3");

  G4RotationMatrix* NaI3_rotation = new G4RotationMatrix();
  NaI3_rotation -> rotateY(NaI3_rotation_angle);	// This we toggle till it looks right from above
							// Also I used a protractor to measure.

  // When I declare a positioning for the case, DO NOT CHANGE it! It was carefully found using a chi-squared
  // grid search method. Furthermore, DON'T DELETE any of the code in this section! You may comment it out,
  // but do not get rid of it since, once the NaI detector is removed, there will be no way to get these
  // numbers back!
  // 			- Xuan Sun, summer 2014
  NaI3_case_phys = new G4PVPlacement(NaI3_rotation, G4ThreeVector(NaI3_pos_x, NaI3_pos_y, NaI3_pos_z),
                                        NaI3_case_log, "NaI3", experimentalHall_log, false, 0);
  NaI3_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), NaI3_log, "crystal3", NaI3_case_log, false, 0);

  // ----------------------- Lead blocks above steel vacuum box -----------------------------
  G4double leadBlock1_x = 0.5*15.24*cm;	// Random blocks placed around the apparatus
  G4double leadBlock1_y = 0.5*7.62*cm;	// Three types of lead blocks found. These are half lengths.
  G4double leadBlock1_z = 0.5*5.08*cm;
  G4double leadBlock2_x = 0.5*15.24*cm; // half lengths again
  G4double leadBlock2_y = 0.5*11.43*cm;
  G4double leadBlock2_z = 0.5*5.08*cm;
  G4double leadBlock3_x = 0.5*10.5*cm;
  G4double leadBlock3_y = 0.5*5*cm;
  G4double leadBlock3_z = 0.5*19.5*cm;

  G4Box* leadBlock1 = new G4Box("leadBlock1", leadBlock1_x, leadBlock1_y, leadBlock1_z);
  G4Box* leadBlock2 = new G4Box("leadBlock2", leadBlock2_x, leadBlock2_y, leadBlock2_z);
  G4Box* leadBlock3 = new G4Box("leadBlock3", leadBlock3_x, leadBlock3_y, leadBlock3_z);
  G4LogicalVolume* leadBlock1_log = new G4LogicalVolume(leadBlock1, Pb, "leadBlock1");
  G4LogicalVolume* leadBlock2_log = new G4LogicalVolume(leadBlock2, Pb, "leadBlock2");
  G4LogicalVolume* leadBlock3_log = new G4LogicalVolume(leadBlock3, Pb, "leadBlock3");

  // lead blocks 3 are the two next to NaI3 on the side table.
  new G4PVPlacement(NaI3_rotation, G4ThreeVector(NaI3_pos_x - 8*cm, NaI3_pos_y, NaI3_pos_z - 20*cm),
                        leadBlock3_log, "leadBlock3", experimentalHall_log, false, 0);
  new G4PVPlacement(NaI3_rotation, G4ThreeVector(NaI3_pos_x - 21.5*cm, NaI3_pos_y, NaI3_pos_z - 10*cm),
                        leadBlock3_log, "leadBlock3", experimentalHall_log, false, 0);

  // All blocks placed in experiment hall. These are just lead blocks we're placing ourselves around the experiment.
  new G4PVPlacement(0, G4ThreeVector(NaI2_pos_x, NaI_pos_y - 2.54*cm, NaI_pos_z + 6.56*cm),
			leadBlock1_log, "leadBlock1", experimentalHall_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(NaI2_pos_x, NaI_pos_y - 2.54*cm, NaI_pos_z - 6.56*cm),
			leadBlock1_log, "leadBlock1", experimentalHall_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(NaI1_pos_x, NaI_pos_y, NaI_pos_z + 6.56*cm),
			leadBlock2_log, "leadBlock2", experimentalHall_log, false, 0);
  new G4PVPlacement(0, G4ThreeVector(NaI1_pos_x, NaI_pos_y, NaI_pos_z - 6.56*cm),
			leadBlock2_log, "leadBlock2", experimentalHall_log, false, 0);
  G4RotationMatrix* xRot = new G4RotationMatrix;
  xRot -> rotateX(0.5*M_PI*rad);

  new G4PVPlacement(xRot, G4ThreeVector(11.92*cm, 25.505*cm, NaI_pos_z),
			leadBlock2_log, "leadBlock2", experimentalHall_log, false, 0);
  new G4PVPlacement(xRot, G4ThreeVector(-8.41*cm, 25.505*cm, NaI_pos_z),
			leadBlock2_log, "leadBlock2", experimentalHall_log, false, 0);
  new G4PVPlacement(xRot, G4ThreeVector(11.92*cm, 30.585*cm, NaI_pos_z),
			leadBlock2_log, "leadBlock2", experimentalHall_log, false, 0);
  new G4PVPlacement(xRot, G4ThreeVector(-8.41*cm, 30.585*cm, NaI_pos_z),
			leadBlock2_log, "leadBlock2", experimentalHall_log, false, 0);
  
  // -------------------------------------- Steel Aperture -----------------------------------------------------
 
  G4double aperture_x = 5*cm;
  G4double aperture_y = 0.2*cm; // 0.4cm thickness
  G4double aperture_z = 5*cm;

  G4double aperture_pos_x = 1.75*cm; // horn Offset
  //G4double aperture_pos_y = 23.46*cm + aperture_y; // (9.3inches)*cos(6.7deg)
  //G4double aperture_pos_z = -2.76*cm; // (-9.3inches)*sin(6.7deg)
  //9.3 inches from the slit is too large a distance
  //G4double aperture_pos_y = 26.28*cm + 2.0*aperture_y; //full length, therefore multiply with 0.5 below
   G4double aperture_pos_y = vacBoxInside_y - aperture_y;

   G4double aperture_pos_z = (-aperture_pos_y)*tan(hornTubeAngle); 
  //G4double aperture_pos_z = -2.76*cm;

  G4RotationMatrix* apertureRotation = new G4RotationMatrix();
  apertureRotation -> rotateX(90*deg);

  G4Box* aperture = new G4Box("aperture", aperture_x, aperture_y, aperture_z);
  aperture_log = new G4LogicalVolume(aperture, StSt316L, "aperture");
  
  //aperture_phys = new G4PVPlacement(0, G4ThreeVector(aperture_pos_x, (-vacBoxOffset_y + aperture_pos_y), (-vacBoxOffset_z + aperture_pos_z)), aperture_log, "aperture", vacBoxInside_log, false, 0); 
  aperture_phys = new G4PVPlacement(0, G4ThreeVector(aperture_pos_x, aperture_pos_y, (-vacBoxOffset_z + aperture_pos_z)), aperture_log, "aperture", vacBoxInside_log, false, 0);

  G4double apertureVacDiam = 2.38*cm; // 15/16 inches

  G4Tubs* apertureVac = new G4Tubs("apertureVac", 0, 0.5*apertureVacDiam, aperture_y, 0, 360*deg);
  apertureVac_log = new G4LogicalVolume(apertureVac, Vacuum, "apertureVac");
  apertureVac_phys = new G4PVPlacement(apertureRotation, G4ThreeVector(0, 0, 0), apertureVac_log, "apertureVac", aperture_log, false, 0);

  // ----------------------------- All components of the detector have been made now ---------------------------

  CreateScorers();	// register the NaI detectors made above as sensitive detectors.

  G4VisAttributes* NaIVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  NaI1_log->SetVisAttributes(NaIVisAtt);	// Setting some visualizations.
  NaI2_log->SetVisAttributes(NaIVisAtt);

  //always return the physical World
  return experimentalHall_phys;

}

void DetectorConstruction::CreateScorers()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();	// Need to declare an instance of SD manager
  SDman->SetVerboseLevel(1);				// verbosity level of the sensitive detector registration.

  G4MultiFunctionalDetector* cryst1 = new G4MultiFunctionalDetector("crystal1");        // crystal 1
  G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");
  cryst1->RegisterPrimitive(primitiv1);
  SDman->AddNewDetector(cryst1);
  NaI1_log->SetSensitiveDetector(cryst1);

  G4MultiFunctionalDetector* cryst2 = new G4MultiFunctionalDetector("crystal2");        // crystal 2
  G4VPrimitiveScorer* primitiv2 = new G4PSEnergyDeposit("edep");
  cryst2->RegisterPrimitive(primitiv2);
  SDman->AddNewDetector(cryst2);
  NaI2_log->SetSensitiveDetector(cryst2);

  G4MultiFunctionalDetector* cryst3 = new G4MultiFunctionalDetector("crystal3");        // crystal 3 = NaI3
  G4VPrimitiveScorer* primitiv3 = new G4PSEnergyDeposit("edep");			// NaI detector on side table
  cryst3->RegisterPrimitive(primitiv3);
  SDman->AddNewDetector(cryst3);
  NaI3_log->SetSensitiveDetector(cryst3);
  
/* New Detectors */
  
// filter needed to make the aperture a positron detector (we don't want to detect gamma-rays)
  G4String filterName, particleName;
  G4SDParticleFilter* positronFilter = new G4SDParticleFilter(filterName="positronFilter",particleName="e+"); 

// aperture detector - counts the number of positrons that pass through the aperture
  G4MultiFunctionalDetector* AV = new G4MultiFunctionalDetector("apertureVac");
  G4VPrimitiveScorer* primitiv4 = new G4PSEnergyDeposit("edep");
  AV->RegisterPrimitive(primitiv4);
  primitiv4->SetFilter(positronFilter);
  SDman->AddNewDetector(AV);
  apertureVac_log->SetSensitiveDetector(AV);

// horn cap detector - detects the positions where the positrons hit the horn cap 
  G4String SDname; 
  HornCapSD* sd = new HornCapSD(SDname = "HornCapSD");
  SDman->AddNewDetector(sd);
  hornBevel_log->SetSensitiveDetector(sd);
 
/* */

}
