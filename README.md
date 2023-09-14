# sniffdemo 

sniffdemo is a fundamentally crude iteration of what our underlying sniffer tool will be able to achieve once all the required modules have been integrated.

## Repos considered:

### Modifiable repos:
*These repos contain algorithms that may be considered when developing our sniffer tool*
- [LTESniffer](https://github.com/SysSec-KAIST/LTESniffer) - The signal decoding algorithms used in this repo are pretty efficient, in both the uplink/downlink. The tool itself is a
  passive one, which implies that the IMSI is only captured when there is initial information handover between a UE (user device) and eNB (cell tower) - we want to instead be using active attacks
  which brings us to
- [SigOverinjector](https://github.com/SysSec-KAIST/sigover_injector) - This is essentially the next-gen FBS (fake base station) attack, except it does not disrupt  
### Foundational repos:
*These repos will be required in its full capacity - i.e., there will be very little modification to the code here - this might change with time*
- [FALCON](https://github.com/falkenber9/falcon)
- [srsRAN4G](https://github.com/srsran/srsRAN_4G)

## Current concerns/challenges

- All sub-modules and building blocks to be used for a real-world iteration of sniffdemo have been included in `/external`, but none have as yet been implemented within `main.cpp`.
- The sniffer already knows device configurations, and in fact, doesn't even require SDR connection to 'sniff' the signals. **It is essentially faking the process of sniffing**.
- All executions and function defintions are made within the `main.cpp` file, which is sub-optimal.

### Dependencies

```
sudo apt update
sudo apt-get install autoconf automake build-essential ccache cmake cpufrequtils doxygen ethtool \
g++ git inetutils-tools libboost-all-dev libncurses5 libncurses5-dev libusb-1.0-0 libusb-1.0-0-dev \
libusb-dev python3-dev python3-mako python3-numpy python3-requests python3-scipy python3-setuptools \
python3-ruamel.yaml
```

### building and executing
```
chmod +x configure.sh build.sh run.sh
sudo ./configure.sh
sudo ./build.sh
sudo ./run.sh
```

## Next steps

### Immediate future (~1 month)
- Get all sub-modules integrated within the main program, and be reliably identifying devices and storing information

### Near future (~3 months)
- Integrate camera identification algorithm based on the C++ library [dlib](http://dlib.net/)
- Start training a NeuralNet that will be matching faces with IMSI values from the stored database
- Develop a web application using Flutter that will demonstrate how all of this will work on the user side


