#include "spider.h"
#include <algorithm>
#include <stdexcept>      // std::invalid_argument

using namespace std;

Spider::Spider(int n){
  int suit_num = 1;
  if (n == 2)
    suit_num = n;
  else if (n == 4)
    suit_num = n;
  this->strat = game_strategy::constant;
  
  int foundations_cards_num = 54;
  std::vector<Card> cards = std::vector<Card>();

  //Compose the deck
  std::vector<suit> suits = std::vector<suit>();
  //Using 4 suits
  if (suit_num == 4) { 
      
  suits.push_back(suit::spades);
  suits.push_back(suit::hearts);
  suits.push_back(suit::clubs);
  suits.push_back(suit::diamonds);  
   
    Card card = Card(suits.at(0), 1);
    for (int i = 0; i < suits.size(); i++){
      for (int j = card.get_min_value(); j <= card.get_max_value(); j++){
        Card card = Card(suits.at(i), j);
        cards.push_back(card);
        Card card2 = Card(suits.at(i), j);
        cards.push_back(card2);
      }
    }
  }
  //Using 2 suits
  //Define the suits at random
  int suits_ = rand() % 4;
  if (suit_num == 2){
    if (suits_ == 0) {
      suits.push_back(suit::spades);
      suits.push_back(suit::hearts);
    }
    else if (suits_ == 1) {
      suits.push_back(suit::clubs);
      suits.push_back(suit::diamonds);        
    }
    else if (suits_ == 2) {
      suits.push_back(suit::spades);
      suits.push_back(suit::diamonds);        
    }   
    else if (suits_ == 3) {
      suits.push_back(suit::hearts);
      suits.push_back(suit::clubs);
    }
        Card card = Card(suits.at(0), 1);
    for (int i = 0; i < suits.size(); i++){
      for (int j = card.get_min_value(); j <= card.get_max_value(); j++){
        for (int l = 0; l < 4; l++){
          Card card = Card(suits.at(i), j);
          cards.push_back(card);
        }
      }
    }
  }
  //Using 1 suit
  //Define the suit at random
  if (suit_num == 1){
    if (suits_ == 0) {
      suits.push_back(suit::hearts);
    }
    else if (suits_ == 1) {
      suits.push_back(suit::diamonds);        
    }
    else if (suits_ == 2) {
      suits.push_back(suit::spades);
    }   
    else if (suits_ == 3) {
      suits.push_back(suit::clubs);
    }
        Card card = Card(suits.at(0), 1);
    for (int i = 0; i < suits.size(); i++){
      for (int j = card.get_min_value(); j <= card.get_max_value(); j++){
        for (int l = 0; l < 8; l++){
          Card card = Card(suits.at(i), j);
          cards.push_back(card);
        }
      }
    }
  }  
  
  
  
  //Shuffle and deal
  std::random_shuffle ( cards.begin(), cards.end() );
  this->deck = cards;  
  
  int stack_num = 8;
  int foundation_num = 10;
  std::vector<Pile>  stacks;
  for (int i = 0; i < stack_num; i++){
    std::vector<Card> stack_cards = std::vector<Card>();
    Pile stack = Pile(pile_type::stack, stack_cards);
    stacks.push_back(stack);
   }

   std::vector<Pile> foundations;
   int r = foundations_cards_num % foundation_num;
   int d = foundations_cards_num / foundation_num;
  //Collect the foundation cards
  for (int i = 0; i < foundation_num; i++){
    std::vector<Card> foundation_cards = std::vector<Card>();
    //Add the cards to the foundations
    for (int k = 0; k < d; k++){
      foundation_cards.push_back(deck.at(i*d+k));
    }
    if (i < r) {
      foundation_cards.push_back(deck.at(deck.size() - i - 1));
    }
    //Face the last cards
    (foundation_cards.at(foundation_cards.size()-1)).up();
    Pile foundation = Pile(pile_type::foundation, foundation_cards);
    foundations.push_back(foundation);
  }
  //The talon cards
  std::vector<Card> talon_cards = std::vector<Card>();
  for (int k = foundations_cards_num - r; k < deck.size() - r; k++){
    talon_cards.push_back(deck.at(k));
  }
  
  Pile talon_ = Pile(pile_type::talon, talon_cards);

  //Collect all of piles
  this->piles = std::vector<Pile>();
  for (int i = 0; i < stack_num; i++){
    piles.push_back(stacks.at(i));
  }
  
  for (int i = 0; i < foundation_num; i++){
    piles.push_back(foundations.at(i));
  }
  piles.push_back(talon_);
  
}

//Removes the top card of pile1 and adds it to the pile2, if it is legal
void Spider::add_card_from(Pile pile1, Pile pile2){
  if (pile1.get_type() == pile_type::foundation){
    if (pile2.get_type() == pile_type::foundation){//Üres kezelése?
      //From a foundation to a foundation
      if (pile2.empty()){
	//The another foundation is empty
          Card card = pile1.remove_card();
          pile2.add_card(card);            
          }
       else {
	  //Before addition we must check if it is legal
          if (is_legal_move_to_foundation(pile1, pile2)){
	    //If it is legal, then move
            Card card = pile1.remove_card();
            pile2.add_card(card);
        } 
        else {
          throw 
          std::invalid_argument("Ez a lépés nem szabályos.");          
        }
      }
    }
  }
}

//Removes the top n card of pile1 and adds it to the pile2, if it is legal
void Spider::add_cards_from(Pile found1, Pile found2, int n){
  //Dealing
  Pile pile1 = found1;
  Pile pile2 = found2;
  if (pile1.get_type() == pile_type::talon){
    if (pile2.get_type() == pile_type::foundation){
      //From the talon to all foundations
      for (int i = 0; i < piles.size(); i++){
        if ((piles.at(i).get_type()) == pile_type::foundation){
          piles.at(i).add_card(pile1.remove_card());
        }
      }
    }
  }
  else if (pile1.get_type() == pile_type::foundation){
    if (pile2.get_type() == pile_type::stack){
      //Move a full sequence to a stack
      int l = pile1.last().get_max_value() - pile1.last().get_min_value()+1;
      if (pile2.is_legal_stack(pile1.last()) && 
        pile2.is_legal_move(l, strat, pile1.get_cards())){
        for (int i = 0; i < l; i++){
          pile2.add_card(pile1.remove_card());
        }
      }
    }
    else if (pile2.get_type() == pile_type::foundation){
      //From a foundation to a foundation
      vector<Card> cas = vector<Card>();
      cas = found1.get_cards();
      vector<Card> cards = vector<Card>();
      if (is_legal_multi_move_to_foundation(found1, found2, n)){
        //Add the cards
        cards.erase(cards.begin());
        found1.remove_cards(n, strat);
        found2.add_cards(cards, strat);        
      }
    }     
    else {
        throw std::invalid_argument("Ez a lépés nem szabályos.");        
    } 
  }
}