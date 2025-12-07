/*
 * base_system.h Copyright 2025 Alwin Leerling <dna.leerling@gmail.com>
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

#include <typeindex>

class Engine;
class World;

class ISystem
{
public:
    virtual ~ISystem() = default;

    virtual std::type_index type() const = 0;

    virtual void init() = 0;
    virtual void update( World& world, double dt ) = 0;
    virtual void draw( World& world ) = 0;
    virtual void shutdown() = 0;
};

template<typename T>
class BaseSystem : public ISystem
{
public:
    BaseSystem( Engine * eng) : engine(eng) {}

    std::type_index type() const override { return typeid(T); }

    void init() override {}
    void update( World& world, double dt ) override {}
    void draw( World& world ) override {}
    void shutdown() override {}

protected:
    Engine * engine;
};