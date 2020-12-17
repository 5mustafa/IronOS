#include "BSP.h"
#include "BSP_Power.h"
#include "QC3.h"
#include "Settings.h"
#include "Pins.h"
#include "fusbpd.h"
#include "Model_Config.h"
#include "policy_engine.h"
#include "int_n.h"
bool FUSB302_present = false;

void power_check() {
#ifdef POW_PD
	if (FUSB302_present) {
		//Cant start QC until either PD works or fails
		if (PolicyEngine::setupCompleteOrTimedOut() == false) {
			return;
		}
		if (PolicyEngine::pdHasNegotiated()) {
			return;
		}
	}
#endif
#ifdef POW_QC
	QC_resync();
#endif
}
uint8_t usb_pd_detect() {
#ifdef POW_PD
	FUSB302_present = fusb302_detect();

	return FUSB302_present;
#endif
	return false;
}

bool getIsPoweredByDCIN() {
	//We return false until we are sure we are not using PD
	if (PolicyEngine::setupCompleteOrTimedOut() == false) {
		return false;
	}
	if (PolicyEngine::pdHasNegotiated()) {
		return false;	// We are using PD
	}
	if (hasQCNegotiated()) {
		return false; // We are using QC
	}
	return true;
}
