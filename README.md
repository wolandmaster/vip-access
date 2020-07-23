# vip-access
Symantec VIP Access security code generator for Samsung Gear Fit 2 (Pro)

<img src="https://github.com/wolandmaster/vip-access/raw/master/images/screenshot-1.jpg" width="108" height="216"/> <img src="https://github.com/wolandmaster/vip-access/raw/master/images/screenshot-2.jpg" width="108" height="216"/> <img src="https://github.com/wolandmaster/vip-access/raw/master/images/screenshot-3.jpg" width="108" height="216"/>

## Install instructions

### Download & Install Tizen Studio with CLI installer
  * https://developer.tizen.org/ko/development/tizen-studio/download

### Connect your device 
  * on your Gear Fit 2 (Pro):
    * Settings -> Gear info -> Debugging: turn on
    * Settings -> Connections -> Bluetooth: turn off
    * Settings -> Connections -> Wi-Fi: turn on -> connect to the same network as your computer
    * Note down the IP address: Settings -> Connections -> Wi-Fi -> *Access Point Name* -> IP address
  * on you computer:
    ```
    ~/tizen-studio/tools/sdb connect <ip-address>
    ~/tizen-studio/tools/sdb devices
    ```
### Get Samsung certificates
  * Package Manager
    ```
    ~/tizen-studio/package-manager/package-manager.bin
    ```
    * Install Main SDK -> Tizen SDK tools -> Baseline SDK -> Certificate Manager
    * Install Extension SDK -> Extras -> Samsung Certificate Extension

    * (optionally: only for building)    
      Install Main SDK -> 2.3.1 Wearable -> Advanced -> Native app. development (CLI)
      
  * Certificate Manager:
    ```
    ~/tizen-studio/tools/certificate-manager/certificate-manager
    ```
    * Create certificate profile by pressing the "+" sign -> select SAMSUNG as type
    * Device type -> Mobile/Wearable
    * Create a new certificate profile -> type any name you want
    * Create a new author certificate
    * Sign in with you Samsung account or create a new one
    * Create a new distributor certificate -> Privilige: Partner
    * DUIDs: first one filled with your device (2.0#...), if not, connect your device: see above
    
## Sign & Install
  * Download the latest release tpk file of the app: https://github.com/wolandmaster/vip-access/releases
  * Sign with your certificate
    ```
    ~/tizen-studio/tools/ide/bin/tizen package -t tpk -s <certificate profile> -- com.github.wolandmaster.vipaccess-1.0.0-arm.tpk
    ```
  * Install
    ```
    ~/tizen-studio/tools/sdb install com.github.wolandmaster.vipaccess-1.0.0-arm.tpk
    ```

## Obtain Symantec VIP secret
* Install [python-vipaccess](https://github.com/dlenski/python-vipaccess)
  ```
  pip3 install python-vipaccess
  ```
* Provision a new VIP Access credential
  ```
  vipaccess provision --token-model SYMC --print
  ```
* Register the credential on your company's corresponding site with the generated ID (SYMCxxxxxxxx)
* Enter the secret key to the settings page of the app

## Build & Run (optionally)
* change to cloned source directory
* clean
  ```
  ~/tizen-studio/tools/ide/bin/tizen clean
  ```
* build
  ```
  ~/tizen-studio/tools/ide/bin/tizen build-native -a arm -c llvm -C Release
  ```
* package
  ```
  ~/tizen-studio/tools/ide/bin/tizen package -t tpk -s <certificate profile> -- Release  
  ```
* install
  ```
  ~/tizen-studio/tools/sdb install com.github.wolandmaster.vipaccess-1.0.0-arm.tpk
  ```
* run
  ```
  ~/tizen-studio/tools/ide/bin/tizen run -p com.github.wolandmaster.vipaccess -t SM-R365
  ```
