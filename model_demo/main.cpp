#include "core/vertices_layer.hpp" // import this first!
#include "core/mesh_layer.hpp"
#include "utils/preset.hpp"
#include "window/window.hpp"
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <vector>

using namespace Ez3DGL;

#define WIN_W 1200
#define WIN_H 800

Shader shader;

camera_t camera(WIN_W*1.0f/WIN_H, glm::vec3(0., 0., 10));

LightSpot light_spot(glm::vec3(0, 5, 0), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), 30.f, 45.f);
LightDir light_dir(glm::vec3(1, 0, 0));
LightPoint light_point(glm::vec3(-2, 0, 0), glm::vec3(1, 1, 0));

std::unique_ptr<Model> missile_car;
std::unique_ptr<Model> jet;
std::unique_ptr<Model> tie_fighter;
void user_setup(){
    shader.setup_shader(16);
    missile_car = std::make_unique<Model>("./3dmodel/PAC3_Defence_MissileLauncher/PAC3.obj");
    tie_fighter = std::make_unique<Model>("./3dmodel/StarWarsTieFighter/tie_fighter.obj");
    jet = std::make_unique<Model>("./3dmodel/jet/Dassault Rafale C.obj");
}

model_t model_matrixs [] = {model_t(glm::vec3(-4, 0, 0), glm::vec3(0.2f)), model_t(glm::vec3(0, 0, 0), glm::vec3(0.2f)), model_t(glm::vec3(4, 0, 0), glm::vec3(0.05f))};

float rotate_param = 10.f;

void user_loop(long int frame_delta_ms){
    static uint32_t loop_cnt = 0;
    loop_cnt += 1;
    
    camera.calc_projection();
    camera.calc_view();

    const auto color = glm::vec3(loop_cnt%1000/1000.f);
    for(auto& model: model_matrixs){
        model.rotate(frame_delta_ms/rotate_param, glm::vec3(0, 1, 0));
        model.rotate(frame_delta_ms/rotate_param, glm::vec3(1, 0, 0));
    }
    // 
    shader.set_lights({light_dir}, {}, {light_spot});

    missile_car->draw(&shader, &camera, &model_matrixs[0]);
    jet->draw(&shader, &camera, &model_matrixs[1]);
    tie_fighter->draw(&shader, &camera, &model_matrixs[2]);
}

#define IMGUI_SETVEC3(V, MIN, MAX) \
    ImGui::SliderFloat(#V".x", &V.x, MIN, MAX); \
    ImGui::SliderFloat(#V".y", &V.y, MIN, MAX); \
    ImGui::SliderFloat(#V".z", &V.z, MIN, MAX)

void user_imgui(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::SliderFloat("rotate_param", &rotate_param, 1, 100);
}


void user_exit(){
    
}

int main(int argc, const char** argv){
    return window_launch("Ez3DGL - vertices", WIN_W, WIN_H);
}