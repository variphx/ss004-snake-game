all: build run

build:
        clang++ ./src/main.cc \
                ./src/coordination.cc \
                ./src/game.cc \
                ./src/snake.cc \
                -lncurses \
                -o ./target/snake_game

run:
        ./target/snake_game⏎
