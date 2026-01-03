/*
 * world.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <any>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <cstdint>

using Entity = std::uint32_t;
constexpr uint32_t InvalidEntity = (uint32_t)-1;

class World
{
private:
    template< typename T>
    class Store
    {
    public:
        using ComponentMap = std::unordered_map<Entity, T>;

        T* add( Entity e, const T& component ) { components[e] = component; return &(components[e]); }
        void remove( Entity e ) { components.erase(e); }
        bool has(Entity e) const { return components.count(e); }

        T* get( Entity e ) { auto it = components.find(e); return it != components.end() ? &it->second : nullptr; }
        const T* get( Entity e ) const { auto it = components.find(e); return it != components.end() ? &it->second : nullptr; }

        ComponentMap& all() { return components; }
        const ComponentMap& all() const { return components; }

    private:
        ComponentMap components;
    };

public:
    Entity create_entity() { return next_id++; }
    void reset_entity_ids() { next_id = 0; };
	void remove_entity( Entity e ) {};

    template<typename T> T& add_component( Entity e, const T& component ) { return *component_store<T>().add(e, component ); }
    template<typename T> T* get_component( Entity e ) { return component_store<T>().get(e ); }
    template<typename T> void remove_component( Entity e ) { removal_queue<T>().push_back(e); }
    template<typename T> void flush_components();

	template<typename T> void set_resource( T resource ) { resources.insert( {typeid(T), std::move(resource)} ); }
	template<typename T> T& get_resource( ) { return std::any_cast<T&>( resources.at(typeid(T)) ); }
	template<typename T> bool has_resource( ) { return resources.contains( typeid(T) ); }

    template<typename T> Store<T>& storage() { return component_store<T>(); }
    template<typename T> const Store<T>& storage() const { return component_store<T>(); }

private:
    Entity next_id = 0;
	std::unordered_map<std::type_index, std::any> resources;

    template<typename T>
    Store<T>& component_store() const
    {
        static Store<T> storage;
        return storage;
    }

    template<typename T>
    std::vector<Entity>& removal_queue()
    {
        static std::vector<Entity> queue;
        return queue;
    }
};

template<typename T>
void World::flush_components( )
{
    auto& store = component_store<T>();

    for( Entity e : removal_queue<T>() )
        store.remove(e);

    removal_queue<T>().clear();
}

