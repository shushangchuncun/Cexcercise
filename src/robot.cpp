//std
#include <cstddef>
#include<iostream>
#include<sstream>
//hpp
#include "../include/robot.hpp"

Robot::Robot(std::string name, Team team, int maxHp, int ammo)
    : robot_name_(name), team_(team), maxHp_(maxHp), ammo_(ammo) {
        hp_ = maxHp_;
        alive_ = true;
        heat_ = 0;
    }

const std::string& Robot::name() const {return robot_name_;}

Team Robot::team() const {return team_;}

int Robot::hp() const {return hp_;}

int Robot::maxHp() const {return maxHp_;}

bool Robot::alive() const {return alive_;}

int Robot::ammo() const {return ammo_;}

int Robot::heat() const {return heat_;}

int Robot::heal(int amount) {
    int last_hp_ = hp_;
    hp_+=amount;
    hp_=std::min(hp_,maxHp_);
    return hp_ - last_hp_;
}

int Robot::takeDamage(int amount){
    int last_hp_ = hp_;
    hp_ -= amount;
    hp_ = std::max(hp_,0);
    if(hp_ == 0)alive_ = false;
    return last_hp_ - hp_;
}

void Robot::cool(int seconds){
    heat_-=seconds*coolingPersec_;
    heat_ = std::max(heat_,0);
}

void Robot::fire(){
    if(heat_>=heatLimit_){
        std::cerr<<"HeatLimit!Ban shooting!"<<std::endl;
        return;
    }
    ammo_--;
    heat_ += heatPerShot_;
    heat_ = std::min(heat_,heatLimit_);
}


Hero::Hero(std::string name, Team team)
          :Robot(name,team,300,10)
          {
    damagePerShot_ = HERO_STANDARD_DAMAGEPERSHOT;
    heatPerShot_ = HERO_STANDARD_HEATPERSHOT;
    heatLimit_ = HERO_STANDARD_HEATLIMIT;
    coolingPersec_ = HERO_STANDARD_COOLPERSHOT;
    hero_mode = HeroMode::standard;
    change_mode_time = 0;
    is_switching_ = false;
}

//mode
Hero::HeroMode Hero::StringToMode(const std::string&mode){
    if(mode == "standard")
        return HeroMode::standard;
    else if(mode == "snipe")
        return HeroMode::snipe;
    else if(mode == "cooling")
        return HeroMode::cooling;
    else
        return HeroMode::unknown;
}

std::string Hero::ModeToString(Hero::HeroMode mode) const{
    if(mode == Hero::HeroMode::standard)
        return "standard";
    else if(mode == Hero::HeroMode::snipe)
        return "snipe";
    else if(mode == Hero::HeroMode::cooling)
        return "cooling";
    else
        return "unknown";
}

Hero::Result Hero::requestMode(const std::string&mode){
    Hero::HeroMode h_mode = StringToMode(mode);
    if(h_mode == HeroMode::unknown)
        return Hero::Result::Unknown;
    else if(switching()){
        return Hero::Result::Busy;
    }
    else if(h_mode == hero_mode)
        return Hero::Result::Same;
    is_switching_ = true;
    request_mode_ = h_mode;
    return Hero::Result::Started;
}

bool Hero::switching() const{return is_switching_;}

std::string Hero::describeMode() const{
    if(!is_switching_)
        return ModeToString(hero_mode);
    std::ostringstream oss;
    oss << "switching -> "<<ModeToString(request_mode_)
        <<" ("<<(3-change_mode_time)<<" s"<<" left)";
    return oss.str();
}

