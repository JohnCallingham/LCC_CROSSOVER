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
    void sendEventsForCurrentState() override;


  private:
      uint16_t eventToggle;

      std::vector<Position_Crossover> positions;

};

#endif
