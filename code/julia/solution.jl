using DataStructures

mutable struct Solution
    target_has_sensor::Array{Bool, 1}
    score::Int64

    dataset::Dataset

    function Solution(target_has_sensor::Array{Bool, 1}, dataset::Dataset; score::Int64=0)
        new(target_has_sensor, score, dataset)
    end
end

function check_admissible(solution::Solution)
    number_targets = solution.dataset.number_targets

    # Check that all targets have the required reception level
    for target_index in 1:number_targets
        # iterate over all communication neighbors to count all convering sensors
        nb_covering_sensors = 0
        for communication_neighbor_index in solution.dataset.communication_neighbors[target_index]
            if solution.target_has_sensor[communication_neighbor_index]
                nb_covering_sensors += 1
            end

            # break if reception level is reached
            if nb_covering_sensors >= solution.dataset.reception_level
                break
            end
        end

        # Check if reception level is correct for target
        if nb_covering_sensors < solution.dataset.reception_level
            return false
        end
    end

    # Check that all sensors are accessible from source
    targets_to_explore = Queue{Int}()
    is_connected_to_source = [false for _ in 1:number_targets]
    # Adding sensors communicating with the source
    for target_index in solution.dataset.source_communication_neighbors
        if solution.target_has_sensor[target_index]
            enqueue!(targets_to_explore, target_index)
            is_connected_to_source[target_index] = true
        end
    end
    # exploration
    while length(targets_to_explore) > 0
        target_index = dequeue!(targets_to_explore)
        for other_target_index in solution.dataset.communication_neighbors[target_index]
            if other_target_index <= number_targets  # check if different than source
                if solution.target_has_sensor[other_target_index] & !is_connected_to_source[other_target_index]
                    enqueue!(targets_to_explore, other_target_index)
                    is_connected_to_source[other_target_index] = true
                end
            end
        end
    end
    # final check
    for (target_index, is_connected) in enumerate(is_connected_to_source)
        if solution.target_has_sensor[target_index] & !is_connected
            return false
        end
    end

    return true
end