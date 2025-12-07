/*
 * renderer.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once

#include <vector>
#include <memory>

#include <glm/mat4x4.hpp>
#include "base_system.h"

#include "rendering/base_renderer.h"

struct GLFWwindow;
class World;

class RenderSystem : public BaseSystem<RenderSystem>
{
public:
    void init( Engine& engine ) override;
    void shutdown() override;
    void update( World& world, double dt ) override;
    void draw( World& world ) override;

    GLFWwindow * get_window() { return window; };

    void set_camera( const glm::mat4& view, const glm::mat4& proj );

private:
    GLFWwindow* window = nullptr;
    glm::mat4 mvp;
    Engine * engine;
    std::vector<std::unique_ptr<BaseRenderer>> renderers; 

    void begin_frame();
    void end_frame();
    bool make_window();
    void make_renderers();
};