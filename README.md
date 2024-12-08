# rehoboam

Software running on an LED Cube that requires the [rpi-rgb-led-matrix library](https://github.com/hzeller/rpi-rgb-led-matrix) by Henner Zeller. For setup, the installation script by Adafruit was used. To run the project, all that needs to be done is to make sure you have the rpi-rgb-led-matrix installed and then run `make`.

## Credits
This led-cube code is based on Staack's [there.oughta.be/an/led-cube github](https://github.com/Staacks/there.oughta.be). The housing is derived from the original design, however, I significantly modified it to provide many quality of life improvements, as well as better functionality for maintenance and sturdiness of the panels.

The housing was 3D printed using PLA, 0.4mm nozzle and 0.16mm layer height.

For development:

`cmake -DCMAKE_BUILD_TYPE=Debug /path/to/source`


For production:

`cmake -DCMAKE_BUILD_TYPE=Release /path/to/source`