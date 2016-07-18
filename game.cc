#include "game.h"
#include <algorithm>
#include <stdexcept>      // std::invalid_argument
#include <vector>

using namespace std;

Game::Game(game_strategy strat, std::vector<Pile> piles, 
         std::vector<Card> deck){
   std::vector<Card> myvector = std::vector<Card>(deck);
   std::random_shuffle ( myvector.begin(), myvector.end() );
   this->strat = strat;
   this->piles = piles;
   this->deck = myvector;
}

game_strategy Game::get_strat(){
  return this->strat;
}

std::vector<Pile> Game::get_piles(){
  return this->piles;
}

std::vector<Card> Game::get_deck(){
  return this->deck;
}   

/*
//How many cards can be put from found1 to found2 at once
int Game::legal_length(Pile found1, Pile found2){
  //If found2 isn't a foundation, then it can only accept one card.
  if (found2.get_type() != pile_type::foundation) {
    return 1;
  }
  //If found1 is't a foundation, then it can give only one card.
  else if (found1.get_type() != pile_type::foundation) {
    return 1;
  }
  else {
    //Empty foundation cann't give a card.
    if (found1.empty()){
      return 0;
    }
    else {
      Card card = found1.last();
      int l = card.get_max_value() - card.get_min_value() + 1;
      //Not by calculation, but by experiment
      for (int i = l-1; i >= 0; i--) {
        std::vector<Card> cards = found1.get_cards();
        std::vector<Card> cars = std::vector<Card>();
        if (!found2.empty()){
          cars.push_back(found2.last());
        }
        for (int j = cards.size()-i; j < cards.size(); j++){
          cars.push_back(cards.at(j));
        }
        if (found2.is_legal_move(cars.size(), strat, cars)){
          return i;
        }
      }
    }
  }
}
*/

bool Game::is_legal_move_to_foundation(Pile pile1, Pile found){
  if (pile1.empty()) {throw 
              std::invalid_argument("Üres helyről nem vehető el lap.");}
  vector<Card> cards = vector<Card>();
  cards.push_back(found.last());
  cards.push_back(pile1.last());
  return (found.is_legal_move(2, strat, cards));
  
}

bool Game::is_legal_multi_move_to_foundation(Pile pile1, Pile found2, int n){
  if (pile1.empty()) {throw 
              std::invalid_argument("Üres helyről nem vehető el lap.");}
  if (found2.empty()){ 
    vector<Card> cas = vector<Card>();
    cas = pile1.get_cards();
    vector<Card> cards = vector<Card>();
    if (found2.empty()){
      for (int i = cas.size()-1-n; i<cas.size(); i++){
        cards.push_back(cas.at(i));
      }
    return (found2.is_legal_move(cards.size(), strat, cards) );
    }
  }
  else {
    Card card = found2.last();//Ezt meg kell nézni
    vector<Card> cards = vector<Card>();
    cards.push_back(card);
    vector<Card> cas = vector<Card>();             
    cas = pile1.get_cards();
    for (int i = cas.size()-1-n; i<cas.size(); i++){
      cards.push_back(cas.at(i));
    }
  return (found2.is_legal_move(cards.size(), strat, cards));
  }
}