std::string Hero::advanceMode(){
    if(!is_switching_)return "";
    change_mode_time++;
    if(change_mode_time == 3){
        change_mode_time = 0;
        hero_mode = request_mode_;
        is_switching_ = false;
        if(hero_mode == Hero::HeroMode::standard){
            damagePerShot_ = HERO_STANDARD_DAMAGEPERSHOT;
            heatPerShot_ = HERO_STANDARD_HEATPERSHOT;
            heatLimit_ = HERO_STANDARD_HEATLIMIT;
            coolingPersec_ = HERO_STANDARD_COOLPERSHOT;
        }
        else if(hero_mode == Hero::HeroMode::snipe){
            damagePerShot_ = HERO_SNIPE_DAMAGEPERSHOT;
            heatPerShot_ = HERO_SNIPE_HEATPERSHOT;
            heatLimit_ = HERO_SNIPE_HEATLIMIT;
            coolingPersec_ = HERO_SNIPE_COOLPERSHOT;
        }
        else if(hero_mode == Hero::HeroMode::cooling){
            damagePerShot_ = HERO_COOLING_DAMAGEPERSHOT;
            heatPerShot_ = HERO_COOLING_HEATPERSHOT;
            heatLimit_ = HERO_COOLING_HEATLIMIT;
            coolingPersec_ = HERO_COOLING_COOLPERSHOT;
        }
        //heat_ = std::min(heat_,heatLimit_);
        return ModeToString(hero_mode);
    }
    return "";
}

std::string Hero::typeName() const {return "hero";}

int Hero::damagePerShot() const {return damagePerShot_;}

int Hero::heatPerShot() const {return heatPerShot_;}

int Hero::heatLimit() const {return heatLimit_;}

int Hero::coolingPersec() const {return coolingPersec_;}

bool Hero::advanceRespawn(int delay){
    if(alive_){
        deadFor_ = 0;
        return false;
    }
    deadFor_ ++;
    if(deadFor_<delay)return false;
    respawn();
    deadFor_ = 0;
    return true;
}

void Hero::respawn(){
    alive_ = true;
    hp_ = maxHp_;heat_ =0;ammo_ = 10;
    damagePerShot_ = HERO_STANDARD_DAMAGEPERSHOT;
    heatPerShot_ = HERO_STANDARD_HEATPERSHOT;
    heatLimit_ = HERO_STANDARD_HEATLIMIT;
    coolingPersec_ = HERO_STANDARD_COOLPERSHOT;
    hero_mode = HeroMode::standard;
    change_mode_time = 0;
    is_switching_ = false;
}

Infantry::Infantry(std::string name, Team team)
    : Robot(name, team, 200 , 100)
    {
    damagePerShot_ = INFANTRY_STANDARD_DAMAGEPERSHOT;
    heatPerShot_ = INFANTRY_STANDARD_HEATPERSHOT;
    heatLimit_ = INFANTRY_STANDARD_HEATLIMIT;
    coolingPersec_ = INFANTRY_STANDARD_COOLPERSHOT;
    infantry_mode = InfantryMode::standard;
    change_mode_time = 0;
    is_switching_ = false;
}

//mode
Infantry::InfantryMode Infantry::StringToMode(const std::string&mode){
    if(mode == "standard")
        return InfantryMode::standard;
    else if(mode == "burst")
        return InfantryMode::burst;
    else if(mode == "cooling")
        return InfantryMode::cooling;
    else
        return InfantryMode::unknown;
}

std::string Infantry::ModeToString(const Infantry::InfantryMode mode) const{
    if(mode == Infantry::InfantryMode::standard)
        return "standard";
    else if(mode == Infantry::InfantryMode::burst)
        return "burst";
    else if(mode == Infantry::InfantryMode::cooling)
        return "cooling";
    else
        return "unknown";
}

Infantry::Result Infantry::requestMode(const std::string&mode){
    Infantry::InfantryMode i_mode = StringToMode(mode);
    if(i_mode == InfantryMode::unknown)
        return Infantry::Result::Unknown;
    else if(switching()){
        return Infantry::Result::Busy;
    }
    else if(i_mode == infantry_mode)
        return Infantry::Result::Same;
    is_switching_ = true;
    request_mode_ = i_mode;
    return Infantry::Result::Started;
}

