#pragma once

//Good practice, include evrithing we nedd put in one file 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "Vec2.h"

//simplify, use prt<T> insted of allways having to type std::shared_ptr<T>
template <class T> using ptr = std::shared_ptr<T>;