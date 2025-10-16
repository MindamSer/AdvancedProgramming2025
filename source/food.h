#pragma once

#include "component.h"
class IFood : public Component {

public:
    virtual void on_consume(GameObjectPtr consumer) = 0;
};