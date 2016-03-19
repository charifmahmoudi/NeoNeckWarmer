# NeoNeckWarmer
Stylish IoT Neck Warmer controlled from mobile browser

## Motivation

After reading some articles about the fashion tends, i was surprised that the neck warmer is considered by some articles as one of the seven biggest trends for the Spring 2016. Like all self-respecting wearable maker, i highly was tempted to add an IoT touch to this fashion trend.

## Build and flash the code

The code source uses the Adafuit neopixel library. Make sure that you are using the latest version. the earlier versions did not have support for RGBW pixels rather than RGB pixels.

The code sources is composed from two main parts. First part handles the server interactions with the client. As illustrated bellow, the server embedded inside the MKR1000 listens on the port 80 and can serve three contents. 

 -    / : the HTML/javascript content for the web application
 -    /neo/R/G/B : display the (R,G,B) colors on the strip . where R, G, and B are values between 0 and 255.
 -    /rainbow : enable the rainbow mode and display a colorful rainbow over the strip.  
 
## Links

https://www.hackster.io/charifmahmoudi/stylish-iot-neck-warmer-controlled-from-mobile-browser-0bb93e
