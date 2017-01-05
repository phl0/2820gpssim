# 2820gpssim - Fixed GPS Position Simulator for IC-2820

This small circuit was designed to come around the issues with an ICOM IC-2820
installed at a fixed location. The built-in GPS unit is way too slow while
syncing to satellite signals and alspo suffers from periodical drifts in
position. This results in a "hopping" icon on aprs.fi maps.

Instead of using the signals from the internal GPS unit this circuit produces a
DPRS position string every 3 secods. That is fed into the radio using the 2.5mm
data jack on the front of the main unit.

Be sure to have GPS enabled in the settings and disconnected the original GPS
antenna. The GPS mode is set to GPS-A and the DPRS position string from this
simulator is transmitted via the slow data channel.

Currently the string is programmed into the ATtiny2313. For the future I think
of storing it in the EEPROM and programming the device using a terminal.
