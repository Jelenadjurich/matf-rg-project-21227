//
// Created by Jelena Djuric on 12.8.26..
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include "engine/core/App.hpp"
#include "engine/core/Controller.hpp"

namespace app {

class MainController : public engine::core::Controller {
public:
    void initialize();
    bool loop();
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
