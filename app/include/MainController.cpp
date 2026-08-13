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


    void MainController::draw_backpack() {
        //Model
        auto  resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics   = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* backpack=resources->model("backpack");

        //Shader
        engine::resources::Shader* shader=resources->shader("basic");

        shader->use();
        shader->set_mat4("projection",graphics->projection_matrix());
        shader->set_mat4("view",graphics->camera()->view_matrix());
        glm::mat4 model=glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model",model);


        backpack->draw(shader);
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
    void MainController::update() {
        update_camera();
    }

    void MainController::begin_draw() {
            engine::graphics::OpenGL::clear_buffers();
    }
    void MainController::draw_skyboxes() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("skybox");
        auto shader=resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader,skybox);
    }
    void MainController::draw_train() {

        auto  resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics   = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* backpack=resources->model("train");

        //Shader
        engine::resources::Shader* shader=resources->shader("trains");

        shader->use();
        shader->set_mat4("projection",graphics->projection_matrix());
        shader->set_mat4("view",graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        float s = 0.003f;
        model = glm::scale(model, glm::vec3(s));
        model = glm::translate(model, glm::vec3(0.0f, -431.0f, -588.0f));
        shader->set_mat4("model", model);

        backpack->draw(shader);

    }


    void MainController::draw() {
        //clear buffer color , depth buffer - pre crtanja to radi begin_draw()
        draw_backpack();
        draw_skyboxes();
        draw_train();

        //swap buffers posle crtanja
    }

    void MainController::end_draw() {
        auto platform= engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

}// namespace app