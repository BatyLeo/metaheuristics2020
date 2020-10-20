mutable struct Dataset
    number_targets::Int64
    communication_radius::Int64
    reception_radius::Int64
    reception_level::Int64

    source_coordinates::Tuple{Float64, Float64}
    target_coordinates::Array{Tuple{Float64, Float64}, 1}

    communication_neighbors::Array{Array{Int64, 1}, 1}
    reception_neighbors::Array{Array{Int64, 1}, 1}
    source_communication_neighbors::Array{Int64, 1}
    communication_edges::Array{Tuple{Int64, Int64}, 1}
    edge_to_index::Dict{Tuple{Int64, Int64}, Int64}
    arc_to_index::Dict{Tuple{Int64, Int64}, Int64}

    function Dataset(communication_radius::Int64, reception_radius::Int64, reception_level::Int64,
                     target_coordinates::Array{Tuple{Float64, Float64}, 1})
        number_targets = length(target_coordinates)
        source_coordinates = (0, 0)

        # initialize neighbors arrays
        communication_neighbors = [Array{Int64, 1}() for i in 1:number_targets]
        reception_neighbors = [Array{Int64, 1}() for i in 1:number_targets]
        source_communication_neighbors = Array{Int64, 1}()
        communication_edges = Array{Tuple{Int64, Int64}, 1}()
        edge_to_index = Dict{Tuple{Int64, Int64}, Int64}()
        arc_to_index = Dict{Tuple{Int64, Int64}, Int64}()

        for (target_index, target) in enumerate(target_coordinates)
            for (other_target_index, other_target) in enumerate(target_coordinates)
                # check if both targets are different
                if target_index == other_target_index
                    continue
                end

                distance = square_distance(target, other_target)

                # check if both targets are in reception range
                if distance <= reception_radius^2
                    push!(reception_neighbors[target_index], other_target_index)
                end

                # check if both targets are in communication range
                if distance <= communication_radius^2
                    push!(communication_neighbors[target_index], other_target_index)

                    # add an edge if increasing index
                    if target_index < other_target_index
                        push!(communication_edges, (target_index, other_target_index))
                        edge_to_index[(target_index, other_target_index)] = length(communication_edges)
                    end
                end
            end

            # check if target are in communication range of the source
            if square_distance(target, source_coordinates) <= communication_radius^2
                push!(source_communication_neighbors, target_index)
                push!(communication_neighbors[target_index], number_targets + 1)

                push!(communication_edges, (target_index, number_targets + 1))
                edge_to_index[(target_index, number_targets + 1)] = length(communication_edges)
            end
        end

        for (key, value) in edge_to_index
            arc_to_index[key] = value
            arc_to_index[(key[2], key[1])] = value + length(communication_edges)
        end

        new(number_targets, communication_radius, reception_radius, reception_level, source_coordinates,
            target_coordinates, communication_neighbors, reception_neighbors, source_communication_neighbors,
            communication_edges, edge_to_index, arc_to_index)
    end

end #end of struct
