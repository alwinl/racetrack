/*
 * component_registry.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <unordered_map>
#include <string>
#include <functional>

#include "../entity.h"
#include "../world.h"

#include "nlohmann/json.hpp"

using ComponentFactory = std::function<void(World&, Entity, const nlohmann::json&)>;

class ComponentRegistry
{
public:
    static void register_factory( const std::string& name, ComponentFactory factory )
        { get().factories[name] = factory; }

    static bool create(const std::string& name, World& world, Entity e, const nlohmann::json& data)
    {
        auto& map = get().factories;

        auto it = map.find(name);
        if( it != map.end() ) {
            it->second( world, e, data );
            return true;
        }

        return false;
    }

private:
    std::unordered_map<std::string, ComponentFactory> factories;

    static ComponentRegistry& get()
    {
        static ComponentRegistry instance;
        return instance;
    }
};

template<typename T>
struct ComponentRegistrar
{
    ComponentRegistrar( const std::string& name )
    {
        auto extraction_func = 
            []( World& world, Entity e, const nlohmann::json& json )
            {
                T component;
                component.from_json(json);
                world.add_component<T>(e, component);
            };

        ComponentRegistry::register_factory( name, extraction_func );
    }
};

void force_component_registration();
