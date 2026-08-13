//
// Created by Jelena Djuric on 12.8.26..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include "engine/core/App.hpp"
#include "engine/core/Controller.hpp"
#include <glm/glm.hpp>

namespace app {

class MainController : public engine::core::Controller {
public:
    void initialize();
    bool loop();

    void update_camera();
    void update_lighting();
    void update_sequence();
    void update() override;

    void begin_draw() override;

    void draw_skyboxes();

    void draw_nefertiti();
    void draw_cezar();
    void draw_floor();
    void draw_wall();
    void draw_lamp();
    void draw() override;

    void end_draw() override;

    // --- Deljeno stanje osvetljenja (menja se preko tastature) ---
    glm::vec3 dir_light_direction{-0.3f, -1.0f, -0.3f};
    glm::vec3 dir_light_color{1.0f, 1.0f, 1.0f};

    glm::vec3 point_light_position{0.0f, 2.0f, -1.0f};
    glm::vec3 point_light_color{1.0f, 0.9f, 0.6f};

    // --- Niz događaja (ACTION -> EVENT_A -> EVENT_B) ---
    int   sequence_stage{0};      // 0 = neaktivno, 1 = ceka EVENT_A, 2 = ceka EVENT_B, 3 = zavrseno
    float sequence_timer{0.0f};   // vreme proteklo od poslednjeg okidača
    bool  lamp_visible{true};     // da li se lampa crta (EVENT_B je nestajanje)

    static constexpr float SEQUENCE_M_SECONDS = 2.0f; // ACTION -> EVENT_A
    static constexpr float SEQUENCE_N_SECONDS = 2.0f; // EVENT_A -> EVENT_B
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPPsss