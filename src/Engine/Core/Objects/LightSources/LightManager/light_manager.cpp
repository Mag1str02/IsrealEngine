#include "light_manager.h"
#include "../../../../ToolBox/Dev/FrameTimeReader/frame_time_reader.h"

#include <iostream>
namespace DE {

void LightManager::ActivatePointLight(GLShaderProgram& shader_program, const PointLight& point_light, int id) {
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.ambient", point_light.ambient);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.diffuse", point_light.diffuse);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.specular", point_light.specular);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].position", point_light.position);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].clq", point_light.clq);
}
void LightManager::ActivateDirectionalLight(GLShaderProgram& shader_program, const DirectionalLight& directional_light, int id) {
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.ambient", directional_light.ambient);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.diffuse", directional_light.diffuse);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.specular", directional_light.specular);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].direction", directional_light.direction);
}
void LightManager::ActivateSpotLight(GLShaderProgram& shader_program, const SpotLight& spot_light, int id) {
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.ambient", spot_light.ambient);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.diffuse", spot_light.diffuse);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.specular", spot_light.specular);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.position", spot_light.position);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.clq", spot_light.clq);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].direction", spot_light.direction);
    shader_program.SetFloat("spot_lights[" + std::to_string(id) + "].outer_cone_cos", spot_light.outer_cone_cos);
    shader_program.SetFloat("spot_lights[" + std::to_string(id) + "].inner_cone_cos", spot_light.inner_cone_cos);
}

LightManager::LightManager() {
}
void LightManager::Update(double dt) {
    DE_FTR_ENTER("Light Passing System");
    auto& shader_programs = GetComponentArray<UniqueShader>();
    auto& point_lights = GetComponentArray<PointLight>();
    auto& directional_lights = GetComponentArray<DirectionalLight>();
    auto& spot_lights = GetComponentArray<SpotLight>();
    for (auto [entity_id, unique_shader] : shader_programs) {
        auto& shader_program = unique_shader.shader_program;
        int cnt_dir_light = 0;
        int cnt_point_light = 0;
        int cnt_spot_light = 0;
        for (auto [entity_id, directional_light] : directional_lights) {
            ActivateDirectionalLight(shader_program, directional_light, cnt_dir_light++);
        }
        for (auto [entity_id, point_light] : point_lights) {
            ActivatePointLight(shader_program, point_light, cnt_point_light++);
        }
        for (auto [entity_id, spot_light] : spot_lights) {
            ActivateSpotLight(shader_program, spot_light, cnt_spot_light++);
        }
        shader_program.SetVec3i("light_amount", cnt_dir_light, cnt_point_light, cnt_spot_light);
    }
    DE_FTR_LEAVE();
}
}  // namespace DE