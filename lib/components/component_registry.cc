/*
 * component_registry.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "point_component.h"
#include "transform_component.h"
#include "triangle_component.h"
#include "velocity_component.h"
#include "track_component.h"
#include "lake_component.h"

ComponentRegistrar<PointComponent> PointComponent::registrar("Point");
ComponentRegistrar<TriangleComponent> TriangleComponent::registrar("Triangle");
ComponentRegistrar<TrackComponent> TrackComponent::registrar("Track");
ComponentRegistrar<LakeComponent> LakeComponent::registrar("Lake");
ComponentRegistrar<TransformComponent> TransformComponent::registrar("Transform");
ComponentRegistrar<VelocityComponent> VelocityComponent::registrar("Velocity");

void force_component_registration() {}
