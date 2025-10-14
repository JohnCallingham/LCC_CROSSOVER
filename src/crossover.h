#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <Arduino.h>
#include "LCC_Node_Component_Base.h"
#include "servo_lcc.h"
#include <vector>
#include "servo_lcc.h" // For the servo position #defines.

/**
 * #defines for the indexes to the crossover positions vector.
 */
#define POS_CR_THROWN 0
#define POS_CR_CLOSED 1
#define POS_CR_UNKNOWN 2

class Position_Crossover {
  public:
    Position_Crossover(uint8_t positionNumber,
                      const char *positionDescription,
                      uint16_t eventMove,
                      uint16_t eventLeaving,
                      uint16_t eventReached);
    
    uint8_t getNumber() { return this->positionNumber; }
    const char *getDescription() { return this->positionDescription; }
    uint16_t getEventMove() { return this->eventMove; }
    uint16_t getEventLeaving() { return this->eventLeaving; }
    uint16_t getEventReached() { return this->eventReached; }

  private:
    uint8_t positionNumber;
    const char *positionDescription;
    uint16_t eventMove;
    uint16_t eventLeaving;
    uint16_t eventReached;
};

class Crossover : public LCC_Node_Component_Base {
  public:
    Crossover(Servo_LCC *servo0, Servo_LCC *servo1) { this->servo0 = servo0; this->servo1 = servo1; }

    void addPosition(uint8_t positionNumber,
                      const char *positionDescription,
                      uint16_t eventMove,
                      uint16_t eventLeaving,
                      uint16_t eventReached);

    bool eventIndexMatches(uint16_t index) override;
    bool eventIndexMatchesCurrentState(uint16_t index) override;

    void setInitialPosition(uint8_t initialPosition) { this->currentPosition = initialPosition; }

    void setEventToggle(uint16_t eventToggle) { this->eventToggle = eventToggle; }

    void eventReceived(uint16_t index);

    void sendEventsForCurrentState() override;

    // void process();
    void loop();

    bool isThrown() { return (currentPosition == POS_CR_THROWN) ? true : false; }
    bool isClosed() { return (currentPosition == POS_CR_CLOSED) ? true : false; }

  private:
      uint16_t eventToggle;

      Servo_LCC *servo0;
      Servo_LCC *servo1;

      std::vector<Position_Crossover> positions;

      uint8_t currentPosition;

      bool waitingForReachedThrown = false;
      bool waitingForReachedClosed = false;

      void moveServosToThrown();
      void moveServosToClosed();

      // Define the test cycles and track the current test cycle.
    enum Test { MOVE_TO_THROWN, WAIT_FOR_THROWN, MOVE_TO_CLOSED, WAIT_FOR_CLOSED };
    Test currentTest;

    void testLoop();
};

#endif
