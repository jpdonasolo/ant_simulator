#pragma once

#include <jsoncpp/json/value.h>

#include "food.h"
#include "ant.h"
#include "anthill.h"

Food * foodFactory(const Json::Value attributes);

Anthill * anthillFactory(const Json::Value attributes, int anthillIndex);