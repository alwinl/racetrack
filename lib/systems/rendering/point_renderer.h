/*
 * pointrenderer.h Copyright 2025 Alwin Leerling <dna.leerling@gmail.com>
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

#include <glm/glm.hpp>

#include "base_renderer.h"
#include "shader.h"

class PointRenderer : public BaseRenderer
{
public:
    void init() override;
    void destroy() override;
    void upload( const World& world ) override;
    void draw() override;

    void set_mvp( glm::mat4& mvp ) override;

private:
    Shader shader;
    unsigned vao = 0;
    unsigned vbo = 0;

    struct vertex
    {
        glm::vec3 position;
        glm::vec3 colour;
    };

    std::vector<vertex> cpu_buffer;
};
