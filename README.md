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

The description of a harware setup for which PetitPoucet was typically developped for is available under [./example/hardware_description.md](./example/hardware_description.md)

The initial usecase is to geolocate point clouds in forests.
<p align="center"> <img src="https://github.com/DamienGilliard/PetitPoucet/raw/main/assets/img/PetitPoucet_hardware_closup_setup.jpg" width="80%">

## Dependencies
PetitPoucet uses two submodules:
> [RTKLIB](https://github.com/DamienGilliard/RTKLIB)
This is to manage the connections and data transfers from the ntrip caster and the GNSS reciever, as well as reading the raw reciever solutions. It is solely used in the petitpoucet::serverinterface namespace

> [FTXUI](https://github.com/ArthurSonzogni/FTXUI)
This is to provide a user interface and is solely used in the petitpoucet::ui namespace

Additionally, building the code requires CMake 3.12 minimum, because of FTXUI's minimum requirements

## Usage

A usage example is provided in the form of a [main_interface.cc](./src/main_interface.cc) and [side_server_for_RTK.cc](./src/side_server_for_RTK.cc) combo, to create two processes that both communicate with the same hardware serial port.

An explanation of the key steps of those executables is coming soon.
