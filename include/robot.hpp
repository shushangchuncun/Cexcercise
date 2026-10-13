#pragma once

//std
#include<string>

//hpp
#include "team.hpp"
#include "extensions.hpp"

class Robot{
 public:
    Robot() = delete;
    Robot(std::string ,Team , int, int);
    const std::string &name() const;
    Team team() const;
    int hp() const;
    int maxHp() const;
    bool alive() const;
    int ammo() const;
    int heat() const;
    int heal(int);
    int takeDamage(int);
    void fire();
    void cool(int);

    virtual std::string typeName() const = 0;
    virtual int damagePerShot() const = 0;
    virtual int heatPerShot() const = 0;
    virtual int heatLimit() const = 0;
    virtual int coolingPersec() const = 0;
 protected:
    std::string robot_name_;
    Team team_;
    int maxHp_,hp_;
    int ammo_;
    bool alive_;
    int heat_;
    bool fire_;
    int damagePerShot_ , heatPerShot_, 
        heatLimit_, coolingPersec_;
    int deadFor_;
 private:
};

class Hero : public Robot,public ModeControl,public Respawnable {
 public:
    //mode
    enum class HeroMode {standard ,snipe ,cooling ,unknown};

    HeroMode StringToMode(const std::string& );
    std::string ModeToString(Hero::HeroMode) const;
    Result requestMode(const std::string& ) override;
    bool switching() const override;
    std::string describeMode() const override;
    std::string advanceMode() override;

    Hero() = delete;
    Hero(std::string, Team);
    std::string typeName() const override;
    int damagePerShot() const override;
    int heatPerShot() const override;
    int heatLimit() const override;
    int coolingPersec() const override;

    bool advanceRespawn(int) override;
    void respawn();
 private:
    HeroMode hero_mode,request_mode_;
    //mode changing time
    int change_mode_time;
    bool is_switching_;
    //yaml
    const static constexpr int HERO_STANDARD_DAMAGEPERSHOT = 100 ,
                               HERO_SNIPE_DAMAGEPERSHOT = 150,
                               HERO_COOLING_DAMAGEPERSHOT = 100;
    const static constexpr int HERO_STANDARD_HEATPERSHOT =100,
                               HERO_SNIPE_HEATPERSHOT = 150,
                               HERO_COOLING_HEATPERSHOT = 100;
    const static constexpr int HERO_STANDARD_HEATLIMIT =200,
                               HERO_SNIPE_HEATLIMIT = 300,
                               HERO_COOLING_HEATLIMIT = 200;
    const static constexpr int HERO_STANDARD_COOLPERSHOT =20,
                               HERO_SNIPE_COOLPERSHOT = 20,
                               HERO_COOLING_COOLPERSHOT = 40;
};

class Infantry : public Robot,public ModeControl,public Respawnable {
 public:
    //mode
    enum class InfantryMode {standard ,burst ,cooling ,unknown};

    InfantryMode StringToMode(const std::string& );
    std::string ModeToString(const Infantry::InfantryMode) const;
    Result requestMode(const std::string& ) override;
    bool switching() const override;
    std::string describeMode() const override;
    std::string advanceMode() override;

    Infantry() = delete;
    Infantry(std::string, Team);
    std::string typeName() const override;
    int damagePerShot() const override;
    int heatPerShot() const override;
    int heatLimit() const override;
    int coolingPersec() const override;

    bool advanceRespawn(int) override;
    void respawn();
 private:
    InfantryMode infantry_mode,request_mode_;
    //mode changing time
    int change_mode_time;
    bool is_switching_;
    //yaml
    const static constexpr int INFANTRY_STANDARD_DAMAGEPERSHOT = 10,
                               INFANTRY_BURST_DAMAGEPERSHOT = 15,
                               INFANTRY_COOLING_DAMAGEPERSHOT = 10;
    const static constexpr int INFANTRY_STANDARD_HEATPERSHOT =10,
                               INFANTRY_BURST_HEATPERSHOT = 15,
                               INFANTRY_COOLING_HEATPERSHOT = 10;
    const static constexpr int INFANTRY_STANDARD_HEATLIMIT =100,
                               INFANTRY_BURST_HEATLIMIT = 150,
                               INFANTRY_COOLING_HEATLIMIT = 100;
    const static constexpr int INFANTRY_STANDARD_COOLPERSHOT =10,
                               INFANTRY_BURST_COOLPERSHOT = 10,
                               INFANTRY_COOLING_COOLPERSHOT = 20;
};

class Engineer:public Robot,public Repairer{
 public:
    Engineer() = delete;
    Engineer(std::string, Team);
    bool ready() const override;
    int repair(Robot &) override;
    int repair(Building &) override;
    void advanceRepair() override;

    std::string typeName() const override;
    int damagePerShot() const override;
    int heatPerShot() const override;
    int heatLimit() const override;
    int coolingPersec() const override;
 private:
    int cool_time;
};
// TODO: Robot、Hero、Infantry
