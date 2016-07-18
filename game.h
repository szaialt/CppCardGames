#pragma once
#include <vector>
#include "pile.h"

//Common ancestor of game classes
class Game {
  protected:
    game_strategy strat; // is it alternate or constant?
    //Alternete: you can move cards that are alternate red-black colors.
    //Constant: you can move only constant suit cards.
    //The piles that contain the cards
    std::vector<Pile> piles;
    //The cards used it the game
    std::vector<Card> deck;
  public:
    Game(){};
    Game(game_strategy strat, std::vector<Pile> piles, 
         std::vector<Card> deck);
    game_strategy get_strat();
    std::vector<Pile> get_piles();
    std::vector<Card> get_deck();
    //Removes the top card of pile1 and adds it to the pile2, if it is legal
    virtual void add_card_from(Pile pile1, Pile pile2) = 0;
    //Removes the top n card of pile1 and adds it to the pile2, if it is legal
    virtual void add_cards_from(Pile found1, Pile found2, int n) = 0;
    //How many cards can be put from found1 to found2 at once
    //virtual int legal_length(Pile found1, Pile found2);
    bool is_legal_move_to_foundation(Pile pile1, Pile found);
    bool is_legal_multi_move_to_foundation(Pile pile1, Pile found2, int n);
};