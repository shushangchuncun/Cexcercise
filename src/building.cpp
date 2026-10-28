//hpp
#include "../include/building.hpp"
#include "../include/team.hpp"
#include <string>
#include <iostream>

Building::Building(std::string name,Team team,int maxHp)
                  :building_name_(name),team_(team),maxHp_(maxHp),alive_(true),hp_(maxHp)
                  {};

int Building::heal(int amount) {
    int last_hp_ = hp_;
    hp_+=amount;
    hp_=std::min(hp_,maxHp_);
    return hp_ - last_hp_;
}

int Building::takeDamage(int amount){
    int last_hp_ = hp_;
    hp_ -= amount;
    hp_ = std::max(hp_, 0);
    if(hp_ == 0)alive_ = false;
    return last_hp_ - hp_;
}

Outpost::Outpost(std::string name , Team team)
                :Building(name,team,300)
                {};

std::string Outpost::typeName() const{return "outpost";}

Base::Base(std::string name , Team team,const Outpost*outpost)
          :Building(name,team,600),
           outpost_(outpost)
          {};

int Base::takeDamage(int amount){
    int last_hp_ = hp_;
    if(outpost_->alive()&&outpost_!=nullptr){
        //std::cout<<"Base invalid"<<std::endl;
        return 0;
    }
    else{
        hp_ -= amount;
        hp_ = std::max(hp_, 0);
        if(hp_ == 0)alive_ = false;
    }
    return last_hp_ - hp_;
}

std::string Base::typeName() const{return "base";}
// TODO
