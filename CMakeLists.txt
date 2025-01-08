cmake_minimum_required(VERSION 3.28.3)

project(chat-app)

add_executable(server server/server.cpp)
add_executable(client client/client.cpp)
