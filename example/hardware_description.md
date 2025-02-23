# Hardware description

PetitPoucet was developped to be used onboard a mobile RTK device, to offer a flexible RTK tool for forest remote sensing and mapping.

Indeed, consumer devices can be unreliable and lack precision under forest canopy. In our experience gps position can be sometimes off by tens of meters, making the coordinates only useful for rough localisation of the work area, but is unusable for more precise purposes.

While the sethup we present is not consistently reaching the "centimeter-level" accuracy RTK is advertised to be reachable, we do get positions that are consistently "sub-meter" accurate. The precision is also relatively good with standard deviation of the position over 2-4 min usually in the 2-6 10^-6 degrees. A more in-depth report analysis of the performance of the system presented here will produced in the coming weeks.

<p align="center"> <img src="https://github.com/DamienGilliard/PetitPoucet/raw/main/assets/img/PetitPoucet_hardware_closup_setup.jpg" width="80%"> 

## Components

The components of the system are as follows:
- Nvidia Jetson Nano single board computer
- Ardusimple RTK reciever with ublox ZED-F9P module
- Ardusimple RTK NOAA calibrated dual band antenna
- 7 inch  WaveForm touch display
- repurposed aluminium casing
- 48 Wh 11.1V LiPo battery
- Teltonika TRB140 4G LTE router
- buck converter for 12V to 5V (to power the Jetson Nano)