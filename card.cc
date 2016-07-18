#include "card.h"

using namespace std;

//Card data record
Card::Card(suit suit_, int value, bool face){
  this->suit_ = suit_;
  this->face = face;
  if (value < min_value){
    this->value = min_value;
  }
  else if (value > max_value){
    this->value = max_value;  
  }
  else {
    this->value = value;
  }
}

suit Card::get_suit_(){
  return this->suit_;
}

int Card::get_value(){
  return this->value;
}

bool Card::get_face(){
  return this->face;
}

int Card::get_min_value(){
  return min_value;
}

int Card::get_max_value(){
  return max_value;
}

void Card::up(){
  this->face = true;
}

void Card::down(){
  this->face = false;
}