bool Infantry::switching() const{return is_switching_;}

std::string Infantry::describeMode() const{
    if(!is_switching_)
        return ModeToString(infantry_mode);
    std::ostringstream oss;
    oss << "switching -> "<<ModeToString(request_mode_)
        <<" ("<<(3-change_mode_time)<<" s"<<" left)";
    return oss.str();
}

std::string Infantry::advanceMode(){
    if(!is_switching_)return "";
    change_mode_time++;
    if(change_mode_time == 3){
        change_mode_time = 0;
        infantry_mode = request_mode_;
        is_switching_ =false;
        if(infantry_mode == Infantry::InfantryMode::standard){
            damagePerShot_ = INFANTRY_STANDARD_DAMAGEPERSHOT;
            heatPerShot_ = INFANTRY_STANDARD_HEATPERSHOT;
            heatLimit_ = INFANTRY_STANDARD_HEATLIMIT;
            coolingPersec_ = INFANTRY_STANDARD_COOLPERSHOT;
        }
        else if(infantry_mode == Infantry::InfantryMode::burst){
            damagePerShot_ = INFANTRY_BURST_DAMAGEPERSHOT;
            heatPerShot_ = INFANTRY_BURST_HEATPERSHOT;
            heatLimit_ = INFANTRY_BURST_HEATLIMIT;
            coolingPersec_ = INFANTRY_BURST_COOLPERSHOT;
        }
        else if(infantry_mode == Infantry::InfantryMode::cooling){
            damagePerShot_ = INFANTRY_COOLING_DAMAGEPERSHOT;
            heatPerShot_ = INFANTRY_COOLING_HEATPERSHOT;
            heatLimit_ = INFANTRY_COOLING_HEATLIMIT;
            coolingPersec_ = INFANTRY_COOLING_COOLPERSHOT;
        }
        //heat_ = std::min(heat_,heatLimit_);
        return ModeToString(infantry_mode);
    }
    return "";
}

std::string Infantry::typeName() const {return "infantry";}

int Infantry::damagePerShot() const {return damagePerShot_;}

int Infantry::heatPerShot() const {return heatPerShot_;}

int Infantry::heatLimit() const {return heatLimit_;}

int Infantry::coolingPersec() const {return coolingPersec_;}

bool Infantry::advanceRespawn(int delay){
    if(alive_){
        deadFor_ = 0;
        return false;
    }
    deadFor_ ++;
    if(deadFor_<delay)return false;
    respawn();
    deadFor_ = 0;
    return true;
}

void Infantry::respawn(){
    alive_ = true;
    hp_ = maxHp_;heat_ =0;ammo_ = 100;
    damagePerShot_ = INFANTRY_STANDARD_DAMAGEPERSHOT;
    heatPerShot_ = INFANTRY_STANDARD_HEATPERSHOT;
    heatLimit_ = INFANTRY_STANDARD_HEATLIMIT;
    coolingPersec_ = INFANTRY_STANDARD_COOLPERSHOT;
    infantry_mode = InfantryMode::standard;
    change_mode_time = 0;
    is_switching_ = false;
}

Engineer::Engineer(std::string name,Team team)
                  :Robot(name,team,250,0),cool_time(0){}

bool Engineer::ready()const{
    if(cool_time == 0)return true;
    else return false;
}

int Engineer::repair(Robot &target){
    cool_time = 5;
    return target.heal(50);
}

int Engineer::repair(Building& target){
    cool_time = 5;
    return target.heal(50);
}

void Engineer::advanceRepair(){
    cool_time--;
    cool_time = std::max(0,cool_time);
}

std::string Engineer::typeName() const{return "engineer";}
int Engineer::damagePerShot()const {return 0;}
int Engineer::heatPerShot() const{return 0;}
int Engineer::heatLimit() const{return 0;}
int Engineer::coolingPersec() const{return 0;}