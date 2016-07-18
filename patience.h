#include "game.h"

class Patience : public Game {
  public:
    Patience();
    //Removes the top card of pile1 and adds it to the pile2, if it is legal
    void add_card_from(Pile pile1, Pile pile2);
    //Removes the top n card of pile1 and adds it to the pile2, if it is legal
    void add_cards_from(Pile found1, Pile found2, int n);
};