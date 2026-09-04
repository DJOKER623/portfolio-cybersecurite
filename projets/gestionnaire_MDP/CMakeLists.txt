cmake_minimum_required(VERSION 3.15)
project(PasswordManager C)

set(CMAKE_CXX_STANDARD 17)

# Trouver les bibliothèques installées
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)
# pkg_check_modules(SQLCIPHER REQUIRED sqlcipher)

add_executable(password_manager main.c)

# Lier les bibliothèques à ton exécutable
target_include_directories(password_manager PRIVATE ${SODIUM_INCLUDE_DIRS})
target_link_libraries(password_manager PRIVATE ${SODIUM_LIBRARIES})