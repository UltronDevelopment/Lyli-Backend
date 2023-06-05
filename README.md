# Lyli Backend
<sup>Developer Guide</sup>

## 1️⃣ Install Dependencies
- cmake
- nlohmann json
- boost

### Apt
```sh
apt install cmake nlohmann-json3-dev libboost-dev
```

### Pacman
```sh
pacman -S cmake nlohmann-json boost
```

### Portage
```sh
emerge -av dev-util/cmake dev-cpp/asio dev-cpp/nlohmann_json dev-libs/boost
```

## 2️⃣ Build & run

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

# 🤝 Contributing
You can view the contribution guides [here](contributing.md)
