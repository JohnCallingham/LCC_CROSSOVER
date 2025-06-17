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

  return false;
}

bool Crossover::eventIndexMatchesCurrentState(uint16_t index) {

  return false;
}

void Crossover::eventReceived(uint16_t index) {

}

void Crossover::sendEventsForCurrentState() {

}

void Crossover::process() {

}
