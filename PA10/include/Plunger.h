#ifndef PLUNGER_H_
#define PLUNGER_H_

#include "physics_object.h"


class Plunger : public Object
{
public:

    Plunger();
    Plunger(std::string objFilename, std::string texFilename);
    ~Plunger();

    //how far out it is, scale of 0-1
    float displacement;
    
    glm::vec3 plunger_resting_position;
    float plunger_min_distance = 0.0;
    float plunger_max_distance;
    float depress_speed = .2;

    bool triggeringPlunger = true;

    void Update(unsigned int dt);
};

#endif
