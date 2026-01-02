/*
 * registry.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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
#include <typeindex>

#include "world.h"

#include "nlohmann/json.hpp"

using ComponentCreator = std::function<void(World&, Entity, const nlohmann::json&)>;
using ComponentFlusher =  std::function<void(World&)>;
using ComponentDestroyer =  std::function<void(World&, Entity)>;

class ComponentRegistry
{
public:
    static void register_funcs( const std::string& name, std::type_index t, ComponentCreator creator, ComponentDestroyer destroyer, ComponentFlusher flusher );

    static bool create( World& world, Entity e, const std::string& name, const nlohmann::json& data);
    static bool destroy( World& world, Entity e );
    static bool flush_all( World& world );
    static void clear_all( World& world );

private:
    std::unordered_map<std::string, ComponentCreator> creators;
    std::unordered_map<std::type_index, ComponentDestroyer> destroyers;
    std::unordered_map<std::type_index, ComponentFlusher> flushers;

    std::unordered_map<std::string, std::type_index> type_lookup;
    std::unordered_map<Entity, std::vector<std::type_index>> entity_typelist;

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
        auto create_func =
            []( World& world, Entity e, const nlohmann::json& json )
            {
                T component;
                component.from_json(json);
                world.add_component<T>(e, component);
            };

        auto destroy_func =
            []( World& world, Entity e )
            {
                world.remove_component<T>( e );
            };

        auto flush_func =
            []( World& world )
            {
                world.flush_components<T>();
            };

        ComponentRegistry::register_funcs( name, typeid(T), create_func, destroy_func, flush_func );
    }
};
