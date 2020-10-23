function parse_instance(file_path::String)
    target_coordinates = Array{Tuple{Float64, Float64}, 1}()

    io = open(file_path, "r")
    lines = split(read(io, String), "\n")
    for line in lines
        target = split(line)
        if length(target) != 3
            continue
        end
        index, coordinates = parse(Int64, target[1]), (parse(Float64, target[2]),parse(Float64, target[3]))

        # don't push source
        if index != 0
            push!(target_coordinates, coordinates)
        end
    end

    return target_coordinates
end


function parse_solution(file_path::String, dataset::Dataset)
    io = open(file_path, "r")
    lines = split(read(io, String), "\n")
    target_has_sensor = Array{Bool, 1}([true for _=1:dataset.number_targets])
    score = 0
    for line in lines
        target = split(line)
        if length(target) != 2
            continue
        end
        index, has_sensor = parse(Int64, target[1]), parse(Bool, target[2])
        if has_sensor
            score += 1
        end
        target_has_sensor[index] = has_sensor
    end

    return Solution(target_has_sensor, dataset, score=score)
end


function write_solution(file_path::String, solution::Solution)
    open(file_path, "w") do io
        for (index, has_sensor) in enumerate(solution.target_has_sensor)
            if has_sensor
                write(io, "$index 1\n")
            else
                write(io, "$index 0\n")
            end
        end
    end
end
