# vip-access
Symantec VIP Access security code generator for Samsung Gear Fit 2 (Pro)

<img src="https://github.com/wolandmaster/vip-access/raw/master/images/screenshot-1.jpg" width="108" height="216"/> <img src="https://github.com/wolandmaster/vip-access/raw/master/images/screenshot-2.jpg" width="108" height="216"/> <img src="https://github.com/wolandmaster/vip-access/raw/master/images/screenshot-3.jpg" width="108" height="216"/>

## Build & Run
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
  ~/tizen-studio/tools/ide/bin/tizen package -t tpk -s <security profile> -- Release  
  ```
* install
  ```
  ~/tizen-studio/tools/sdb install com.github.wolandmaster.vipaccess-1.0.0-arm.tpk
  ```
* run
  ```
  ~/tizen-studio/tools/ide/bin/tizen run -p com.github.wolandmaster.vipaccess -t SM-R365
  ```
  
## Obtain Symantec VIP secret
* install [python-vipaccess](https://github.com/dlenski/python-vipaccess)
  ```
  pip3 install python-vipaccess
  ```
* provision a new VIP Access credential
  ```
  vipaccess provision --token-model SYMC --print
  ```
* register the credential on your company's corresponding site with the generated ID (SYMCxxxxxxxx)
* enter the secret key to the settings page of the app
