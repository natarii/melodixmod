A dedicated flash IC is used to implement chip select and bank switching logic.

A pre-built flash image is included, but a new one can be built using the PHPPLD project. This image is generated for up to 4 megabit flash, but can be written (with the higher unused area ignored) to 1 and 2 megabit parts.

`php <path to PHPPLD> -i 19 -w logic.bin logic.php`
