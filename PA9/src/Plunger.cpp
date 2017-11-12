#include "Plunger.h"

Plunger::Plunger()
{
    displacement = 0.0;
    plunger_max_distance = .04;
}

Plunger::Plunger(std::string objFilename, std::string texFilename)
: Object(objFilename, texFilename)
{
    displacement = 0.0;
    plunger_max_distance = .04;
}

Plunger::~Plunger()
{

}

void Plunger::Update(unsigned int dt)
{
    glm::vec3 translation_vector;

    if (!triggeringPlunger)
    {
        if (displacement > 0)
        {
            displacement -= depress_speed;
        }

        if (displacement < 0)
        {
            displacement = 0;
        }
    }

    translation_vector = glm::vec3(0, 0, displacement * plunger_max_distance);
    
    glm::mat4 translationMat = glm::translate(glm::mat4(), translation_vector);

    model = translationMat;
}
