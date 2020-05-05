
#include "YourDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

//new head files
#include "G4Orb.hh"



YourDetectorConstruction::YourDetectorConstruction() 
: G4VUserDetectorConstruction() {
  // set default target material
  G4String matName = "G4_Si";
  G4String matName2 = "G4_Al";
  fTargetMaterial  = G4NistManager::Instance()->FindOrBuildMaterial(matName);
  fSphereMaterial  = G4NistManager::Instance()->FindOrBuildMaterial(matName2);
  
  if (fTargetMaterial == nullptr) {
    G4cerr << "  ERROR YourDetectorConstruction() \n" 
           << "  Material with name " << matName << " was not found! \n"
           << G4endl;
  }
  // set default target thickness
  fTargetThickness = 1.0*CLHEP::cm;
  // initial gun-x position 
  fGunXPosition    = 0.0;
}

YourDetectorConstruction::~YourDetectorConstruction() {}


G4VPhysicalVolume* YourDetectorConstruction::Construct() {
  G4cout << " === YourDetectorConstruction::Construct() method === " << G4endl;
  // define dimensions
  G4double targetXSize  = fTargetThickness;
  G4double targetYZSize = 1.25*targetXSize;
  G4double worldXSize   = 1.1*targetXSize;
  G4double worldYZSize  = 1.1*targetYZSize;
  // compute gun-x position 
  fGunXPosition  = -0.25*(targetXSize+worldXSize);
  // 
  G4double zet      = 1.0;
  G4double amass    = 1.01*CLHEP::g/CLHEP::mole;
  G4double density  = CLHEP::universe_mean_density;
  G4double pressure = 3.0E-18*CLHEP::pascal;
  G4double tempture = 2.73*CLHEP::kelvin;
  G4Material* materialWorld = new G4Material("Galactic", zet, amass, density, 
                                             kStateGas, tempture, pressure); 
 
  // world 
  G4Box* worldSolid = new G4Box( "solid-World",   // name
                                 0.5*worldXSize,  // box half x-size
                                 0.5*worldYZSize, // box half y-size
                                 0.5*worldYZSize  // box half z-size
                                );   
  G4LogicalVolume* worldLogical = new G4LogicalVolume( worldSolid,    // solid 
                                                       materialWorld, // material                             
                                                       "logic-World"  // name
                                                     );                              
  G4VPhysicalVolume* worldPhysical = new G4PVPlacement( nullptr,                    // (no) rotation 
                                                        G4ThreeVector(0., 0., 0.),  // translation
                                                        worldLogical,               // logical volume
                                                        "World",                    // name
                                                        nullptr,                    // mother volume (!)
                                                        false,                      // don't care
                                                        0                           // cpy number
                                                      );

  // original target 
  G4Box*   targetSolid = new G4Box( "solid-Target", 
  																	0.5*targetXSize, 
  																	0.5*targetYZSize, 
  																	0.5*targetYZSize);
  G4LogicalVolume* targetLogical = new G4LogicalVolume(targetSolid, 
  																										fTargetMaterial, 
  																										"logic-Target");                                 
  G4VPhysicalVolume* targetPhysical = new G4PVPlacement(nullptr, 
  																											G4ThreeVector(0., 0., 0.), 
  																											targetLogical, 
  																											"Target", 
  																											worldLogical, 
  																											false,
  																											 0);
  
  // new spherical target
  G4Orb* sphereSolid = new G4Orb("solid-Sphere", 
  																0.25*targetXSize);
  G4LogicalVolume* sphereLogical = new G4LogicalVolume(sphereSolid, 
  																										fSphereMaterial, 
  																										"logic-Sphere");                                 
  G4VPhysicalVolume* spherePhysical = new G4PVPlacement(nullptr, 
  																											G4ThreeVector(0., 0., 0.), 
  																											sphereLogical, 
  																											"Sphere", 
  																											worldLogical, 
  																											false, 
  																											0);
  
  
  
  

   // return the root (i.e. world worldPhysical volume ptr)
   return worldPhysical;                                                         
}
