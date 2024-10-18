#include "lighting.h"

LightingState currentState;
LightingState previousState;

void initLighting(void) {
  // disableLight();
  currentState = DEFAULT_LIGHTING_STATE;
  previousState = DEFAULT_LIGHTING_STATE;
}

void handleLighting(void) {
  if (currentState == LIGHTING_STATE_AUTO) {

    // if (readIllumination() < 0.3) {
    //   enableLight();
    // } else {
    //   disableLight();
    // }
  }

  else if (currentState == LIGHTING_STATE_OFF) {
    // disableLight();
  }

  else if (currentState == LIGHTING_STATE_ON) {
    // enableLight();
  }

  else if (currentState == LIGHTING_STATE_SECURE) {
    
  }
}

void switchLightingState(LightingState state) {
  previousState = currentState;
  currentState = state;
}