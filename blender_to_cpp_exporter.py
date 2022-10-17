# This file is intended to be ran inside Blender
# To do so, open a "text editor" window and copy/paste or open this file.
# The code is written to the system output

# Important notes to exporting a model:
# Make sure it's triangulated in Blender
# Make sure you're in object mode
# Make sure the object doesn't have strange scales, translations, etc

# Limitations:
# Single mesh supported

import bpy  

from os import system

# Try Windows and Linux commands to clear the console
system('cls')
system('clear')

# TODO: test eventually
#bpy.ops.mesh.quads_convert_to_tris(quad_method='FIXED', ngon_method='BEAUTY')

current_obj = bpy.data.meshes[0]
verts_local = [v.co for v in current_obj.vertices.values()]
verts_world = [v_local for v_local in verts_local]

current_obj.calc_loop_triangles()

current_obj.calc_normals_split()

normals = [
    l.normal
    for l in current_obj.loops
]

indices = [
    current_obj.loops[loop_index].vertex_index
    for triangle in current_obj.loop_triangles
    for loop_index in triangle. loops
]

vertices = [
    v.co
    for v in current_obj.vertices
]

vertex_uv_coords = [
    current_obj.uv_layers.active.data[loop_index].uv
    for triangle in current_obj.loop_triangles
    for loop_index in triangle. loops
]

vertex_colors = [
    list(current_obj.materials[triangle.material_index].node_tree.nodes["Diffuse BSDF"].inputs["Color"].default_value)
    for triangle in current_obj.loop_triangles
    for loop_index in triangle. loops
]

print('#include <tuple>')

print('std::tuple<double, double, double, double> geom_vertex_list [] {');
for vert in vertices:
    print("{{ {v[0]}, {v[1]}, {v[2]}, 0.0 }},".format(v=vert))
print('};\n');

tri_count = 0

print('std::tuple<int, int, int> geom_vertex_indices [] {')

index_acc = []
for index in indices:
    index_acc.append(str(index))
    if len(index_acc) == 3:
        print("{{ {v_i} }},".format(v_i=', '.join(index_acc)))
        index_acc = []
        tri_count = tri_count + 1
print('};\n');



print('std::tuple<double, double, double, double> geom_normal_list [] {');
for normal in normals:
    print("{{ {n[0]}, {n[1]}, {n[2]}, 1.0 }},".format(n=normal))
print('};\n');

print("const int triangleCount = {0};\n".format(tri_count))

print('std::tuple<int, int, int> geom_vertex_colors [] {')

index_acc = []
for color in vertex_colors:    
    print("{{ {c[0]}, {c[1]}, {c[2]} }},".format(c=[int(c*255) for c in color]))
    
print('};\n');


print('std::tuple<double, double> geom_uv_coords [] {')

for uv in vertex_uv_coords:
    print("{{ {c[0]}, {c[1]} }},".format(c=uv))


print('};\n');
        
print('// triangles=', tri_count)
print('// vertices=', len(vertices))
print('// indices=', len(indices))

print('// split normals=', len(normals))
print('// colors=', len(vertex_colors))
print('// uvs=', len(vertex_uv_coords))

