struct Position
    row::Int64
    column::Int64
end

struct Target
    index::Int64
    position::Position
end

struct Sensor
    target_index::Int64
end

struct Grid
    width::Int64
    height::Int64
    targets::Array{Target, 1}
    sensors::Array{Sensor, 1}
    sink::Position
end

function distance(position1::Position, position2::Position)
    return
end
