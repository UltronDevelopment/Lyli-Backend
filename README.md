# Lyli Backend
<sup>Developer Guide</sup>

## 1️⃣ Install Dependencies
- cmake
- nlohmann_json
- boost
- mongoc
- bson
- openssl

### Apt
```sh
apt install cmake nlohmann-json3-dev libboost-dev libbson-dev libmongoc-dev libssl-dev
```

### mongoc linking hack (only required on ubuntu 20.04)
```sh
sed -i 's/foreach\(.*\)_IMPORT_CHECK_TARGETS\(.*\)/foreach\1_IMPORT_CHECK_TARGETS\2 \n break()/g' /usr/lib/x86_64-linux-gnu/cmake/mongoc-1.0/mongoc-targets.cmake
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

<sub>[Here is a link to the frontend repository](https://github.com/UltronDevelopment/Lyli)</sub>
