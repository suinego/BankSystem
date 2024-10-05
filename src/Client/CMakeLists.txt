set(CLIENT_SOURCES
    ../Client/Client.cpp
    ../Client/ClientBuilder.cpp
)

add_library(Client ${CLIENT_SOURCES})

target_link_libraries(Client Transaction)
