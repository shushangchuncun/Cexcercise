#pragma once
//std
#include <string>
//hpp
#include "team.hpp"

class Building {
 public:
    Building() = delete;
    Building(std::string,Team,int);
    Team team() const {return team_;}
    int hp() const {return hp_;}
    int maxHp() const {return maxHp_;}
    bool alive() const {return alive_;}
    int heal(int);

    virtual int takeDamage(int);
    virtual std::string typeName() const = 0;

    std::string building_name_;
    Team team_;
    int maxHp_,hp_;
    bool alive_;
};

class Outpost : public Building {
 public:
    Outpost(std::string , Team);
    std::string typeName() const override;
};

class Base : public Building {
 public:
    Base(std::string , Team , const Outpost*);
    int takeDamage(int) override;
    std::string typeName() const override;
 private:
    const Outpost* outpost_;
};

// TODO: Building、Outpost、Base
