//
// Created by Jelena Djuric on 12.8.26..
//

#include "MyApp.hpp"
#include "MainController.hpp"
#include "spdlog/spdlog.h"

namespace app {
    void MyApp::app_setup() {
    spdlog::info("App Setup completed!");

    register_controller<MainController>();
}

}// namespace app