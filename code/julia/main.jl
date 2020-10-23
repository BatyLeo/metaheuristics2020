using JuMP

include("SensorNetwork.jl")
using .SensorNetwork

# global parameters
const communication_radius = 2
const reception_radius = 1
const reception_level = 1
const instance_path = "/Users/taleboy/Desktop/metaheuristics2020/instances/captANOR225_8_10.txt"

# parsing the instance
target_coordinates = parse_instance(instance_path)

# create a dataset from the instance
dataset = Dataset(communication_radius, reception_radius, reception_level, target_coordinates)

# visualize it
visualize_instance(dataset, "instance.png")

# build and solve the MIP
model, variables = build_MIP(dataset)
solution = solve_MIP!(model, variables, dataset)

# check admissibility
println("Is the solution admissible ? $(check_admissible(solution))")

# visualize the solutions
visualize_solution(solution, "solution.png")
