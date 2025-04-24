
Programming Using Serial to UDPI and the .HEX binary:

1) Erase the chip (recommended to always do this before flashing firmware)

	pymcuprog erase -d attiny412 -t uart -u /dev/cu.usbserial-0001

2) write a hex file and then verify

	pymcuprog write -d attiny412 -t uart -u /dev/cu.usbserial-0001 -f RampingIOS.hex --verify

3) write fuses, this enables BOD in Active Mode

	pymcuprog write -d attiny412 -t uart -u /dev/cu.usbserial-0001 -m fuses -o 0x01 -l 0x04

4) read back the fuse value

	pymcuprog read -d attiny412 -t uart -u /dev/cu.usbserial-0001 -m fuses -o 0x01