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

#include <vector>

#include "entity.h"
#include "components/component_storage.h"

class World
{
public:
    Entity create_entity() { return next_id++; }

    template<typename T>
    void remove_entity( Entity e ) { mark_for_removal<T>( e ); }

    template<typename T>
    T& add_component( Entity e, const T& component )
        { return *get_storage<T>().add(e, component ); }

    template<typename T> 
    T* get_component( Entity e )
        { return get_storage<T>().get(e ); }

    template<typename T>
    ComponentStorage<T>& storage()
        { return get_storage<T>(); }

    template<typename T>
    const ComponentStorage<T>& storage() const
        { return get_storage<T>(); }

    template<typename T>
    void flush_components( ) 
    {
        auto& list = removal_queue<T>();
        auto& store = get_storage<T>();

        for( Entity e : list )
            store.remove(e);

        list.clear();
    }

private:
    Entity next_id = 1;

    template<typename T>
    ComponentStorage<T>& get_storage() const
    {
        static ComponentStorage<T> storage;
        return storage;
    }

    template<typename T>
    std::vector<Entity>& removal_queue()
    {
        static std::vector<Entity> queue;
        return queue;
    }

    template<typename T>
    void mark_for_removal(Entity e)
    {
        removal_queue<T>().push_back(e);
    }
};
