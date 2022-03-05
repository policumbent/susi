//
// Created by rikyc on 01/03/2022.
//
#include <Arduino.h>

#ifndef TEST_GEAR_H
#define TEST_GEAR_H

#include <stdlib.h>
#include <vector>
#include <string>

class Gear {
private:

    // vettori con le posizioni di ogni servo per ogni marcia
    std::vector<int> gear_up_s1{};
    std::vector<int> gear_up_s2{};
    std::vector<int> gear_down_s1{};
    std::vector<int> gear_down_s2{};

    // marcia inserita
    unsigned cur_gear{1};

    // indirizzi della eeprom a cui sono salvate le posizioni delle marce
    const int position_address{0};
    const int cur_gear_address{0};

public:
    // Il costruttore carica le posizioni dalla EEPROM,
    // carica l'ultima posizione del cambio,
    // inizializza il collegamento con i servo
    // e li posiziona sull'ultima marcia inserita
    Gear();

    // Cambia la marcia spostando i servo verso la marcia più grande
    void shift_up();

    // Cambia la marcia spostando i servo verso la marcia più piccola
    void shift_down();

    // Aggiorna le posizioni salvate ricevendo un json come parametro
    void load(std::string json);

    // Aggiorna le posizioni salvate ricevendo 4 vettori con le posizioni di ogni servo
    void load(
            std::vector<int> gear_up_s1,
            std::vector<int> gear_up_s2,
            std::vector<int> gear_down_s1,
            std::vector<int> gear_down_s2
            );

    // salva i valori delle posizioni caricati in ram nella eeprom
    void save();
};


#endif //TEST_GEAR_H
