# pit-detection
Detecting ditches and pits filled with water on road in monsoon season and to disperse kerosene oil in them to prevent breading of mosquitoes in stagnant water
############################################################################
Components required:
1.	Microcontroller (I used Arduino pro mini to reduce the size but any Arduino UNO will work just fine)
2.	IR Sensor
3.	Ultrasonic Sensor (HC-SR04)
4.	Gyroscope and Accelerometer Sensor (MPU-6050)
5.	Relay Module
6.	Some connecting wires
################################################################################# 
The basic principle of this project this the fact that IR(Light) waves can travel through both air and water but ultrasonic(sound) waves reflect back from the surface of water.
Ultrasonic sensor (HC-SR04) is often used for detecting the water level of a tank for the same reason. So, it would have been enough to detect the distance from the floor of the car to the road but I needed some thing that could detect the distance of the road (depth of a pit) even if there is water present.IR sensor was just the thing that I needed to accomplish this job.
Combining the properties of these 2 sensors I wrote a code that could detect whether a glass is full of water. The depth of the glass was measures when it was empty using the ultrasonic sensor, now the potentiometer of the IR sensor (its threshold value) was set to the exact height of the glass (i.e. it won’t trigger if the height is even a millimetre more). Due to the phenomenon of refraction of light when it passes through different media, if there was water present the IR waves took more time to reach the receiver, fooling the sensor into thing that the depth of the glass was increased. 
One the code was working all I needed was to test it on water filled with debris and luckily the increase of impurities in water caused an increase in the time it took the light to reflect from the bottom of the pit. 
I integrated the Accelerometer to indicate the angle of the accelerator of the car. So when the acceleration paddle is at maximum inclination, we can assume that the driver is stopping the car. Once the car is stopped the relay is triggered emulating the dispersion of the oil.
Additional Information is being printed on the serial montior.
##################################################################################
Illustrative figures are included in the folder 
