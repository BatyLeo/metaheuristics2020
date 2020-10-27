using JuMP

include("SensorNetwork.jl")
using .SensorNetwork

# global parameters
const communication_radius = 2
const reception_radius = 1
const reception_level = 1
const instance_path = "/Users/taleboy/Desktop/metaheuristics2020/instances/captANOR1500_18_100.txt"

# parsing the instance
target_coordinates = parse_instance(instance_path)

# create a dataset from the instance
dataset = Dataset(communication_radius, reception_radius, reception_level, target_coordinates)

solution = parse_solution("sol.txt", dataset)

# check admissibility
println("Is the solution admissible ? $(check_admissible(solution))")

# visualize the solutions
visualize_solution(solution, "solution.png")
