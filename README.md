# Lyli Backend
## Developer Guide
### Install Dependencies
- cmake
- nlohmann json
- asio

#### Pacman
`pacman -S cmake nlohmann-json asio`

### Debug Build
```sh
mkdir build
cd build
cmake .. -DLOG_DEBUG=ON -DLOG_TRACE=ON
cmake --build .
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