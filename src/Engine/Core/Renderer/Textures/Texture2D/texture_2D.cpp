#include "texture_2D.h"
#include <iostream>

namespace DE {

//*----------------------------------------------------------------------------------------------------

void Texture2D::SetData(const Texture2DData& texture_data) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture_data.format, texture_data.width, texture_data.height, 0, texture_data.format, GL_UNSIGNED_BYTE, *(texture_data.data));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::BindToUnit(TextureUnitId unit_id) const {
    glActiveTexture(GL_TEXTURE0 + unit_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}

//*--------------------------------------------------

Texture2DId Texture2D::Texture2DManager::ICreateTexture2D() {
    ::GLuint texture_id;
    glGenTextures(1, &texture_id);
    _reference_count[texture_id] = 1;
    return texture_id;
}
Texture2DId Texture2D::Texture2DManager::ICreateInstance(Texture2DId texture_id) {
    ++_reference_count[texture_id];
    return texture_id;
}
void Texture2D::Texture2DManager::IDestroyInstance(Texture2DId texture_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[texture_id];
    if (_reference_count[texture_id] == 0) {
        IDestroyTexture2D(texture_id);
    }
}
void Texture2D::Texture2DManager::IDestroyTexture2D(Texture2DId texture_id) {
    glDeleteTextures(1, &texture_id);
    _reference_count.erase(texture_id);
}

//*----------------------------------------------------------------------------------------------------

Texture2D::Texture2D() {
    _texture_id = Texture2DManager::CreateTexture2D();
}
Texture2D::Texture2D(const Texture2D& other) {
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
}
Texture2D::Texture2D(Texture2D&& other) {
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
}
Texture2D& Texture2D::operator=(const Texture2D& other) {
    if (&other == this) {
        return *this;
    }
    Texture2DManager::DestroyInstance(_texture_id);
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
    return *this;
}
Texture2D& Texture2D::operator=(Texture2D&& other) {
    if (&other == this) {
        return *this;
    }
    Texture2DManager::DestroyInstance(_texture_id);
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
    return *this;
}
Texture2D::~Texture2D() {
    Texture2DManager::DestroyInstance(_texture_id);
}

//*--------------------------------------------------

Texture2D::Texture2DManager::Texture2DManager() {
    _initialized = false;
}
Texture2D::Texture2DManager& Texture2D::Texture2DManager::Get() {
    static Texture2DManager texture_2d_manager;
    return texture_2d_manager;
}

Texture2DId Texture2D::Texture2DManager::CreateTexture2D() {
    return Texture2DManager::Get().ICreateTexture2D();
}
Texture2DId Texture2D::Texture2DManager::CreateInstance(Texture2DId texture_id) {
    return Texture2DManager::Get().ICreateInstance(texture_id);
}

void Texture2D::Texture2DManager::DestroyInstance(Texture2DId texture_id) {
    Texture2DManager::Get().IDestroyInstance(texture_id);
}
void Texture2D::Texture2DManager::DestroyTexture2D(Texture2DId texture_id) {
    Texture2DManager::Get().IDestroyTexture2D(texture_id);
}

}  // namespace DE
