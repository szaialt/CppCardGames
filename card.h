#pragma once
#include "suit.h"

class Card{
  private:
    suit suit_; //the suit of the card
    int value; //the value of the card
    bool face; //is this up?
    static const int min_value = 1;
    static const int max_value = 13;
  public:
    Card(suit suit_, int value, bool face = false);
    suit get_suit_();
    int get_value();
    bool get_face();
    static int get_min_value();
    static int get_max_value();
    void up(); 
    void down();
};
