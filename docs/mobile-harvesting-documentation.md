# Mobile Harvesting Documentation
Version: 25 August 2023

## Specs:
#### Base computer (for MVP):
- AMD A9 processor or better
- 8 GB DDR4 RAM or better
- Ubuntu 22.04
#### (for prototype):
- Raspberry Pi 4 4GB RAM
#### SDR:
- ~~ADALM-Pluto Rev C/D~~
- Blade RF micro 2.0

## 0. What we want to achieve
We're building a security software that will use an SDR (Blade RF) to sniff LTE networks within the SDR's frequency range and extract relevant user information such as IMSI. 

This software is to be built on top of the existing open-source LTE surveillance tool called [LTESniffer](https://github.com/SysSec-KAIST/LTESniffer).

Some other relevant GitHub repos are:
- [srsLTE-Sniffer](https://github.com/Wooniety/srsLTE-Sniffer)
- [sigover_injector](https://github.com/SysSec-KAIST/sigover_injector)
- [5gsniffer](https://github.com/spritelab/5GSniffer)
- [dlProbe](https://github.com/kotuliak/dlProbe)
- [black widow](https://black-widow.it/)
- [imsi-catcher-sniffer](https://github.com/0xbitx/dedsecimsi)

## 1. Configuring the SDR (Blade RF)


## 1. Configuring the SDR (ADALM-Pluto) (*Discontinued*)
In order to ensure there aren't issues with the Pluto itself, we first test its functionality and update the firmware if required:
- Follow the steps [here](https://wiki.analog.com/university/tools/pluto/common/firmware) to install the latest firmware for the SDR

Assuming that we build off of LTESniffer, we also need to have the following dependencies installed:
- [SoapySDR](https://github.com/pothosware/SoapySDR), and then for the Pluto support, install [SoapyPlutoSDR](https://github.com/pothosware/SoapyPlutoSDR). Check that SoapySDR is reading the Pluto as an RF device with: `SoapySDRUtil --probe`
- [UHD](https://github.com/EttusResearch/uhd/releases) - here's a code snippet to install **its** dependencies:
    ``` 
        sudo apt update

        sudo apt-get install autoconf automake build-essential ccache cmake cpufrequtils doxygen ethtool \
        g++ git inetutils-tools libboost-all-dev libncurses5 libncurses5-dev libusb-1.0-0 libusb-1.0-0-dev \
        libusb-dev python3-dev python3-mako python3-numpy python3-requests python3-scipy python3-setuptools \
        python3-ruamel.yaml
    ```
- [SoapyUHD](https://github.com/pothosware/SoapyUHD) in order to link SoapySDR and UHD together


## 2. The road to MVP:

A major chunk of June was spent figuring out the correct SDR to use as well as looking around for existing open-source software that was reliably doing signal sniffing. 

I also spent some days focusing on building **virtual LTE signals and base stations** so as to comply with regulatory procedures. 

#### __Iteration 0:__

Once I got the green light on tapping into actual signals, I stumbled across a major issue: [LTESniffer]("https://github.com/SysSec-KAIST/LTESniffer") was not working with the Pluto SDR.

Some 2-3 weeks of troubleshooting and getting input from the authors of the code, and I had resolved the issue with the steps mentioned in Section 0. But, now a new issue arose, which was that I was getting invalid PRB values ($>100$) in uplink sniffing, and worse yet, the authors of the code flagged a possible hardware compatibility issue (requirement of supporting 2 clock sources for 2 RX antennas), which I've since been attempting to address in various ways. 

#### __Iteration 1: GNU re-build [*discontinued*]__
The first one was in mid-July, after I had conceded to getting help from freelancers and had already reached out to a few, describing the nature of the job in a succinct manner. 

Understanding the issue of hardware compatibility, **I started building the LTESniffer functionality using GNU Radio from scratch.**

This was one of those things that as I progressed the complexity grew ever-larger, until I had to step back and see whether this was worthwhile. As it would turn out, it was not.


#### __Iteration 2: Pluto software hardware mod and sync w/ zynq_timestamping__

The current unstable approach is to reconfigure the pluto sdr itself. I opened up the board to inspect it and discovered that there are **one each of Rx and Tx ports, which implies we will be able to have two Rx antennas and two Tx antennas in total**. Additionally, there is a clock_input and clock_output port as well, which means the board has the capacity for an external clock to be used. 

It's important to note why we would even need these configurations. Given our need to get accurate capturing of user information through signal interception, we need to employ a dual-receive and dual-transmit system to increase the accuracy and strength of the signal received.

#### Iteration 3: BladeRF combined with a GPSDO


*Why would researchers use these insanely expensive SDRs for their experiments if they want to express the practicality of their software?*
- with a high-grade SDR comes perks such as reliability, accessibility and plug-n-play options.
- politics within the academic field can sometimes hinder creativity
- a higher budget and ease of access to these devices will speed up their research process, thus they pick this route.
- they're more concerned about completing their paper than in considering the practicality of commercialising their product

## 3. Future iterations and challenges

- There's a **two-pronged** advantage to building the development board for our SDR from scratch.
    1. A potential way to bring the overall costs down significantly. 
    2. Given the open-source nature of the Pluto, we are free to simply copy the design, purchase the individual parts (op-amps, zynq chips, etc.) separately, and then solder it together. This would then be a fully **proprietary** product.

- I've also come across the [LTProbe]() github repo [dlProbe](https://github.com/kotuliak/dlProbe), which could be of interest as there's also the option of reaching out to the author himself and get his input.


## 4. Literature
**What does a mobile network look like?**
<img title="a title" alt="Alt text" src="F:\Notion SS\safesphere">


## 5. Links
1. [Hiding in Plain Signal: Physical Signal Overshadowing Attack on LTE](https://syssec.kaist.ac.kr/pub/2019/sec19-yang-hojoon.pdf)

2. [LTESniffer: An Open-source LTE Downlink/Uplink Eavesdropper](https://syssec.kaist.ac.kr/pub/2023/wisec2023_tuan.pdf)

3. [LTrack: Stealthy Tracking of Mobile Phones in LTE](https://www.usenix.org/conference/usenixsecurity22/presentation/kotuliak)

4. [Easy 4G/LTE IMSI Catchers for Non-Programmers](https://arxiv.org/pdf/1702.04434.pdf)

5. [LTE security, protocol exploits and location tracking
experimentation with low-cost software radio](https://arxiv.org/pdf/1607.05171.pdf)


## 6. Archive
#### GNU Radio: 
GNU Radio is an open-source software development toolkit for SDR. It provides a graphical interface and a set of signal processing blocks that can be interconnected to create radio systems. Using GNU Radio, you can build LTE receivers, perform synchronization, decode LTE control channels, analyze LTE signals, and develop custom processing blocks.

### Steps to build LTE sniffing tool with GNU Radio:
Since our primary focus is IMSI collecting for now, I want to focus on building that functionality in GNU Radio. Let's look at what the literature says for LTESniffer:

*IMSI is leaked in plaintext through two procedures; 1) paging
and 2) the initial network connection process. LTESNIFFER collects
the IMSI exposed in the air by **monitoring uplink Attach Request,
Identity Response, and downlink Paging messages***

Our current configuration contains the following:
1. **Pluto SDR Source Block**:
    - Specification: Sample rate of 70k and LO frequency of 900MHz.
    - Purpose: Provides the capability to capture the uplink signal emitted by the test phone in the specified frequency band. The sample rate and LO frequency are set according to the requirements of the test scenario.
2. **Lowpass Filter Block**:
    - Specification: Performs lowpass filtering on the input signal to remove high-frequency components.
    - Purpose: The Lowpass Filter Block plays a crucial role in signal conditioning by removing high-frequency noise and unwanted spectral content. Let's explore its functionality and design choices:
        - The block is implemented using the `filter.fir_filter_ccf` class in GNU Radio.
        - The filter taps for the lowpass filter are generated using the `firdes.low_pass` function. The design parameters for the lowpass filter include the filter order, cutoff frequency, transition width, window type, and beta value.
        - The cutoff frequency and transition width are design choices based on the specific requirements of the LTE signal. The cutoff frequency determines the maximum frequency allowed in the filtered signal, and the transition width defines the range of frequencies over which the filter transitions from passband to stopband. These values are typically chosen to satisfy the system's bandwidth requirements and signal characteristics.
        - The window type and beta value affect the shape of the filter's frequency response, providing control over factors such as sidelobe levels and filter roll-off. The choice of window type and beta value depends on the specific application and design trade-offs.
        - The filtered output signal from the Lowpass Filter Block serves as the input for further processing, such as quadrature demodulation.
3. **Quadrature Demodulation Block**:
    - Specification: Performs quadrature demodulation on the input signal to extract the baseband information.
    - Purpose: Performs quadrature demodulation on the filtered signal, extracting the baseband representation of the modulated LTE signal. This step converts the signal from its RF representation to the complex baseband representation.
        - The block is implemented using the `analog.quadrature_demod_cf` class in GNU Radio.
        - Quadrature demodulation is a key step in extracting the modulated baseband signal from the filtered input signal. It involves multiplying the input signal with two synchronized carriers, shifted by 90 degrees in phase. This process creates the I (in-phase) and Q (quadrature) components of the baseband signal.
        - The quadrature demodulator block takes the filtered input signal as its input and produces the demodulated baseband signal as its output.
        - The choice of quadrature demodulation is based on the modulation scheme used in LTE, which typically employs quadrature amplitude modulation (QAM) or quadrature phase shift keying (QPSK) modulation. Quadrature demodulation is effective in recovering the baseband signal from these modulation schemes.
4. **Correlation Block**:
    - Specification: Takes in the received signal and PSS sequence as inputs, and outputs the cross-correlation result.
    - Purpose: The Correlation Block plays a crucial role in synchronizing with the LTE signal and identifying the presence and timing of the PSS (Primary Synchronization Signal). Let's explore its functionality and design choices in more detail:
        - The block is implemented as a `gr.sync_block` subclass in GNU Radio.
        - The `__init__` method initializes the block and sets its name, input and output signal types.
        - The `work` method is where the actual processing takes place. It receives the received signal and the PSS sequence as input items and outputs the cross-correlation result.
        - The `perform_correlation` method performs the cross-correlation operation between the received signal and the PSS sequence. It iterates through the received signal, sliding the PSS sequence over it and computing the correlation value at each position.
        - The cross-correlation result is stored in the `cross_correlation_result` list and eventually assigned to the output items.
        - The block uses the `np.complex64` data type for the received signal and PSS sequence, and `np.float32` for the cross-correlation result, as specified in the `in_sig` and `out_sig` arguments of the `gr.sync_block` initialization.
        - The block outputs the cross-correlation result, which represents the correlation between the received signal and the PSS sequence. Peaks in the cross-correlation result indicate the timing offset and presence of the PSS in the received signal.
    
    The design choice of using cross-correlation is based on the fact that the PSS sequence is known and can be used as a reference to identify the presence and timing offset in the received signal. By sliding the PSS sequence over the received signal and computing the correlation at each position, we can identify the peaks that correspond to the PSS. The location of these peaks provides an estimation of the timing offset, which is crucial for synchronization.
    
5. **PCI Decoder Block**:
    - Specification: Takes in the PCI value and generates the corresponding PSS sequence.
    - Purpose: The PCI Decoder Block is responsible for generating the PSS sequence specific to the given PCI value. Let's explore its functionality and design choices:
        - The block is implemented as a `gr.sync_block` subclass in GNU Radio.
        - The `__init__` method initializes the block and sets its name, input and output signal types.
        - The `work` method receives input items (which are not used in this case) and outputs the generated PSS sequence.
        - The `generate_pss_sequence` method generates the PSS sequence based on the provided PCI value. It iterates through the range of indices and calculates the complex values according to the given formula.
        - The generated PSS sequence is stored in the `pss_sequence` list and assigned to the output items.
        - The block uses the `np.complex64` data type for the output signal, as specified in the `out_sig` argument of the `gr.sync_block` initialization.
    
    The design choice of generating the PSS sequence based on the PCI value is crucial for synchronization. Each PCI has a unique PSS sequence associated with it, and by generating the correct sequence, we can effectively correlate it with the received signal and identify the specific cell transmitting the LTE signals.
    
    By combining the functionality of the Correlation Block and the PCI Decoder Block, we can synchronize with the LTE signal, detect the presence and timing of the PSS, and determine the transmitting cell based on the PCI value.
    
    These blocks, along with the other blocks in the flowgraph, work together to achieve the goal of the PoC by extracting the IMSI and CellID from the received LTE signals.

## 7. Some interesting asides and helpful notes:
- [How to open md file preview on vscode?](https://www.jasongaylord.com/blog/2021/09/27/preview-markdown-visual-studio-code)
