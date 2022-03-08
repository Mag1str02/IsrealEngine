#include "point_light.h"

PointLight::PointLight() {
}
PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 clq) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->position = position;
    this->clq = clq;
}
PointLight::~PointLight() {
}