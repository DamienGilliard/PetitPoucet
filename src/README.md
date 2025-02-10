# PetitPoucet

PetitPoucet is a small library that allows to record geolocations using RTK corrections. The library's documentation is available [here](https://damiengilliard.github.io/PetitPoucet/index.html)

A small executable is proposed, with a simple terminal user interface allows some user-friendly basic choices:
<p align="center"> <img src="https://github.com/DamienGilliard/PetitPoucet/raw/main/assets/img/PetitPoucet_terminal_interface.png"> 

The initial usecase is to geolocate point clouds in forests.
<p align="center"> <img src="https://github.com/DamienGilliard/PetitPoucet/raw/main/assets/img/PetitPoucet_evaluation_setup.jpg">

## Dependencies
we use two submodules:
> [RTKLIB](https://github.com/DamienGilliard/RTKLIB)

> [FTXUI](https://github.com/ArthurSonzogni/FTXUI)

Additionally, building the code requires CMake 3.12 minimum