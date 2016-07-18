#include "patience.h"
#include <algorithm>
#include <stdexcept>      // std::invalid_argument

using namespace std;

Patience::Patience(){
  this->strat = game_strategy::alternate;
  std::vector<Card> cards = std::vector<Card>();
  //Compose the deck
  std::vector<suit> suits = std::vector<suit>();
  suits.push_back(suit::spades);
  suits.push_back(suit::hearts);
  suits.push_back(suit::clubs);
  suits.push_back(suit::diamonds);
  Card card = Card(suits.at(0), 1);
  for (int i = 0; i < suits.size(); i++){
    for (int j = card.get_min_value(); j <= card.get_max_value(); j++){
      Card card = Card(suits.at(i), j);
      cards.push_back(card);
    }
  }
  //Shuffle and deal
  std::random_shuffle ( cards.begin(), cards.end() );
  this->deck = cards;
  //waste and stacks are empty at first
  std::vector<Card> waste_cards = std::vector<Card>();
  Pile waste_ = Pile(pile_type::waste, waste_cards);
  //The numbers of stacks and foundations, resp. 
  int stack_num = 4;
  int foundation_num = 7;
  
  //Define the stacks
  std::vector<Pile> stacks;
  for (int i = 0; i < stack_num; i++){
    std::vector<Card> stack_cards = std::vector<Card>();
    Pile stack = Pile(pile_type::stack, stack_cards);
    stacks.push_back(stack);
  }

  //At first there are in all foundation so much card as its ordinal number
  std::vector<Pile> foundations;
  //Collect the foundation cards
  int k = 0;
  for (int i = 0; i < foundation_num; i++){
    std::vector<Card> foundation_cards = std::vector<Card>();
    //Add the cards to the foundations
    for (int j = 1; j <= i+1; j++){
      foundation_cards.push_back(deck.at(k));
      k++;
    }
    //Face the last cards
    (foundation_cards.at(foundation_cards.size()-1)).up();
    Pile foundation = Pile(pile_type::foundation, foundation_cards);
    foundations.push_back(foundation);
  }
  
  //Add the rest cards to the talon
  std::vector<Card> talon_cards;
  for (int i = k; i < deck.size(); i++){
    talon_cards.push_back(deck.at(i));
  }  
  Pile talon_ = Pile(pile_type::talon, talon_cards);
  //Collect all of piles
  this->piles = std::vector<Pile>();
  piles.push_back(waste_);
  for (int i = 0; i < stack_num; i++){
    piles.push_back(stacks.at(i));
  }
  for (int i = 0; i < foundation_num; i++){
    piles.push_back(foundations.at(i));
  }
  piles.push_back(talon_);

}

//Removes the top card of pile1 and adds it to the pile2, if it is legal
void Patience::add_card_from(Pile pile1, Pile pile2){
  if (pile1.get_type() == pile_type::talon){
    if (pile2.get_type() == pile_type::waste) {
      //The deck turns
      if (pile1.empty()){
        while (pile2.empty() == false){
          Card card = pile2.remove_card();
          card.down();
          pile1.add_card(card);
        }
      }
      else {
	//Add a card from the talon to the waste
        Card card = pile1.remove_card();
        pile2.add_card(card);
      }
    }
    else {
      throw std::invalid_argument("Ez a lépés nem szabályos.");
    }
  }
  if (pile2.get_type() == pile_type::foundation){
  //Add a card to a foundation
    if (pile2.empty()){
    //Empty foundation can begin only with king
      Card card = pile1.last();
      if (card.get_value() == card.get_max_value()){
        card = pile1.remove_card();
        pile2.add_card(card);            
      }
      else {
        throw 
          std::invalid_argument("Üres oszlopba csak király tehető.");          
        }
      }
      //Okay, it isn't empty
      else {
	//Before addition we must check if it is legal
        if (is_legal_move_to_foundation(pile1, pile2)){
	  //If it is legal, then move
          Card card = pile1.remove_card();
          pile2.add_card(card);
        } 
      }     
  }
  else if (pile2.get_type() == pile_type::stack){
    //Add a card to a stack
    Card card = pile1.last();
    if (pile2.is_legal_stack(card)) {
      pile1.remove_card();
      pile2.add_card(card);
    }
  } 
  else {
    throw std::invalid_argument("Ez a lépés nem szabályos.");
  }
}
 
//Removes the top n card of pile1 and adds them to the pile2, if it is legal
void Patience::add_cards_from(Pile found1, Pile found2, int n){
  if (n == 1){
    //If n = 1, then we use another function
    add_card_from(found1, found2);
  } 
  if (found1.get_type() == pile_type::waste){
    if (found2.get_type() == pile_type::talon){
      //The deck turns
      while (found1.empty() == false){
        Card card = found1.remove_card();
        found2.add_card(card);
      }
    }
  }
  if (found1.get_type() == pile_type::foundation){
    if (found2.get_type() == pile_type::foundation){     
      vector<Card> cas = vector<Card>();    
      cas = found1.get_cards();
      if (found2.empty()){
        //Empty foundation can begin only with king
        vector<Card> cards = vector<Card>();
        for (int i = cas.size()-1-n; i<cas.size(); i++){
          cards.push_back(cas.at(i));
        }        
        if ((is_legal_multi_move_to_foundation(found1, found2, n)) &&
          (cards.at(0)).get_value() == (cards.at(0)).get_max_value()){
	  cards.erase(cards.begin());
          found1.remove_cards(n, strat);
          found2.add_cards(cards, strat);        
        }
        else {
          throw std::invalid_argument("Üres oszlop csak királlyal kezdhető.");        
        }        
      }
      else {
        //Okay, it isn't empty
	vector<Card> cards = vector<Card>();
        for (int i = found1.get_cards().size()-1-n; i<found1.get_cards().size(); i++){
          cards.push_back(found1.get_cards().at(i));
        }
        if (is_legal_multi_move_to_foundation(found1, found2, n)){
          found1.remove_cards(n, strat);
          found2.add_cards(cards, strat);
        }
        else {
          throw std::invalid_argument("Ez a lépés nem szabályos.");
        }
      }
    }
  else {
    throw std::invalid_argument("Ez a lépés nem szabályos.");
  }
  }
}       
   