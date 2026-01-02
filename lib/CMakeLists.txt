#
# CMakeLists.txt Copyright 2024 Alwin Leerling dna.leerling@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#

find_package( glfw3 )

add_library(
    racetrack_lib

    core/engine.cc
    core/world.cc
    core/registry.cc
	core/inputqueue.cc

    systems/render_system.cc
    systems/time_system.cc
    systems/input_system.cc
    systems/resource_system.cc
    systems/physics_system.cc
    systems/geometry_system.cc
    systems/track_system.cc

    render_pipeline/shader.cc
    render_pipeline/point_renderer.cc
    render_pipeline/triangle_renderer.cc
    render_pipeline/lake_renderer.cc
    render_pipeline/mesh_renderer.cc
)

target_link_libraries(racetrack_lib PRIVATE glad glfw)
target_include_directories(racetrack_lib INTERFACE ./)
target_include_directories(racetrack_lib PUBLIC rendering)