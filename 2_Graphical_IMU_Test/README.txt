Arduino Code: Reads sensors, filters data and outputs roll and pitch.
Processing: visual interface with rectangle to show the angles outputed by arduino

- SERIAL AND BLUETOOH VERSION WITH HC-05

--- BLUETOOTH HC-05 SETUP ---
Default baudrate: 9600

0) Enable bluetooth

1) Get HC-05 MAC
hcitool scan

2) Bind serial connection
sudo rfcomm bind 0 <MAC> 1

3)Use CuteCom for serial communication: pick /dev/rfcomm0 and click open

IMPORTANT: THE RX,TX pair of ARDUINO BOARD WILL BE THE OPPOSITE OF THE BLUETOOTH MODULE!


-----------

A REVER: 
- correcção drift (sp,sq) - em embedded fizemos diferente
- calcular tempos de filtragem