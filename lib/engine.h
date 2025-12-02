/*
 * engine.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "systems/base_system.h"
#include "world.h"

class RenderSystem;

class Engine
{
public:
    void init();
    void run();
    void shutdown();

    void stop_running() { running = false; }

    template<typename T>
    T* get_system()
    {
        for( auto& sys : systems ) {
            if( sys->type() == typeid(T) )
                return static_cast<T*>(sys.get());
        }
        return nullptr;
    }

private:
    bool running = true;
    std::vector<std::unique_ptr<ISystem>> systems;

    World world;
};