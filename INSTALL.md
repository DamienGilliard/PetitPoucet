# Install

This library relies on 1 core assumption:
- You have a RTK reciever connected via serial to your computer. Depending on your computer and OS, the serial port name can vary. On my computer (jetson nano, ubuntu 18.04) it is at /dev/ttyACM0

It also relies on a secondary assumption:
- you are located in a geographical location where correction data is available through an online caster. In my case, I get the correction data at http://caster.centipede.fr:2101/LXLY

## build the library and the executable on ubuntu
To build PetitPoucet on your ubuntu computer and use its executable, proceed as follows:

- clone the repository if it is not already done
- create a build folder in the src folder:
```bash
cd PetitPoucet/src
mkdir build && cd build
```
- configure the build using CMake:
```bash
cmake ..
```

- build the executables and link the library:
```bash
make
```

## link the library only to your project

coming as soon as it is solved

## configuration file

To easily pass the caster and serial port, PetitPoucet allows to read a [configuration file](https://damiengilliard.github.io/PetitPoucet/classpetitpoucet_1_1serverinterface_1_1ConfigurationSetup.html)
The small executable included looks for a file called config.txt located in the root directory of PetitPoucet. The content is structured as follows:
```txt
caster: <your_chosen_caster>
serial: <the_serial_port_where_the_reciever_is_connected:baudrate:data_bits:parity_bits:n_stop_bits:flow_control>
```
For example:
```txt
caster: caster.centipede.fr:2101/LXLY
serial: ttyACM0:115200:8:n:1:off
```