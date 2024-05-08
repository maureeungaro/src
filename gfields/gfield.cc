// geant4
#include "G4Mag_UsualEqRhs.hh"
#include "G4DormandPrince745.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4SimpleHeum.hh"
#include "G4NystromRK4.hh"
#include "G4ImplicitEuler.hh"
#include "G4ExplicitEuler.hh"
#include "G4ChordFinder.hh"

// gfield
#include "gfield.h"


// notice: we are always using G4Mag_UsualEqRhs here
G4FieldManager *GField::create_FieldManager() {

    G4Mag_UsualEqRhs *iEquation = new G4Mag_UsualEqRhs(this);

    if (std::find(SUPPORTED_STEPPERS.begin(), SUPPORTED_STEPPERS.end(), integration_stepper) == SUPPORTED_STEPPERS.end()) {
        gFLogMessage("Integration Stepper " + integration_stepper + " not supported. Using default: " + GFIELD_DEFAULT_INTEGRATION_STEPPER);
        integration_stepper = GFIELD_DEFAULT_INTEGRATION_STEPPER;
    }

    G4MagIntegratorStepper *mag_int_stepper = nullptr;

    // any way to do this autmoatically?
    if (integration_stepper == "G4DormandPrince745") {
        mag_int_stepper = new G4DormandPrince745(iEquation);
    } else if (integration_stepper == "G4ClassicalRK4") {
        mag_int_stepper = new G4ClassicalRK4(iEquation);
    } else if (integration_stepper == "G4SimpleRunge") {
        mag_int_stepper = new G4HelixSimpleRunge(iEquation);
    } else if (integration_stepper == "G4HelixExplicitEuler") {
        mag_int_stepper = new G4HelixExplicitEuler(iEquation);
    } else if (integration_stepper == "G4HelixImplicitEuler") {
        mag_int_stepper = new G4HelixImplicitEuler(iEquation);
    } else if (integration_stepper == "G4CashKarpRKF45") {
        mag_int_stepper = new G4CashKarpRKF45(iEquation);
    } else if (integration_stepper == "G4RKG3_Stepper") {
        mag_int_stepper = new G4RKG3_Stepper(iEquation);
    } else if (integration_stepper == "G4SimpleHeum") {
        mag_int_stepper = new G4SimpleHeum(iEquation);
    } else if (integration_stepper == "G4NystromRK4") {
        mag_int_stepper = new G4NystromRK4(iEquation);
    } else if (integration_stepper == "G4ImplicitEuler") {
        mag_int_stepper = new G4ImplicitEuler(iEquation);
    } else if (integration_stepper == "G4ExplicitEuler") {
        mag_int_stepper = new G4ExplicitEuler(iEquation);
    } else {
        // error, exit
        gFLogMessage("Integration Stepper " + integration_stepper + " not supported. Exiting.");
        exit(EC__STEPPER_NOT_FOUND);
    }

    return new G4FieldManager(this, new G4ChordFinder(this, minimum_step, mag_int_stepper));

}
