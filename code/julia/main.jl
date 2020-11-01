using JuMP

include("SensorNetwork.jl")
using .SensorNetwork

# global parameters
const communication_radius = 1
const reception_radius = 1
const reception_level = 2
const instance_path = "/Users/taleboy/Desktop/metaheuristics2020/instances/captANOR1500_18_100.txt"

# parsing the instance
target_coordinates = parse_instance(instance_path)

# create a dataset from the instance
dataset = Dataset(communication_radius, reception_radius, reception_level, target_coordinates)

# visualize it
# visualize_instance(dataset, "instance.png")

# build and solve the MIP
# model, variables = build_MIP(dataset)
model = build_LP(dataset)
set_optimizer_attribute(model, "Method", 1)  # dual simplex
solution = solve_LP!(model)

# check admissibility
# println("Is the solution admissible ? $(check_admissible(solution))")

# visualize the solutions
# visualize_solution(solution, "solution.png")

# write_solution("test.txt", solution)

# parsed_solution = parse_solution("test.txt", dataset)

# println(parsed_solution)
