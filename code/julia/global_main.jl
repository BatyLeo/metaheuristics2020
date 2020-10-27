using JuMP

include("SensorNetwork.jl")
using .SensorNetwork

# global parameters
const instances_folder = "/Users/taleboy/Desktop/metaheuristics2020/instances"

const instance_file_names = Array{String, 1}(["captANOR150_7_4", "captANOR225_8_10", "captANOR625_12_100",
                                        "captANOR900_15_20", "captANOR1500_15_100", "captANOR1500_18_100"])

const radius_array = Array{Tuple{Int64, Int64}, 1}([(1, 1), (1, 2), (2, 2), (2, 3)])
const reception_levels_array = Array{Int, 1}([1, 2, 3])


inferior_bounds = Dict{String, Float64}()

for instance_file in instance_file_names
    instance_path = "$instances_folder/$instance_file"
    # parsing the instance
    target_coordinates = parse_instance("$instance_path.txt")

    for (reception_radius, communication_radius) in radius_array
        for reception_level in reception_levels_array
            println("$instance_file, $reception_radius, $communication_radius, $reception_level")
            # create a dataset from the instance
            dataset = Dataset(communication_radius, reception_radius, reception_level, target_coordinates)

            # visualize it
            visualize_instance(dataset, "instance_figures/$instance_file.png")

            # build and solve the MIP
            model = build_LP(dataset, log_file_path="LP_logs/k_$reception_level/$(reception_radius)_$communication_radius/$instance_file.txt")
            inferior_bound = solve_LP!(model)

            inferior_bounds[instance_file] = inferior_bound

            println("Inferior bound: $inferior_bound")
            print("\n\n\n\n")
        end
    end
end

results_file = "inferior_bounds.txt"
rm(results_file, force=true)
open(results_file, "w") do io
    for (key, value) in inferior_bounds
        write(io, "$key $value\n")
    end
end
