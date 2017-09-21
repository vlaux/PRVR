/*
 * Utilis.cpp
 *
 *  Created on: 9 de jan de 2017
 *      Author: leo
 */

#include "utils.h"

utils::random::mt19937_rng_singleton* utils::random::mt19937_rng_singleton::_instance = nullptr;

utils::random::mt19937_rng_singleton::mt19937_rng_singleton(){

}

utils::random::mt19937_rng_singleton* utils::random::mt19937_rng_singleton::getInstance() {
    if (_instance == nullptr) {
        _instance = new utils::random::mt19937_rng_singleton();
    }
    return _instance;
}


