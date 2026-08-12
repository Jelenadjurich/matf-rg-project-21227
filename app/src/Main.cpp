#include "MyApp.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <engine/core/Engine.hpp>
#include <iostream>

int main(int argc, char** argv) {
    auto app = std::make_unique<app::MyApp>();
    return app->run(argc, argv);

}