include("SensorNetwork.jl")
using .SensorNetwork

# global parameters
const instances_folder = "/Users/taleboy/Desktop/metaheuristics2020/instances"

const instance_file_names = Array{String, 1}(["captANOR150_7_4", "captANOR225_8_10", "captANOR625_12_100",
                                        "captANOR900_15_20", "captANOR1500_15_100", "captANOR1500_18_100"])

const radius_array = Array{Tuple{Int64, Int64}, 1}([(1, 1), (1, 2), (2, 2), (2, 3)])
const reception_levels_array = Array{Int, 1}([1, 2, 3])

results_file = "better_solutions.txt"
rm(results_file, force=true)

open(results_file, "w") do io
    for instance_file in instance_file_names
        instance_path = "$instances_folder/$instance_file"
        # parsing the instance
        target_coordinates = parse_instance("$instance_path.txt")

        for reception_level in reception_levels_array
            for (reception_radius, communication_radius) in radius_array
                # create a dataset from the instance
                dataset = Dataset(communication_radius, reception_radius, reception_level, target_coordinates)

                solution_path = "../../better_solutions/k_$(reception_level)/$(reception_radius)_$(communication_radius)/$(instance_file).txt"
                solution = parse_solution(solution_path, dataset)

                # check admissibility
                println("instance=$instance_file, k=$reception_level, R_capt=$reception_radius, R_com=$communication_radius, best_solution=$(solution.score)")
                write(io, "$instance_file $reception_level $reception_radius $communication_radius $(solution.score)\n")
            end
        end
    end
end
