//******************************************************************************
// LICENSE DISCLAIMER:
// ===================

// Permission is granted to the public to copy and use this software
// without charge, provided that this Notice and any statement of
// authorship are reproduced on all copies.  Neither the Authors nor 
// Northern Michigan University makes any warranty, express or implied, 
// or assumes any liability or responsibility for the use of this software.
//
// DetectorConstruction.cc
//
//******************************************************************************
//

#include "DetectorConstruction.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


DetectorConstruction::DetectorConstruction()
{;}

DetectorConstruction::~DetectorConstruction()
{;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //------------------------------------------------------ materials
  
  // Vacuum, from PhysicalConstants.h
  //
  G4Material* matVac 
    = new G4Material("Vacuum", 1.0, 1.01*g/mole, universe_mean_density,
		     kStateGas, 2.73*kelvin, 3.e-18*pascal);
  
  // Detector materials
  //
  //G4Material* matC  = new G4Material("Carbon",  6., 12.01*g/mole,2.265*g/cm3);
  //G4double Thick = 0.589*mm;
  
    G4Material* matCu = new G4Material("Copper", 29., 63.55*g/mole,8.960*g/cm3);
  //  G4double Thick = 2.000*mm;

  G4Material* matPb = new G4Material("Lead",   82.,207.19*g/mole,11.35*g/cm3);
  //  G4double Thick = 0.500*mm;
    
  G4double z ;
  G4double density;
  G4int ncomponents;
  G4int natoms;
  
  // Define CH plastic for scintillators
  G4double a = 1.01*g/mole;
  G4Element* elH  = new G4Element("Hydrogen","H" , z= 1., a);
  a = 12.0107*g/mole;
  G4Element* elC  = new G4Element("Carbon"  ,"C" , z= 6., a);
  density = 1.030*g/cm3;
  G4Material* CH = new G4Material("Plastic",density,ncomponents=2);
  CH->AddElement(elC,natoms=10);
  CH->AddElement(elH,natoms=11);

  // Define NaI (sodium iodide) for NaI detectors
  a=22.98977*g/mole;
  G4Element* elNa = new G4Element("Sodium","Na", z=11.,a);
  a=126.9045*g/mole;
  G4Element* elI = new G4Element("Iodine","I", z=53.,a);
  density = 3.67*g/cm3;
  G4Material* NaI = new G4Material("NaIdet",density,ncomponents=2);
  NaI->AddElement(elNa,natoms=1);
  NaI->AddElement(elI,natoms=1);

  //------------------------------------------------------ volumes

  //------------------------------ (world volume)
  G4Box* solidOrig
    = new G4Box("Orig", 30.0*cm, 30.0*cm, 30.0*cm);

  G4LogicalVolume* logicOrig
    = new G4LogicalVolume(solidOrig, matVac, "Orig", 0, 0, 0);

  G4VPhysicalVolume* physiOrig
    = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicOrig,
                        "Orig", 0, false, 0);

  //  (*logicOrig).SetVisAttributes(G4VisAttributes::Invisible);
  (*logicOrig).SetVisAttributes(G4VisAttributes::wireframe);

 //----------------------------- Pb shield box

  G4Box* solidShield
    = new G4Box("Shield",15.24*cm,15.24*cm,15.24*cm);

  G4LogicalVolume* logicShield
   = new G4LogicalVolume(solidShield, matPb, "Shield", 0, 0, 0);

   new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicShield,"Shield", logicOrig, false, 0);
   
   G4VisAttributes*ShieldVisAtt = new G4VisAttributes(G4Colour(0.,1.,1.));
   ShieldVisAtt->SetForceWireframe(true);
   logicShield->SetVisAttributes(ShieldVisAtt);				      
  //---------------------------- Copper liner 4 mm thick

   G4Box* solidLiner
     =new G4Box("Liner",10.16*cm,10.16*cm,10.16*cm);

   G4LogicalVolume* logicLiner
     = new G4LogicalVolume(solidLiner, matCu, "Liner", 0, 0,0);

   new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicLiner,"Liner", logicShield, false, 0);
   
   G4VisAttributes*LinerVisAtt = new G4VisAttributes(G4Colour(1.,0.,0.));
   LinerVisAtt->SetForceWireframe(true);
   logicLiner->SetVisAttributes(LinerVisAtt);	

  //------------------------------ Inside the shield

  G4Box* solidInside
    = new G4Box("Inside",9.96*cm,9.96*cm,9.96*cm);

  G4LogicalVolume* logicInside
    = new G4LogicalVolume(solidInside, matVac, "Inside", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicInside,"Inside", logicLiner, false, 0);
  
   G4VisAttributes*InsideVisAtt = new G4VisAttributes(G4Colour(1.,1.,0.));
   InsideVisAtt->SetForceWireframe(true);
   logicInside->SetVisAttributes(InsideVisAtt);	
  //------------------------------ scatterer

  G4Tubs* solidD1
    = new G4Tubs("D1", 0.0*cm, 2.54*cm, 2.54*cm, 0.0*deg, 360.0*deg);

  G4LogicalVolume* logicD1
    = new G4LogicalVolume(solidD1, NaI, "D1", 0, 0, 0);

  new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicD1,
		    "D1", logicInside, false, 0);


  //------------------------------------------------------------------
  // Must return pointer to the master physical volume
  //
  return physiOrig;
}

