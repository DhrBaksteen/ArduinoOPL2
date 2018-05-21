#!/bin/sh

if ! type "gpio" > /dev/null
then
	echo "WiringPi was not found!"
	echo "Please run the build script to install your OPL2 Audio Board first."
	echo ""
	exit
fi

PIN_LATCH=$(gpio readall | awk -F\| '{if ($3==1) print $7} {if ($13==1) print $9}')
PIN_A0=$(gpio readall | awk -F\| '{if ($3==4) print $7} {if ($13==4) print $9}')
PIN_RESET=$(gpio readall | awk -F\| '{if ($3==5) print $7} {if ($13==5) print $9}')
PIN_DATA=$(gpio readall | awk -F\| '{gsub(/ /, "", $4) ; if ($4=="MOSI") print $7} {gsub(/ /, "", $12) ; if ($12=="MOSI") print $9}')
PIN_SHIFT=$(gpio readall | awk -F\| '{gsub(/ /, "", $4) ; if ($4=="SCLK") print $7} {gsub(/ /, "", $12) ; if ($12=="SCLK") print $9}')

echo ""
echo "Connect the OPL2 Audio Board to your Raspberry Pi like this:"
echo ""
echo "+------------+------+"
echo "| OPL2 Board | GPIO |\tGPIO header orientation:"
echo "+------------+------+"
echo "|      Reset | $PIN_RESET |\t                     |"
echo "|      Shift | $PIN_SHIFT |\t        ... 5 3 1    |"
echo "|      Latch | $PIN_LATCH |\t       -----------+  |"
echo "|       Data | $PIN_DATA |\t        o o o o o |  |"
echo "|         A0 | $PIN_A0 |\t      o o o o o o |  |"
echo "|        GND |  6   |\t     -------------+  |"
echo "|        +5v |  2   |\t        ... 6 4 2    |  <-- Raspberry Pi"
echo "+------------+------+\t  ------------------'       board edge"
echo ""
