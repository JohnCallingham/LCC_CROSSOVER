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
  Serial.printf("\nevent index 0x%02X received for crossover", index);

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
      // Serial.printf("\nmoving to position %d", targetPosition.getNumber());
      Serial.printf("\nmoving to position %s", targetPosition.getDescription());
    }



  }
}

void Crossover::sendEventsForCurrentState() {

}

void Crossover::process() {

}
