#include "game.h"

class Freecell : public Game {
  public:
    Freecell();
    //Removes the top card of pile1 and adds it to the pile2, if it is legal
    void add_card_from(Pile pile1, Pile pile2);
    //Removes the top k cards of pile1 and adds it to the pile2, if it is legal
    void add_cards_from(Pile found1, Pile found2, int k);
    //How much empty freecells we have
    int empty_freecells();
};