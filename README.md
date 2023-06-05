# Lyli Backend
## Developer Guide
### Install Dependencies
- cmake
- nlohmann json
- boost

#### Debian
`apt install cmake nlohmann-json3-dev libboost-dev`

#### Pacman
`pacman -S cmake nlohmann-json boost`

#### Portage
`emerge -av dev-util/cmake dev-cpp/asio dev-cpp/nlohmann_json dev-libs/boost`

### Release Build (and run haha)
```sh
./scripts/build-and-run.sh
```
### Debug Build (without run :c)
```sh
mkdir -p build
cmake -S . -B ./build -DDEBUG=1
cmake --build ./build
```

### File Layout
- Header Comment
- Preprocessor
- boost-asio includes
- nlohmann includes
- lyli-backend includes
- system includes
- namespace
- ...
