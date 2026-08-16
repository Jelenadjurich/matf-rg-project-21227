

//
// Created by Jelena Djuric on 12.8.26..
//

#include "MainController.hpp"


#include "GuiController.hpp"
#include "engine/core/App.hpp"
#include "engine/core/Controller.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "glad/glad.h"


namespace app {

    class MainPlatformEventObserver: public  engine::platform::PlatformEventObserver {
        public:
          void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position){
        auto gui_controler = engine::core::Controller::get<GUIController>();
        if (!gui_controler->is_enabled()) {
            auto camera= engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            camera->rotate_camera(position.dx,position.dy);
        }
    }

    void MainController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();

    }
    bool  MainController::loop() {
            auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
            if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
                return false;
            }
            return true;
    }

    void MainController::update_camera() {
        auto gui_controler = engine::core::Controller::get<GUIController>();
        if (gui_controler->is_enabled()) {
            return;
        }
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics   = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        float dt = platform->dt();//ptoteko vrme izmedju 2 frejma
        if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD,dt);
        }

        if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD,dt);
        }

        if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT,dt);
        }

        if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT,dt);
        }
    }

    void MainController::update_lighting() {
        auto gui_controler = engine::core::Controller::get<GUIController>();
        if (gui_controler->is_enabled()) {
            return;
        }
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        float dt = platform->dt();
        float move_speed = 2.0f;      // brzina pomeranja svetla
        float light_speed = 1.0f;     // brzina promene intenziteta

        // Pomeranje point light-a po X/Z strelicama
        if (platform->key(engine::platform::KeyId::KEY_RIGHT).is_down()) {
            point_light_position.x += move_speed * dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_LEFT).is_down()) {
            point_light_position.x -= move_speed * dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_UP).is_down()) {
            point_light_position.z -= move_speed * dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_DOWN).is_down()) {
            point_light_position.z += move_speed * dt;
        }

        // Pomeranje point light-a po Y (visina) - Page Up / Page Down
        if (platform->key(engine::platform::KeyId::KEY_PAGE_UP).is_down()) {
            point_light_position.y += move_speed * dt;
        }
        if (platform->key(engine::platform::KeyId::KEY_PAGE_DOWN).is_down()) {
            point_light_position.y -= move_speed * dt;
        }

        // Intenzitet point light-a: taster "=" (+) pojačava, "-" slabi
        if (platform->key(engine::platform::KeyId::KEY_EQUAL).is_down()) {
            point_light_color += glm::vec3(light_speed * dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_MINUS).is_down()) {
            point_light_color -= glm::vec3(light_speed * dt);
        }
        point_light_color = glm::clamp(point_light_color, glm::vec3(0.0f), glm::vec3(3.0f));
    }

    void MainController::update_sequence() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        float dt = platform->dt();

        // ACTION_X: pritisak tastera T pokreće niz (samo ako je trenutno neaktivan)
        if (platform->key(engine::platform::KeyId::KEY_T).state() ==
            engine::platform::Key::State::JustPressed) {
            if (sequence_stage == 0 || sequence_stage == 3) {
                sequence_stage = 1;
                sequence_timer = 0.0f;

                // vracamo scenu u pocetno stanje pre novog pokretanja niza
                lamp_visible = true;
                point_light_color = glm::vec3(1.0f, 0.9f, 0.6f);
            }
        }

        if (sequence_stage == 0 || sequence_stage == 3) {
            return; // niz nije aktivan, nema sta da se azurira
        }

        sequence_timer += dt;

        if (sequence_stage == 1 && sequence_timer >= SEQUENCE_M_SECONDS) {
            // EVENT_A: boja point light-a postaje crvena
            point_light_color = glm::vec3(1.0f, 0.0f, 0.0f);

            sequence_stage = 2;
            sequence_timer = 0.0f; // reset - sada brojimo N sekundi do EVENT_B
        }
        else if (sequence_stage == 2 && sequence_timer >= SEQUENCE_N_SECONDS) {
            // EVENT_B: lampa nestaje sa scene
            lamp_visible = false;

            sequence_stage = 3; // niz zavrsen
        }
    }

    void MainController::update() {
        update_camera();
        update_lighting();
        update_sequence();
    }

    void MainController::begin_draw() {
            glClearColor(0.82f, 0.80f, 0.74f, 1.0f);
            engine::graphics::OpenGL::clear_buffers();
    }
    void MainController::draw_skyboxes() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("skybox");
        auto shader=resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader,skybox);
    }

    void MainController::draw_nefertiti() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* nefertiti = resources->model("nefertiti");

        engine::resources::Shader* shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_vec3("dirLight_direction", dir_light_direction);
        shader->set_vec3("dirLight_color", dir_light_color);
        shader->set_vec3("pointLight_position", point_light_position);
        shader->set_vec3("pointLight_color", point_light_color);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, -3.0f));   // ulevo
        model = glm::scale(model, glm::vec3(8.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.495f, -0.27f, -0.005f));
        shader->set_mat4("model", model);

        nefertiti->draw(shader);
    }

    void MainController::draw_cezar() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* cezar = resources->model("cezar");

        engine::resources::Shader* shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_vec3("dirLight_direction", dir_light_direction);
        shader->set_vec3("dirLight_color", dir_light_color);
        shader->set_vec3("pointLight_position", point_light_position);
        shader->set_vec3("pointLight_color", point_light_color);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.3f, 1.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.30f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.37f, -4.585f, 2.03f));
        shader->set_mat4("model", model);

        cezar->draw(shader);
    }


void MainController::draw_floor() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* floor = resources->model("floor");

        engine::resources::Shader* shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_vec3("dirLight_direction", dir_light_direction);
        shader->set_vec3("dirLight_color", dir_light_color);
        shader->set_vec3("pointLight_position", point_light_position);
        shader->set_vec3("pointLight_color", point_light_color);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        shader->set_mat4("model", model);

        floor->draw(shader);
    }

void MainController::draw_wall() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* wall = resources->model("wall");

        engine::resources::Shader* shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_vec3("dirLight_direction", dir_light_direction);
        shader->set_vec3("dirLight_color", dir_light_color);
        shader->set_vec3("pointLight_position", point_light_position);
        shader->set_vec3("pointLight_color", point_light_color);

        // ZID 1 - iza skulptura
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-5.0f, -1.0f, -8.0f));
        shader->set_mat4("model", model);
        wall->draw(shader);


        // ZID 2 - bocni zid
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(5.0f, -1.0f, -8.0f));
        model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f, -1.0f, 0.0f));
        shader->set_mat4("model", model);
        wall->draw(shader);
    }


void MainController::draw_lamp() {
        if (!lamp_visible) {
            return; // EVENT_B je "obrisao" lampu sa scene
        }

        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* lamp = resources->model("lamp");

        engine::resources::Shader* shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_vec3("dirLight_direction", dir_light_direction);
        shader->set_vec3("dirLight_color", dir_light_color);
        shader->set_vec3("pointLight_position", point_light_position);
        shader->set_vec3("pointLight_color", point_light_color);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f, -1.0f, -1.0f)); // lampa stoji na podu
        model = glm::scale(model,glm::vec3(1.25f));  // velicina lampe
        shader->set_mat4("model", model);

        lamp->draw(shader);
    }



void MainController::draw() {
        //clear buffer color , depth buffer - pre crtanja to radi begin_draw()

        //draw_skyboxes();

        draw_nefertiti();
        draw_cezar();
        draw_floor();
        draw_wall();
        draw_lamp();

        //swap buffers posle crtanja
    }

    void MainController::end_draw() {
        auto platform= engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

}// namespace app