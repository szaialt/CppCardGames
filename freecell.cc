#include "freecell.h"
#include <algorithm>
#include <stdexcept>      // std::invalid_argument

using namespace std;

Freecell::Freecell(){
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
  //freecells and stacks are empty at first
  
  //The numbers of stacks, freecells and foundations, resp. 
  int stack_num = 4;
  int freecell_num = 4;
  int foundation_num = 8;
  
  //Define the stacks
  std::vector<Pile> stacks;
  for (int i = 0; i < stack_num; i++){
    std::vector<Card> stack_cards = std::vector<Card>();
    Pile stack = Pile(pile_type::stack, stack_cards);
    stacks.push_back(stack);
   }
  
  //define the freecells
    std::vector<Pile> freecells;
  for (int i = 0; i < freecell_num; i++){
    std::vector<Card> freecell_cards = std::vector<Card>();
    Pile freecell = Pile(pile_type::freecell, freecell_cards);
    freecells.push_back(freecell);
   }
  
  //Define the foundations
  std::vector<Pile> foundations;
   int r = deck.size() % foundation_num;
   int d = deck.size() / foundation_num;
  for (int i = 0; i < foundation_num; i++){
    std::vector<Card> foundation_cards = std::vector<Card>();
    //Add the cards to the foundations
    for (int k = 0; k < d; k++){
      deck.at(i*d+k).up();
      foundation_cards.push_back(deck.at(i*d+k));
    }
    if (i < r) {
      deck.at(deck.size() - i - 1).up();
      foundation_cards.push_back(deck.at(deck.size() - i - 1));
    }
    Pile foundation = Pile(pile_type::foundation, foundation_cards);
    foundations.push_back(foundation);
  }
  
  //Collect the piles
  this->piles = std::vector<Pile>();
   for (int i = 0; i < freecell_num; i++){
    piles.push_back(freecells.at(i));
  }
   for (int i = 0; i < stack_num; i++){
    piles.push_back(stacks.at(i));
  }
  for (int i = 0; i < foundation_num; i++){
    piles.push_back(foundations.at(i));
  }

}

//Removes the top card of pile1 and adds it to the pile2, if it is legal
void Freecell::add_card_from(Pile pile1, Pile pile2){
  if (pile2.get_type() == pile_type::foundation){
    //Add card to a foundation
    if (pile2.empty()){
      //You can add any card to an empty foundation
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
    if (pile1.get_type() == pile_type::freecell){
      if (pile2.get_type() == pile_type::stack){
        //From a freecell to a stack
        Card card = pile1.last();
        if (pile2.is_legal_stack(card)){
          pile2.add_card(pile1.remove_card());
        }
      }
    }
      else {
        throw std::invalid_argument("Ez a lépés nem szabályos.");
      }
    }
  }
  else if (pile2.get_type() == pile_type::stack){
      //From a freecell to a stack
      Card card = pile1.last();
      if (pile2.is_legal_stack(card)){
        pile2.add_card(pile1.remove_card());
      }
    }
  else if (pile1.get_type() == pile_type::foundation){
    if (pile2.get_type() == pile_type::freecell){
      //From a foundation to a freecell
      Card card = pile1.last();
      //You can add a card only to an empty freecell
      if (pile2.empty()){
        pile1.remove_card();
        pile2.add_card(card);
      }
    }
  }
  else {
    throw std::invalid_argument("Ez a lépés nem szabályos.");
  }
}

//Removes the top k cards of pile1 and adds it to the pile2, if it is legal
void Freecell::add_cards_from(Pile found1, Pile found2, int k){
  //You cann't take too many cards
  int n = 1;
  if (k < empty_freecells() + 1)
    n = k;
  else
    n = empty_freecells() + 1;
  if (n == 1) {
    add_card_from(found1, found2);
  }
  else {
    if (found1.get_type() == pile_type::foundation){
      if (found2.get_type() == pile_type::foundation){  
	//From a foundation to a foundation
        if (is_legal_multi_move_to_foundation(found1, found2, n)){
	  //Add the cards
          vector<Card> cards = vector<Card>();
	  for (int i = found1.get_cards().size()-1-n; i<found1.get_cards().size(); i++){
            cards.push_back(found1.get_cards().at(i));
          }
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

//How much empty freecells we have
int Freecell::empty_freecells(){
  int free = 0;
  for (int i = 0; i < 4; i++){
    if (piles.at(i).empty()){
      free++;
    }
  }
  return free;
}