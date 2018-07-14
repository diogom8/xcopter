Arduino Code: Reads sensors, filters data and outputs roll and pitch.
Processing: visual interface with rectangle to show the angles outputed by arduino

- SERIAL AND BLUETOOH VERSION WITH HC-05

--- BLUETOOTH HC-05 SETUP ---
Default baudrate: 9600

1) Get HC-05 MAC
hcitool scan

2) Bind serial connection
sudo rfcomm bind 0 <MAC> 1


-----------

A REVER: 
- correcção drift (sp,sq) - em embedded fizemos diferente
- calcular tempos de filtragem