<h1 align="center" style="color:white; background-color:black">[Zoio]</h1>
<h4 align="center">[Image transmission using FSK and SX1276 module.]</h4>

<p align="center">
	<a href="http://zenith.eesc.usp.br/">
    <img src="https://img.shields.io/badge/Zenith-Embarcados-black?style=for-the-badge"/>
    </a>
    <a href="https://eesc.usp.br/">
    <img src="https://img.shields.io/badge/Linked%20to-EESC--USP-black?style=for-the-badge"/>
    </a>
    <a href="https://github.com/${{ env.REPOSITORY_FULL_NAME }}/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/${{ env.REPOSITORY_FULL_NAME }}?style=for-the-badge"/>
    </a>
    <a href="https://github.com/${{ env.REPOSITORY_FULL_NAME }}/issues">
    <img src="https://img.shields.io/github/issues/${{ env.REPOSITORY_FULL_NAME }}?style=for-the-badge"/>
    </a>
    <a href="https://github.com/${{ env.REPOSITORY_FULL_NAME }}/commits/main">
    <img src="https://img.shields.io/github/commit-activity/m/${{ env.REPOSITORY_FULL_NAME }}?style=for-the-badge">
    </a>
    <a href="https://github.com/${{ env.REPOSITORY_FULL_NAME }}/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/${{ env.REPOSITORY_FULL_NAME }}?style=for-the-badge"/>
    </a>
    <a href="https://github.com/${{ env.REPOSITORY_FULL_NAME }}/commits/main">
    <img src="https://img.shields.io/github/last-commit/${{ env.REPOSITORY_FULL_NAME }}?style=for-the-badge"/>
    </a>
    <a href="https://github.com/${{ env.REPOSITORY_FULL_NAME }}/issues">
    <img src="https://img.shields.io/github/issues-raw/${{ env.REPOSITORY_FULL_NAME }}?style=for-the-badge" />
    </a>
    <a href="https://github.com/${{ env.REPOSITORY_FULL_NAME }}/pulls">
    <img src = "https://img.shields.io/github/issues-pr-raw/${{ env.REPOSITORY_FULL_NAME }}?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#environment-and-tools">Environment and Tools</a> •
    <a href="#steps-to-run-and-debug">Steps to run and debug</a> •
    <a href="#how-to-contribute">How to contribute?</a> •
</p>


## What am I looking at?

That is exactly the purpose of this project, to show us what our stratospheric sensors see during flight.
Jokes aside, this project was born to experiment with and learn about real-time image transmission processes.

It uses an ESP32 CAM board as a base, connected to a handmade breakout board that contains an SX1276 radio and an indicator LED. Everything was powered using an 18650 Li-ion battery with a 5V power converter from AliExpress. Pretty simple yet good enough.

## Environment and tools

All done with the Arduino IDE v2.0.

The code for the radio and FSK functions was also made by us because, at the time, no FSK-able libraries existed. It can be found [HERE](https://github.com/JulioCalandrin/SX127X-FSK-LoRa). The only changes made were altering the pin write and SPI functions to the Arduino ones.


## The transmitter:

The transmitter code is pretty straight-forward and a little caotic because it was made in a hurry, as was everything else in this project. It begins by taking a picture with the standard Arduino ESP32 CAM functions. It returns a vector that contains the byte stream that, when saved with the .jpg extension, becomes an image.

All we did was send all of those bytes in 255-byte packets, without any encoding, using FSK. The first packet acts as a header, containing how many packets are going to be transmitted. The following packets contain the actual byte stream that forms the photo. Yes, this is a bad idea, since lost packets break the jpg, but transmitting the entire raw file would be impractical.

## The receiver

On the receiver side of things, a TTGO board was used for simplicity, simply receiving the packets, extracting the byte stream, and dumping it into the serial port once all packets were received in order to be processed by another piece of software that collects, saves, and displays the photos. Its repository is HERE. (add this later). If at least half of the total packets were successfully received, the receiver dumps the data anyway, in case the jpg is partially usable.

## Benchmark

The transmission and reception of a 400x296-pixel image takes about two seconds at 38.4 kbps. The SX1276 FSK can go faster, but since we're dealing with 255 byte packets, which is greater than the 63 byte FIFO size of the module (for FSK packets), we have to use pooling to fill and defill the FIFO in real time, hence, a bottleneck occurs because of the SPI and system speed. I did not have the time to investigate where the bottleneck actually lies, but a lot of optimization can be done to increase the transfer speed.


## Problems

In the lab, it works pretty well and is pretty stable. Transmitting and receiving many images, with a few damaged images here and there. When you start increasing the distance from the transmitter, images get choppier and more damaged.

When we put the board and battery into the styrofoam balls used to launch things with the baloon (necessary to protect against the -70 degree Celsius air in the stratosphere), it started overheating when on the ground, because the ESP32 peaks at 200 mA when taking a picture and 100 mA when transmitting. When this happened, the ESP32 would reset and require a manual reset to start working again. Attemps were made to put it into a low-power mode with very little success, and it seems the high power consumption is a known problem with the ESP32 CAM boards.

We actually tried testing it on a sonde flight, but in the hurry of tracking it, we forgot to actually use the receiving software, so our images were lost since we did not implement any SD Card functionality. And that takes us to another problem: the SD card. The SX1276 radio is using SPI, which interferes with the SD card pins, so unfortunately I could not get both things to work at the same time using the Arduino libraries.




## Some pictures we took when testing :)


<p align="center">
    <a href="http://zenith.eesc.usp.br">
    <img src="https://img.shields.io/badge/Check%20out-Zenith's Oficial Website-black?style=for-the-badge" />
    </a> 
    <a href="https://www.facebook.com/zenitheesc">
    <img src="https://img.shields.io/badge/Like%20us%20on-facebook-blue?style=for-the-badge"/>
    </a> 
    <a href="https://www.instagram.com/zenith_eesc/">
    <img src="https://img.shields.io/badge/Follow%20us%20on-Instagram-red?style=for-the-badge"/>
    </a>

</p>
<p align = "center">
<a href="zenith.eesc@gmail.com">zenith.eesc@gmail.com</a>
</p>
