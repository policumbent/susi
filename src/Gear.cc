//
// Created by rikyc on 01/03/2022.
//
#include <Arduino.h>

#include "Gear.h"
#include <EEPROM.h>

Gear::Gear() {
    for(int i=0; i<11; i++){
        gear_up_s1.push_back(EEPROM.read(Gear::position_address+i));
        gear_up_s2.push_back(EEPROM.read(Gear::position_address+i+11));
        gear_down_s1.push_back(EEPROM.read(Gear::position_address+i+22));
        gear_down_s2.push_back(EEPROM.read(Gear::position_address+i+33));
    }
}

void Gear::load(std::vector<int> g_up_s1, std::vector<int> g_up_s2, std::vector<int> g_down_s1,
                std::vector<int> g_down_s2) {

}

void Gear::load(std::string json) {

}

void Gear::shift_down() {

}

void Gear::shift_up() {

}

void Gear::save() {

}
