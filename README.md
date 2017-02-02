# IR to MySql RaspPi

I've a Aquacell (https://www.aquacell-waterontharder.nl/) installed in a cabinet and want to log the level of the salt-blocks. When it's below a critical level it should send a warning to place new salt-blocks into the Aquacell. It should also send an e-mail when the stock of blocks gets empty to place a new order at https://www.aquacombi-webshop.nl/producten/regeneratiezout .

I use two IR Sharp GP2Y0A21YK sensors (http://bit.ly/2k2vzdY) above the salt-containers. They are connected to an Arduino Uno with an ethernetshield. The measured data is send (GET) to an php file and put it into a mySql database.

https://github.com/florushoogendoorn/IR-to-MySql-RaspPi/Untitled Sketch_bb.png


