# MusiCrap - MVJam IV 


## - DEPENDENCIES
- SFML v2.5 (Not Included)
- ZPG (Not Included)
- FMOD (Included)

# COMPILATION PROCESS (LINUX)
#### Install SFML v2.5.1 (Static Mode)
```
sudo apt-get install cmake zlib1g-dev libfreetype6-dev libx11-dev libxrandr-dev libxcb1-dev libx11-xcb-dev libxcb-randr0-dev libxcb-image0-dev libflac-dev libogg-dev libvorbis-dev libvorbisenc2 libvorbisfile3 libopenal-dev libudev-dev libsndfile1-dev libglew-dev
wget -O ~/sfmlv2.5.1.tar.gz https://github.com/SFML/SFML/archive/2.5.1.tar.gz
tar -zxvf ~/sfmlv2.5.1.tar.gz -C ~/
cd ~/SFML-2.5.1
cmake -DBUILD_SHARED_LIBS:BOOL=FALSE -DCMAKE_INSTALL_PREFIX:PATH=/usr .
make
sudo make install
```
#### Install ZPG
```
wget -O ~/zpgv1.0.0.zip https://gitlab.com/Tardo/Zpg/-/archive/master/Zpg-master.zip
unzip ~/zpgv1.0.0.zip -d ~/
cd ~/Zpg-master
cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr .
make Zpg
make zpg_packer
sudo make install
```
### Compile Game
```
git clone https://github.com/Tardo/MVRunner.git --depth=1
cd MVRunner/
mkdir build-release/
cd build-release/
cmake -DCMAKE_BUILD_TYPE:STRING=Release ..
make
```
-  ### Create ZPG Assets Package
  ```
  make create_zpg
  ```
-  ### Create Distribution Packages (Optional)
  ```
  cpack --debug --verbose -C CPackConfig.cmake
  ```
  
# COMPILATION PROCESS (WINDOWS)
#### Install SFML v2.5.1 (Static Mode)
```
curl -LfsS -o sfml.zip https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip
7z x sfml.zip
md C:\Program Files\SFML-2.5.1
move SFML-2.5.1 C:\Program Files\

```

#### Install ZLib
```
curl -LfsS -o zlib-1.2.11.tar.gz http://zlib.net/zlib-1.2.11.tar.gz
7z x zlib-1.2.11.tar.gz
7z x zlib-1.2.11.tar
cd zlib-1.2.11
md build & cd build
cmake -Werror=dev -G"Visual Studio 15 2017 Win64" ..
cd ..
cmake --build build --config Release --target install
```

#### Install ZPG
```
curl -LfsS -o libzpg.zip https://gitlab.com/Tardo/Zpg/-/archive/master/Zpg-master.zip
7z x libzpg.zip
cd Zpg-master
md build & cd build
cmake -Werror=dev -G"Visual Studio 15 2017 Win64" -DZLIB_ROOT="C:\Program Files\zlib" -DCMAKE_BUILD_TYPE=Release ..
cd ..
cmake --build build --config Release --target Zpg
cmake --build build --config Release --target zpg_packer
cmake --build build --config Release --target install
cd ..
```
### Compile Game
```
git clone https://github.com/Tardo/musicrap.git --depth=1
cd musicrap/
mkdir build-release/
cd build-release/
md build & cd build
set ZLIB_ROOT=C:\Program Files\zlib
set ZPG_ROOT=C:\Program Files\libZpg
set SFML_ROOT=c:\projects\SFML-2.5.1_win64
cmake -Werror=dev -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Release -DZLIB_ROOT="%ZLIB_ROOT%" -DZPG_PACKER_BIN="%ZPG_ROOT%\bin\zpg_packer.exe" ..
cmake --build build --config Release --target MusiCrap
```
-  ### Create ZPG Assets Package
  ```
  cmake --build build --config Release --target create_zpg
  ```
-  ### Create Distribution Packages (Optional)
  ```
  cmake --build build --config Release --target package
  ```
