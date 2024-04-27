#include "core/vertices_layer.hpp" // import this first!
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

std::unique_ptr<vertices_t> vaos[4];
const auto vert_data_layout = {3, 3, 2};

const auto vertices_ball = Ez3DGL::preset::vgen_ball::generate(20);
const auto vertices_cone_45 = Ez3DGL::preset::vgen_cone::generate(20, 45);
std::unique_ptr<shader_t> shader_simple;

camera_t camera(WIN_W*1.0f/WIN_H, glm::vec3(0., 0., 10));

std::vector<float> gen_vertices_dynamic(int t){
    std::vector<glm::vec2> outlines;
    for(float i=0;i<1;i+=0.01){
        outlines.push_back(glm::vec2(std::fabs(std::sin(i*2*M_PI+t/10.f))*0.5, i-0.5));
    }
    return Ez3DGL::preset::vgen_revolu_surf::generate(20, outlines, glm::vec3(0, 1, 0));
}

void user_setup(){
    vaos[0] = std::make_unique<vertices_t>(36*(3+3+2), vert_data_layout, preset::cube_vertices_with_normals_texture, 0, nullptr, GL_STATIC_DRAW);
    vaos[1] = std::make_unique<vertices_t>(vertices_ball.size(), vert_data_layout, &vertices_ball[0], 0, nullptr, GL_STATIC_DRAW);
    vaos[2] = std::make_unique<vertices_t>(vertices_cone_45.size(), vert_data_layout, &vertices_cone_45[0], 0, nullptr, GL_STATIC_DRAW);
    const auto vertices_dynamic = gen_vertices_dynamic(0);
    vaos[3] = std::make_unique<vertices_t>(vertices_dynamic.size(), vert_data_layout, &vertices_dynamic[0], 0, nullptr, GL_STATIC_DRAW);

    shader_simple = std::make_unique<shader_t>("./shader/cube.vs", "./shader/cube.fs", "view", "projection", "model");
}

model_t model_matrixs [] = {model_t(glm::vec3(-2, 0, 0)), model_t(glm::vec3(0, 0, 0)), model_t(glm::vec3(2, 0, 0)), model_t(glm::vec3(4, 0, 0))};

const glm::vec3 colors [] = {glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(1, 1, 0)};

float rotate_param = 10.f;

void user_loop(long int frame_delta_ms){
    static uint32_t loop_cnt = 0;
    loop_cnt += 1;

    camera.calc_projection();
    camera.calc_view();
    
    const auto vertices_dynamic = gen_vertices_dynamic(loop_cnt);
    vaos[3]->update_vbo_buffer(vertices_dynamic.size(), &vertices_dynamic[0]);
    
    for(auto& model: model_matrixs){
        model.rotate(frame_delta_ms/rotate_param, glm::vec3(0, 1, 0));
        model.rotate(frame_delta_ms/rotate_param, glm::vec3(1, 0, 0));
    }

    shader_simple->use();
    shader_simple->update_camera(&camera);

    for(int i=0;i<4;++i){
        shader_simple->set_uniform("color", colors[i]*(loop_cnt%1000/1000.f));
        shader_simple->update_model(model_matrixs[i]);
        vaos[i]->draw_array(GL_TRIANGLES);
    }
}

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