#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <Arduino.h>
#include "LCC_Node_Component_Base.h"
#include <vector>

class Position_Crossover {
  public:
    Position_Crossover(uint8_t positionNumber,
                      const char *positionDescription,
                      uint16_t eventMove,
                      uint16_t eventLeaving,
                      uint16_t eventReached);



  private:
    uint8_t positionNumber;
    const char *positionDescription;
    uint16_t eventMove;
    uint16_t eventLeaving;
    uint16_t eventReached;
};

class Crossover : public LCC_Node_Component_Base {
  public:
    void addPosition(uint8_t positionNumber,
                      const char *positionDescription,
                      uint16_t eventMove,
                      uint16_t eventLeaving,
                      uint16_t eventReached);



    bool eventIndexMatches(uint16_t index) override;
    bool eventIndexMatchesCurrentState(uint16_t index) override;

    void setEventToggle(uint16_t eventToggle) { this->eventToggle = eventToggle; }

    void eventReceived(uint16_t index);

    void sendEventsForCurrentState() override;

    void process();

    bool isThrown() { return false; } // to be completed
    bool isClosed() { return false; } // to be completed

  private:
      uint16_t eventToggle;

      std::vector<Position_Crossover> positions;

};

#endif
