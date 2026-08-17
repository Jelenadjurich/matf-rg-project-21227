//
// Created by Jelena Djuric on 12.8.26..
//

#include "MyApp.hpp"

#include "GuiController.hpp"
#include "MainController.hpp"
#include "spdlog/spdlog.h"

namespace app {
    void MyApp::app_setup() {
    spdlog::info("App Setup completed!");

    auto main_controller=register_controller<MainController>();
    auto gui_controller=register_controller<GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    main_controller->before(gui_controller);//gui se crta na kraju nakon sto su svi ostali objekti nacrani
}

}// namespace app