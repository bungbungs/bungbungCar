# bungbungCar

A auto driving car project using two MPU (master and slave)
The boards control the parts of the car(e.g. wheels, ultrasonic sensors, buzzer, etc.) and communicate each other when needed.
The software architecture is like the following:

<img width="644" alt="붕붕이 구조" src="https://user-images.githubusercontent.com/22390526/62760996-5f7c9c80-bac0-11e9-8ace-55fe7c4d953a.png">

# What bungbungCar can do?

1. Bungbung can found out the path by itself, using the ultrasonic sensors, and automatically drives on the path
2. It can do some commands like stopping, spinning, speeding up, etc by sending a RF(radio frequency) packet to bungbung
3. If the track has colors, the buzzer on bungbung can make sound, and during the drive it can make some music!!
