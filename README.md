# PetitPoucet

![Build](https://github.com/DamienGilliard/PetitPoucet/actions/workflows/c-cpp.yml/badge.svg)


> [!NOTE]  
> This repo is still in active development and its structure is suceptible to deep changes.
> It also stayed too long as private repository and I got a bit lazy in some area, on which I am currently catching up.
> This ``Note`` will be removed as soon as I deem the repo sufficiently "satisfactory", even if under development.
> Cheers XXX

PetitPoucet is a small library that allows to record geolocations using RTK corrections. The library's documentation is available [here](https://damiengilliard.github.io/PetitPoucet/index.html)

A small executable is proposed, with a simple terminal user interface allows some user-friendly basic choices:
<p align="center"> <img src="https://github.com/DamienGilliard/PetitPoucet/raw/main/assets/img/PetitPoucet_terminal_interface.png" width="80%"> 

The initial usecase is to geolocate point clouds in forests.
<p align="center"> <img src="https://github.com/DamienGilliard/PetitPoucet/raw/main/assets/img/PetitPoucet_evaluation_setup.jpg" width="80%">

## Dependencies
we use two submodules:
> [RTKLIB](https://github.com/DamienGilliard/RTKLIB)

> [FTXUI](https://github.com/ArthurSonzogni/FTXUI)

Additionally, building the code requires CMake 3.12 minimum