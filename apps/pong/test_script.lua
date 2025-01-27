 
animation = {
    type = "animation",
    source_file = "../resources/pong.png",
    pivot = {7, 7},
    rotation = 0.0,
    frames = {
        {
            rect = {56, 34, 16, 16},
            pivot = {0, 0},
            scale = 1.0,
            rotation = 0.0
        },
        {
            rect = {72, 34, 16, 16},
            pivot = {0, 0},
            scale = 1.0,
            rotation = 0.0
        }
    }
}

SelectImage("../resources/pong.png")
PONG_MOVE_ANIM = CreateAnimation({pivot={7, 7}})
for x = 0, 8
    pongAnim:addFrame({rect={56 + x * 16, 34, 16, 16}})
end

ballSprite = CreateSprite({key="BALL",
                           file="../resources/pong.ball",
                           rect={128, 128, 32, 32},
                           rotation=45,
                           pivot="center"})
ballSprite
