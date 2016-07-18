#pragma once
#include <vector>
#include "card.h"
#include "pile_type.h"
#include "game_strategy.h"

class Pile {
  private:
    pile_type type; //type of pile
    std::vector<Card> cards; //cards of pile
  public:
    Pile(){};
    Pile(pile_type type, std::vector<Card> cards); //new pile with type and cards
    bool black(suit s); //is the suit black?
    pile_type get_type(); //what is the type
    std::vector<Card> get_cards(); //the cards
    void set_cards(std::vector<Card> card);
    void add_card(Card card);
    void add_cards(std::vector<Card> cards2, game_strategy strat);
    Card remove_card();
    //remove the last n cards if it is legal according to the strategy
    std::vector<Card> remove_cards(int n, game_strategy strat);
    //is this move legal according to strategy?
    bool is_legal_move(int n, game_strategy strat, 
                       std::vector<Card> cards2); 
    //is this move legal according to strategy?
    bool is_legal_move(int n, game_strategy strat);
    //if this is a stack, is legal to add the card? 
    bool is_legal_stack(Card card);
    bool empty();
    Card last();
    //the last n cards
    std::vector<Card> last(int n);
};