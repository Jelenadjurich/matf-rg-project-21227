//
// Created by Jelena Djuric on 12.8.26..
//

#ifndef MATF_RG_PROJECT_MYAPP_H
#define MATF_RG_PROJECT_MYAPP_H
#include "engine/core/App.hpp"
#include "MainController.hpp"

namespace app {

class MyApp: public engine::core::App{
    void app_setup() override;
    MainController mainController;

};

}// namespace app

#endif//MATF_RG_PROJECT_MYAPP_H
