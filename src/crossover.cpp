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

    // If the crossover's current position is unknown take no action.
    if (this->currentPosition == POS_CR_UNKNOWN) return;

    // If the crossover's current position is Thrown,
    //  send the Thrown leaving event and start the servos moving to Closed.
    if (this->currentPosition == POS_CR_THROWN) {
      if (sendEvent) sendEvent(positions[POS_CR_THROWN].getEventLeaving());
      moveServosToClosed();
      return;
    }

    // If the crossover's current position is Closed,
    //  send the Closed leaving event and start the servos moving to Thrown.
    if (this->currentPosition == POS_CR_CLOSED) {
      if (sendEvent) sendEvent(positions[POS_CR_CLOSED].getEventLeaving());
      moveServosToThrown();
      return;
    }
  }

  /***
   * Handle the move to a position event.
   */
  // Determine the target position for this event.
  for (auto & targetPosition : positions) {
    if (index == targetPosition.getEventMove()) {
      Serial.printf("\ncrossover moving to position %d (%s)", targetPosition.getNumber(), targetPosition.getDescription());

      /**
       * Check to see if both servos are already at the target position.
       * If so then just send the crossover's reached event for the target position
       * and return.
       */
      // Get each servo's current angle.
      uint8_t servo0CurrentAngle = servo0->servoEasing.getCurrentAngle();
      uint8_t servo1CurrentAngle = servo1->servoEasing.getCurrentAngle();

      // Get each servo's target angle for the crossover's target position.
      uint8_t servo0TargetAngle;
      uint8_t servo1TargetAngle;

      uint8_t targetPositionNumber = targetPosition.getNumber();

      // If the crossover's target position number is 0 (Thrown), then the servo's target position is 0 (Thrown).
      if (targetPositionNumber == POS_CR_THROWN) {
        servo0TargetAngle = servo0->getAngleForPosition(POS_THROWN);
        servo1TargetAngle = servo1->getAngleForPosition(POS_THROWN);
      }
      // If the crossover's target position number is 1 (Closed), then the servo's target position is 2 (Closed).
      if (targetPositionNumber == POS_CR_CLOSED) {
        servo0TargetAngle = servo0->getAngleForPosition(2);
        servo1TargetAngle = servo1->getAngleForPosition(2);
      }

      if ((servo0CurrentAngle == servo0TargetAngle) && (servo1CurrentAngle == servo1TargetAngle)) {
        if (sendEvent) sendEvent(positions[targetPositionNumber].getEventReached());
        return;
      }

      // If we get here then at least one servo needs to move.

      // Send the crossover's leaving event for its current position.
      // Do not send anything if the crossover's current position is Unknown.
      if ((this->currentPosition == POS_CR_THROWN) || (this->currentPosition == POS_CR_CLOSED)) {
        if (sendEvent) sendEvent(positions[currentPosition].getEventLeaving());
      }

      // Start both servos moving to their target angles.
      // Need to send their leaving events here.
      if (targetPositionNumber == POS_CR_THROWN) {
        // The crossover's target position is 0 (Thrown). Each servo's target position is 0 (Thrown).
        moveServosToThrown();
        // servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(POS_THROWN));
        // servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(POS_THROWN));
        // if (sendEvent) sendEvent(servo0->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo0.
        // if (sendEvent) sendEvent(servo1->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo1.
        // waitingForReachedThrown = true; // So we can send the reached event when appropriate.
      } else if (targetPositionNumber == POS_CR_CLOSED) {
        // The crossover's target position is 1 (Closed). Each servo's target position is 2 (Closed).
        moveServosToClosed();
        // servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(POS_CLOSED));
        // servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(POS_CLOSED));
        // if (sendEvent) sendEvent(servo0->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo0.
        // if (sendEvent) sendEvent(servo1->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo1.
        // waitingForReachedClosed = true; // So we can send the reached event when appropriate.
      } else {
        Serial.printf("\nerror in Crossover::eventReceived()");
      }
    }
  }
}

void Crossover::moveServosToThrown() {
  servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(POS_THROWN));
  servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(POS_THROWN));
  if (sendEvent) sendEvent(servo0->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo0.
  if (sendEvent) sendEvent(servo1->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo1.
  waitingForReachedThrown = true; // So we can send the reached event when appropriate.
}

void Crossover::moveServosToClosed() {
  servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(POS_CLOSED));
  servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(POS_CLOSED));
  if (sendEvent) sendEvent(servo0->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo0.
  if (sendEvent) sendEvent(servo1->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo1.
  waitingForReachedClosed = true; // So we can send the reached event when appropriate.
}

void Crossover::sendEventsForCurrentState() {

}

void Crossover::process() {
  // Keep checking for both servos having reached their target position.
  if (waitingForReachedThrown) {
    // If both servos have reached their thrown position, send the crossover's reached thrown event
    //  and update the crossover's current posotion.
    if ((servo0->isThrown()) && (servo1->isThrown())) {
      waitingForReachedThrown = false;
      // if (sendEvent) sendEvent(this->positions[0].getEventReached());
      if (sendEvent) sendEvent(this->positions[POS_CR_THROWN].getEventReached());
      // this->currentPosition = 0;
      this->currentPosition = POS_CR_THROWN;
    }
  }

  if (waitingForReachedClosed) {
    // If both servos have reached their closed position, send the crossover's reached closed event
    //  and update the crossover's current posotion.
    if ((servo0->isClosed()) && (servo1->isClosed())) {
      waitingForReachedClosed = false;
      if (sendEvent) sendEvent(this->positions[POS_CR_CLOSED].getEventReached());
      this->currentPosition = POS_CR_CLOSED;
    }
  }
}
