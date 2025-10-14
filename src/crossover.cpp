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
  // Check for one of the testing event indexes.
  if ((index == this->testStartEventIndex) ||
      (index == this->testStopEventIndex)) return true;

  // Check for the toggle event index.
  if (index == this->eventToggle) return true;

  // Check for one of the position event indexes.
  for (auto & position : positions) {
    if ((index == position.getEventMove()) ||
        (index == position.getEventLeaving()) ||
        (index == position.getEventReached())) return true;
  }

  return false;
}

bool Crossover::eventIndexMatchesCurrentState(uint16_t index) {
  // Determine which of the crossover's four events this event index is for.
  if (index == positions[POS_CR_THROWN].getEventReached()) {
    if (this->currentPosition == POS_CR_THROWN) return true;
  }
  if (index == positions[POS_CR_THROWN].getEventLeaving()) {
    if (this->currentPosition != POS_CR_THROWN) return true;
  }
  if (index == positions[POS_CR_CLOSED].getEventReached()) {
    if (this->currentPosition == POS_CR_CLOSED) return true;
  }
  if (index == positions[POS_CR_CLOSED].getEventLeaving()) {
    if (this->currentPosition != POS_CR_CLOSED) return true;
  }

  return false;
}

void Crossover::eventReceived(uint16_t index) {
  // Serial.printf("\nevent index 0x%02X received for crossover", index);

  /**
   * Handle the test cycle start and stop events.
   */
  if (index == testStartEventIndex) {
    Serial.printf("\nCrossover starting the testing cycle.");

    // Set the first test.
    currentTest = MOVE_TO_THROWN;

    // Set the timer so that testing starts immediately.
    testingTimer = millis();

    testing = true;
  }
  if (index == testStopEventIndex) {
    Serial.printf("\nCrossover stopping the testing cycle.");

    // Leave both servos at their mid position.
    //servoEasing.moveTo(positions[POS_MID].getAngle());

    // TO DO: need to send the appropriate leave and reached events.
    // The servo may be moving or not !!!

    testing = false;
  }

  /**
   * Handle the toggle event.
   */
  if (index == eventToggle) {
    Serial.printf("\ntoggle position");

    // // If the crossover's current position is unknown take no action.
    // Used for quick initialisation of both points after a power up.
    if (this->currentPosition == POS_CR_UNKNOWN) {
      moveServosToClosed(); // === move both servos to closed (arbitrarily chosen) ===
      return;
    }

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
  uint8_t servo0TargetAngle;
  uint8_t servo1TargetAngle;

  // Get each servo's current angle.
  uint8_t servo0CurrentAngle = servo0->servoEasing.getCurrentAngle();
  uint8_t servo1CurrentAngle = servo1->servoEasing.getCurrentAngle();

  // Determine the target position for this event.
  if (index == positions[POS_CR_THROWN].getEventMove()) {
    // This is a move to Thrown event.
    Serial.printf("\ncrossover moving to position %d (%s)", positions[POS_CR_THROWN].getNumber(), positions[POS_CR_THROWN].getDescription());

    // If the crossover's target position number is 0 (Thrown), then the servo's target position is 0 (Thrown).
    servo0TargetAngle = servo0->getAngleForPosition(POS_THROWN);
    servo1TargetAngle = servo1->getAngleForPosition(POS_THROWN);

    /**
     * Check to see if both servos are already at the target position.
     * If so then just send the crossover's reached event for the target position
     * and return.
     */
    if ((servo0CurrentAngle == servo0TargetAngle) && (servo1CurrentAngle == servo1TargetAngle)) {
      if (sendEvent) sendEvent(positions[POS_CR_THROWN].getEventReached());
      return;
    }

    // If we get here then at least one servo needs to move.

    // Send the crossover's leaving event for its current position.
    // Do not send anything if the crossover's current position is Unknown.
    if (sendEvent) sendEvent(positions[POS_CR_CLOSED].getEventLeaving());

    moveServosToThrown();
  }

  if (index == positions[POS_CR_CLOSED].getEventMove()) {
    // This is a move to Closed event.
    Serial.printf("\ncrossover moving to position %d (%s)", positions[POS_CR_CLOSED].getNumber(), positions[POS_CR_CLOSED].getDescription());

    // If the crossover's target position number is 0 (Thrown), then the servo's target position is 0 (Thrown).
    servo0TargetAngle = servo0->getAngleForPosition(POS_CLOSED);
    servo1TargetAngle = servo1->getAngleForPosition(POS_CLOSED);

    /**
     * Check to see if both servos are already at the target position.
     * If so then just send the crossover's reached event for the target position
     * and return.
     */
    if ((servo0CurrentAngle == servo0TargetAngle) && (servo1CurrentAngle == servo1TargetAngle)) {
      if (sendEvent) sendEvent(positions[POS_CR_CLOSED].getEventReached());
      return;
    }

    // If we get here then at least one servo needs to move.

    // Send the crossover's leaving event for its current position.
    // Do not send anything if the crossover's current position is Unknown.
    if (sendEvent) sendEvent(positions[POS_CR_THROWN].getEventLeaving());

    moveServosToClosed();
  }
}

void Crossover::moveServosToThrown() {
  // servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(POS_THROWN));
  servo0->servoEasing.easeTo(servo0->getAngleForPosition(POS_THROWN));
  // servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(POS_THROWN));
  servo1->servoEasing.easeTo(servo1->getAngleForPosition(POS_THROWN));
  if (sendEvent) sendEvent(servo0->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo0.
  if (sendEvent) sendEvent(servo1->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo1.
  waitingForReachedThrown = true; // So we can send the reached event when appropriate.
}

void Crossover::moveServosToClosed() {
  // servo0->servoEasing.setTargetAngle(servo0->getAngleForPosition(POS_CLOSED));
  servo0->servoEasing.easeTo(servo0->getAngleForPosition(POS_CLOSED));
  // servo1->servoEasing.setTargetAngle(servo1->getAngleForPosition(POS_CLOSED));
  servo1->servoEasing.easeTo(servo1->getAngleForPosition(POS_CLOSED));
  if (sendEvent) sendEvent(servo0->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo0.
  if (sendEvent) sendEvent(servo1->getLeavingEventForCurrentAngle()); // Send the leaving closed event for servo1.
  waitingForReachedClosed = true; // So we can send the reached event when appropriate.
}

void Crossover::sendEventsForCurrentState() {
  switch (this->currentPosition) {
    case POS_CR_THROWN:
      if (sendEvent) sendEvent(positions[POS_CR_THROWN].getEventReached());
      break;

    case POS_CR_CLOSED:
      if (sendEvent) sendEvent(positions[POS_CR_CLOSED].getEventReached());
      break;

    case POS_CR_UNKNOWN:
      if (sendEvent) sendEvent(positions[POS_CR_THROWN].getEventLeaving());
      if (sendEvent) sendEvent(positions[POS_CR_CLOSED].getEventLeaving());
      break;
  }
}

void Crossover::loop() {
  /**
   * Keep checking for both servos having reached their target position.
   */
  if (waitingForReachedThrown) {
    // If both servos have reached their thrown position, send the crossover's reached thrown event
    //  and update the crossover's current posotion.
    if ((servo0->isThrown()) && (servo1->isThrown())) {
      waitingForReachedThrown = false;
      if (sendEvent) sendEvent(this->positions[POS_CR_THROWN].getEventReached());
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

  if (testing) testLoop();
}

void Crossover::testLoop() {
  if (millis() < testingTimer) return;

  // Time to move to the next part of the test cycle.

  switch (currentTest) {
    case MOVE_TO_THROWN:
      if (sendEvent) sendEvent(positions[POS_CR_CLOSED].getEventLeaving());
      moveServosToThrown();
      waitingForReachedThrown = true;
      testingTimer = millis() + 1000;
      currentTest = WAIT_FOR_THROWN;
      break;
    
    case WAIT_FOR_THROWN:
      if (currentPosition == POS_CR_THROWN) {
        waitingForReachedThrown = false;
        testingTimer = millis() + 1000;
        currentTest = MOVE_TO_CLOSED;
      }
      break;
    
    case MOVE_TO_CLOSED:
      if (sendEvent) sendEvent(positions[POS_CR_THROWN].getEventLeaving());
      moveServosToClosed();
      waitingForReachedClosed = true;
      testingTimer = millis() + 1000;
      currentTest = WAIT_FOR_CLOSED;
      break;
    
    case WAIT_FOR_CLOSED:
      if (currentPosition == POS_CR_CLOSED) {
        waitingForReachedClosed = false;
        testingTimer = millis() + 1000;
        currentTest = MOVE_TO_THROWN;
      }
      break;
    
    default:
      break;
  }
}
