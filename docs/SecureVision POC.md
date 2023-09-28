# SecureVision POC and MVP Documentation
Version: 27 September 2023

## Specs:
#### Base computer (for MVP):
- AMD A9 processor or better
- 8 GB DDR4 RAM or better
- Ubuntu 22.04
#### (for prototype):
- Raspberry Pi 4 4GB RAM
#### SDR:
- Blade RF micro 2.0
#### GPSDO:
- Leo Bodnar Mini GPSDO

## 0. Iterations 
## Iter 0: POC
- Essentially do what [sniffdemo](https://github.com/apex-jpg/activsniff) does but: 
    - comb through the [SigOver](https://github.com/SysSec-KAIST/sigover_injector) and [LTESniffer](https://github.com/SysSec-KAIST/LTESniffer) codebase to understand how they utilise [srsRAN](https://github.com/srsran/srsRAN_4G) and [FALCON](https://github.com/falkenber9/falcon) to connect with and receive signals that are received by the SDR (bladeRF).
    - implement mechanism described in *Connecting bladeRF with GPSDO*.
    - use [SigOver](https://github.com/SysSec-KAIST/sigover_injector) signal overshadowing attack algorithm to transmit a signal (more on this in below section) overlapping a portion of the signal transmitted by a cell tower that will request the mobile device to share identification information (IMSI) \- this mechanism is also called *paging request*.
    - use [LTESniffer](https://github.com/SysSec-KAIST/LTESniffer) framework (*see 1.5*) to do the signal capturing and processing. 

## Iter 1: MVP
- Expand on POC codebase, such that now we can capture and process multiple user devices at the same time and store the captured information using [WireShark](https://www.wireshark.org/).


## 1. Literature

### 1.1 LTE Framework (simplified)

Broadcast messages (messages from cell towers - an example being System Information Blocks(SIBs) that provide network details and updates to a phone) in the LTE framework are not encrypted or integrity protected. This is because they are meant to be received by all devices within a cell.

#### 1.1.1 Abbreviations and brief definitions

### 1.2 Configuring bladeRF
1. Clone the repo using: `git clone https://github.com/Nuand/bladeRF.git`
2. Get this pre-built bladeRF [FPGA package](https://www.nuand.com/fpga/hostedxA4-latest.rbf)
3. Get this pre-build [firmware image](https://www.nuand.com/fx3/bladeRF_fw_latest.img)
4. Follow the rest of the instructions on [Quick Start](https://github.com/Nuand/bladeRF)


### 1.3 Connecting bladeRF with GPSDO
#### *Why use GPSDO?*
We are capturing signals from multiple devices at different locations simultaneously, and a GPSDO will ensure all these devices are synchronised with a common and accurate time reference. This is crucial for coordinated data capture and analysis. 

#### *How?*
Look into the *bladerf2-micro* subsection of [Using an External Clock Input](https://github.com/Nuand/bladeRF/wiki/bladeRF-CLI-Tips-and-Tricks#user-content-Using_an_External_Clock_Input), and follow the steps given there.


### 1.5 LTESniffer codebase

#### Design
LTESniffer has a pretty solid design framework with 4 parts:
1. signal processor: captures analog signals and converts them into digital samples
Not much modifications required in this block.

2. synchroniser: **NEEDS MODIFICATION** 

3. configuration extractor: 

Before we get into the actual decoding process, we need to get the specific configurations (modulation scheme and radio configuration) allocated to each device in a given area. This consists of three sub-blocks to gain this information:
- PDCCH decoder
    - Role: Decodes DCI (Downward Control Information) messages and identifies RNTIs (Radio Network Temporary Identities)
    - DCI, RNTI: DCI contains details on modulation schemes, resource allocation, and more. RNTIs are unique identifiers assigned to UEs in LTE networks for identification within the network.
    - Function: Once decoded, it will have important information on location of user data within the PDSCH (Physical Downlink Shared Channel), the MCS (Modulation and Coding Scheme) index required for decoding the data channel, and the packet size of downlink data. We need this information to correctly process the data channel.
- MCS table verifier
    - Role: The MCS (Modulation and Coding Scheme) table verifier validates and manages the MCS table used by each UE (User Equipment) for uplink and downlink signals.
    - Function: If LTESniffer does not possess the correct MCS table for a specific UE, it utilises the MCS table verifier to verify and store the configuration of that UE's MCS table. This ensures that the appropriate modulation and coding schemes are applied during data decoding.
- Radio connection parser
    - Role: The radio connection parser extracts essential radio configurations during the initial radio connection setup between a user device (UE) and a cell tower (eNB)
    - Function: By monitoring the radio connection setup messages, the radio connection parser gathers crucial configurations used for processing data channels. This includes information necessary for both uplink and downlink data channels. These configurations are essential for accurately decoding and handling data traffic between the UE and the network.

4. data channel decoder: **NEEDS MODIFICATION**
