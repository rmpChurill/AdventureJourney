#include "GlScreens.hpp"
#include "data/Random.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "data/Math.hpp"

gui::DummyObject::DummyObject(glutil::SceneObject *sceneObject) : sceneObject(sceneObject),
                                                                  axis(0.f),
                                                                  rotation(0.f),
                                                                  basePos(0.f),
                                                                  pos(0.f),
                                                                  max(0.f),
                                                                  speed(0.f),
                                                                  scale(0.f)
{
    util::Random rnd;

    this->basePos = glm::vec3(rnd.Next(0.f, 5.f),
                              rnd.Next(0.f, 5.f),
                              rnd.Next(0.f, 5.f));

    this->scale = rnd.Next(0.1f, 0.5f);

    this->axis = glm::vec3(rnd.Next(0.0f, 1.0f),
                           rnd.Next(0.0f, 1.0f),
                           rnd.Next(0.0f, 1.0f));

    this->max = rnd.Next(0.f, 4.f);
    this->pos = rnd.Next(0.f, this->max);

    this->speed = rnd.Next(0.5f, 1.5f);
    this->rotation = rnd.Next(0.f, 360.f);

    this->sceneObject->SetModelMatrix(glm::translate(this->basePos) *
                                      glm::rotate(glm::radians(this->rotation), this->axis) *
                                      glm::scale(glm::vec3(this->scale, this->scale, this->scale)));
}

void gui::DummyObject::Upate(double delta)
{
    this->rotation += static_cast<float>(glm::degrees(delta));
    this->pos += static_cast<float>(delta * this->speed);

    if (this->pos < 0 || this->pos > this->max)
        this->speed *= -1;

    this->pos = util::Crop(this->pos, 0.f, this->max);

    this->sceneObject->SetModelMatrix(glm::translate(this->basePos + glm::vec3(0, this->pos, 0)) *
                                      glm::rotate(glm::radians(this->rotation), this->axis) *
                                      glm::scale(glm::vec3(this->scale, this->scale, this->scale)));
}