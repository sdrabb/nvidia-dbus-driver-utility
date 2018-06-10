# nvidia-dbus-driver-utility
simple **D-bus** utility for nvidia driver and some stuff written in C.
Language ```C
         ```



## Description

This is a simple **d-bus** server interface written in C.
It exposes three methods callable using **d-feet**:  


1. **Sum**: given two integer  **a** and **b** it returns the sum **a**  + **b** 
2. **BitChecking**: given a char **a** and an integer **b** check if the **b**-th bit of **a** is high
3. **DlDriver**: it downloads driver for **nvidia-GTX1060** go in systemd **runlevel 2** and install new driver, solve the login loop problem when new drivers are installed automatically


## Requirements

* **gcc**
* **ldbus-1**
* **lcurl**

