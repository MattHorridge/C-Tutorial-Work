#pragma once
#include <SFML/Graphics.hpp>
#include "Zombie.h"
using namespace sf;


int createBackground(VertexArray& rVA, IntRect arena);
//&address
//*dereference

Zombie* createHorde(int numZombies, IntRect arena);