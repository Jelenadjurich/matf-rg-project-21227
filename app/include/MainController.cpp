//
// Created by Jelena Djuric on 12.8.26..
//

#include "MainController.hpp"

#include "engine/core/App.hpp"
#include "engine/core/Controller.hpp"
#include "engine/platform/PlatformController.hpp"

namespace app {
    void MainController::initialize() {

    }
    bool  MainController::loop() {
            auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
            if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
                return false;
            }
            return true;
    }
}// namespace app