//
// Created by Jelena Djuric on 13.8.26..
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_HPP
#define MATF_RG_PROJECT_GUICONTROLLER_HPP
#include "engine/core/Controller.hpp"

namespace app {

class GUIController :public  engine::core::Controller {
public:
      std::string_view name() const override {
          return "GUI";
      }
private:
    void initialize() override;

    void poll_events() override;

    void draw() override;
};

}// namespace app

#endif//MATF_RG_PROJECT_GUICONTROLLER_HPP
