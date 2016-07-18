#include "pile.h"
#include <stack>
#include <stdexcept>      // std::invalid_argument

//Lehet, hogy át kell írni mindent klónozásra
//Az egy újabb változat lesz

//new pile with type and cards
Pile::Pile(pile_type type, std::vector<Card> cards){
  this->type = type;
  this->cards = cards;
}

//is the suit black?
bool Pile::black(suit s){
    return ((s == suit::spades) || (s == suit::clubs));
  }

//what is the type
pile_type Pile::get_type(){
  return this->type;
}

//the cards
std::vector<Card> Pile::get_cards(){
  return this->cards;
}
    
    
void Pile::set_cards(std::vector<Card> card){
  this->cards = cards;
}
//Miért nem hajlandó ez felismerni a típusokat????  
void Pile::add_card(Card card){
  if (type == pile_type::stack){
    if (is_legal_stack(card)){
        this->cards.push_back(card);              
      }
      else {
        throw std::invalid_argument("A gyűjtőbe csak ugyanaz a szín és a következő lapérték rakható.");
      }      
    }
  if (type == pile_type::talon){
    card.down();
    this->cards.push_back(card);
  
  }
  if (type == pile_type::waste){
    card.up();
    this->cards.push_back(card);    
  }
  if (type == pile_type::freecell){
    if (cards.empty()) {
      this->cards.push_back(card);      
    }
      else {
        throw std::invalid_argument("Ide csak egy lap tehető.");
      }
  }
  if (type == pile_type::foundation){
    this->cards.push_back(card);
  }
}
    
void  Pile::add_cards(std::vector<Card> cards2, game_strategy strat){
  if ((type == pile_type::foundation) 
    && (is_legal_move(cards2.size(), strat, cards2))){
       for (int i = 0; i < cards2.size(); i++) {
         cards.push_back(cards2.at(i));
       }
    }
  else {
    throw std::invalid_argument("Nem szabályos lépés");
  }  
}   
   
Card Pile::remove_card(){
  Card card = cards.at(cards.size()-1);
  cards.pop_back();
  return card;
}
    
//remove the last n cards if it is legal according to the strategy
std::vector<Card> Pile::remove_cards(int n, game_strategy strat){
  std::vector<Card> ca = std::vector<Card>();
  if ((type == pile_type::foundation) 
       && (is_legal_move(n, strat))){
    std::stack<Card> sta = std::stack<Card>();
    for (int i = 1; i <= n; i++) {
      Card card = cards.at(cards.size()-1);
      cards.pop_back();
      sta.push(card);
    }
    for (int i = 1; i <= n; i++) {
      ca.push_back(sta.top());
    }
  }
  else {
    throw std::invalid_argument("Nem szabályos lépés");
  }   
  return ca;
}

//is this move legal according to strategy?
bool Pile::is_legal_move(int n, game_strategy strat, 
                    std::vector<Card> cards2){
  if (type != pile_type::foundation) {
    return false;
  }
  Card card = cards2.at(cards2.size()-1);
  int val = (cards2.at(cards2.size()-1)).get_value();
  for (int i = 1; i <= n; i++){
    if (cards2.at(cards2.size()-n).get_value() != val + n - 1){
      return false;
    }
  }
  suit su = (cards2.at(cards2.size()-1)).get_suit_();
  if (strat == game_strategy::alternate){
    int col = 1;
    if (black(su)){
      col = 0;
    }

    for (int i = 1; i <= n; i++){
      int col2 = 1;
      if (black(cards2.at(cards2.size()-n).get_suit_())){
        col = 0;
      }
      if (col2 != (n + col + 1) %2){
        return false;
      }
    }
  return true;
  }
  if (strat == game_strategy::constant){
  for (int i = 1; i <= n; i++){
    if ((cards2.at(cards2.size()-n)).get_suit_() != card.get_suit_()){
      return false;
    }
    }
  return true;
  }
  return false;
}

//is this move legal according to strategy?
bool Pile::is_legal_move(int n, game_strategy strat){
  return is_legal_move(n, strat, cards);
}

//if this is a stack, is legal to add the card? 
bool  Pile::is_legal_stack(Card card){
    if (cards.empty()) {
      //Az üres stack elfogadja az ászokat
      if (card.get_value() == 1){
        return true;     
      }
      else {
        return false;
      }
    }
    else {
      Card card2 = cards.at(cards.size()-1);
      suit suit1 = card.get_suit_();
      int val1 = card.get_value();
      suit suit2 = card2.get_suit_();
      int val2 = card2.get_value();
      if ((suit1 == suit2) && (val1 == val2+1)){
        return true;          
      }
      return false;
    }
}    

bool Pile::empty(){
  return cards.empty();
}

Card Pile::last(){
  return cards.at(cards.size()-1);
}

//the last n cards
std::vector<Card> Pile::last(int n){
  std::vector<Card> cas;
  for (int i = cards.size()-n-1; i < cards.size(); i++){
    cas.push_back(cards.at(i));
  }
  return cas;
}
