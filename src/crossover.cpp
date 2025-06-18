#include "crossover.h"

Position_Crossover::Position_Crossover(uint8_t positionNumber,
                      const char *positionDescription,
                      uint16_t eventMove,
                      uint16_t eventLeaving,
                      uint16_t eventReached) {
  this->positionNumber = positionNumber;
  this->positionDescription = positionDescription;
  this->eventMove = eventMove;
  this->eventLeaving = eventLeaving;
  this->eventReached = eventReached;
}

void Crossover::addPosition(uint8_t positionNumber,
                          const char *positionDescription,
                          uint16_t eventMove,
                          uint16_t eventLeaving,
                          uint16_t eventReached) {
  positions.push_back(Position_Crossover (positionNumber,
                        positionDescription,
                        eventMove,
                        eventLeaving,
                        eventReached));
}

bool Crossover::eventIndexMatches(uint16_t index) {
  if (index == this->eventToggle) return true;

  for (auto & position : positions) {
    if ((index == position.getEventMove()) ||
        (index == position.getEventLeaving()) ||
        (index == position.getEventReached())) return true;
  }

  return false;
}

bool Crossover::eventIndexMatchesCurrentState(uint16_t index) {

  return false;
}

void Crossover::eventReceived(uint16_t index) {
  // Serial.printf("\nevent index 0x%02X received for crossover", index);

  /**
   * Handle the toggle event.
   */
  if (index == eventToggle) {
    Serial.printf("\ntoggle position");



  }

  /***
   * Handle the move to a position event.
   */
  // Determine the target position for this event.
  for (auto & targetPosition : positions) {
    if (index == targetPosition.getEventMove()) {
      Serial.printf("\ncrossover moving to position %d (%s)", targetPosition.getNumber(), targetPosition.getDescription());

      // Check to see if both servos are already at the target position. If so then just send the crossover's reached event and return.
      
      // Check to see if either servo is already at the target position. Do we need to get their current angles to do this?

      // If we get here then at least one servo needs to move.
      // Send the crossover's leaving event.

      


      // Start both servos moving to their target angles.
      if (targetPosition.getNumber() == 0) {
        // The crossover's target position is 0 (Thrown). Each servo's target position is 0 (Thrown).
        servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(0));
        servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(0));
      } else if (targetPosition.getNumber() == 1) {
        // The crossover's target position is 1 (Closed). Each servo's target position is 2 (Closed).
        servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(2));
        servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(2));
      } else {
        Serial.printf("\nerror in Crossover::eventReceived()");
      }
    }
  }
}

void Crossover::sendEventsForCurrentState() {

}

void Crossover::process() {

}
