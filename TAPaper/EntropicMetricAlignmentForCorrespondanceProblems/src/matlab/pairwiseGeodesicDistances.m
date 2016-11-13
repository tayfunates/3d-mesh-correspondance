function [ DistMat ] = pairwiseGeodesicDistances( model, algorithm_type )
%%Adapted from https://www.mathworks.com/matlabcentral/fileexchange/18168-exact-geodesic-for-triangular-meshes/content/example5.m
%%Gets a model of a triangular mesh and extracts a pairwise geodesic
%%distance matrix from the mesh
%% [input] model: a triangular mesh containing vertices and faces. model struct must have 'v' and 'f' as the fields of vertices and faces, respectively.
%% [output] DistMat: Output symmetrical matrix containing the geodesic distances of all vertices inside the model
                    
global geodesic_library;                
geodesic_library = 'geodesic_release';      %"release" is faster and "debug" does additional checks

[mesh, edge_to_vertex, edge_to_face] = geodesic_new_mesh(model.v,model.f);         %initilize new mesh and receive edge info

algorithm = geodesic_new_algorithm(mesh, algorithm_type);      %initialize new geodesic algorithm

N = length(model.v);                                  %number of points in a mesh
DistMat = [];
for vertex_id = 1:N
    fprintf('%d/%d\n', vertex_id, N);
        
    source_points = {};
    source_points{1} = geodesic_create_surface_point('vertex',vertex_id, model.v(vertex_id,:));    
    geodesic_propagate(algorithm, source_points);   %propagation stage of the algorithm (the most time-consuming)

    [source_ids, distances] = geodesic_distance_and_source(algorithm);     %for every vertex, figure out which source it belongs to
    DistMat(vertex_id, :) = distances;

end
    geodesic_delete;

end

