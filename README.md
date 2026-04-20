Overview:


Dice thrower is a device that simulates the roll of multiple types and number of dice. With it is designed to help the user roll faster and get the result sooner, than with traditional physical dice. It is fully portable and runs on one rechargable LiON battery. located inside its case. To turn on this device you need to toggle the power switch on the right side of the device as seen below:


![](https://github.com/PatrykZymelka/DiceThrowerFinal/blob/main/Docs/Gifs/OnGIF.gif)

The device uses an OLED screen as its display, in the main menu you can see all the dice types that can be rolled and their amount, that was selected by the user.
We can roll the following 7 types of RPG dice:
- d4 (four sided die)
- d6 (six sided die)
- d8 (eight sided die)
- d10 (ten sided die)
- d12 (twelve sided die)
- d20 (twenty sided die)
- d100 (one hundred sided die)

Each type of die has its own assigned button located below the display. One additional button is the FUNC or function button which use will be explained in the results section.
To use the device firstly you need to choose the type and amount of dice you want to roll. To do so simply push the corresponding button to the type of die you want to roll. With each press you will increment the amount of dice rolled of the specific type.
You can see this on the below gif, in it we choose to roll four d20's and four d4's by simply holding down their buttons.


![](https://github.com/PatrykZymelka/DiceThrowerFinal/blob/main/Docs/Gifs/ChooseGIF.gif)

After selecting your dice to roll them you will need to gently shake the device, as if you were rolling real dice, when done correctly you will be notified by a quick tune comming from the speaker of the device. 
The gyroscope inside will detect sudden movement and sent the signal to the main board, to generate and display the results.
The results will be shown step by step, starting from the lowest type of die (in our case d4) and finishing on the largest type of die (in our example d20). On the result screen you will see each result individually on the top row, and the sum of all the rolls on the bottom row.
When you would want to see the next results or go back to the menu, simply bush the FUNC or function button located on the bottom right of the divice.
The following gif will display how this process works:


![](https://github.com/PatrykZymelka/DiceThrowerFinal/blob/main/Docs/Gifs/ResultsGIF.gif)

Hardware specification:
1) Microcontroller - Blackpill is based on the STM32F411CE microcontroller, which utilizes an ARM® 32-bit Cortex®-M4 core. This microcontroller operates at frequencies of up to 100 MHz, providing high performance for a wide range of applications. The board features 512 KB of Flash memory for firmware storage and 128 KB of SRAM for real-time data processing. The operating supply voltage ranges from 1.7V to 3.6V. Additionally, the board includes an RTC (Real-Time Clock), which is useful for applications requiring precise timekeeping. The STM32 Blackpill offers multiple communication interfaces, including USB 2.0, I2C, SPI/I2S, and USART, enabling easy integration with peripheral devices such as sensors, displays, audio devices, or communication modules. This development board can be programmed using popular development environments such as STM32CubeIDE or Arduino IDE, provided that STM32 core support and the required drivers are installed.
2) Battery - 18650 lithium-ion cell is a rechargeable battery with a typical capacity of 3000 mAh and a minimum capacity of 2900 mAh. Its nominal voltage is 3.7 V, although the actual voltage may vary depending on the state of charge. According to the specification, the maximum discharge current is 15 A, which is more than sufficient for this device.
3) Display - SH1106 1.3-inch 128x64 OLED display features a built-in SH1106 controller and supports multiple control platforms, including STM32 microcontrollers. It offers very low power consumption of 0.08 W, making it ideal for this project. The display operates in a voltage range from 3V to 5V and can function in temperatures from -30°C to 70°C. It communicates with the microcontroller via the SPI interface.
4) Accelotometer - LIS3DH 3-axis digital accelerometer, Adafruit 2809, enables acceleration measurements along the X, Y, and Z axes, allowing accurate detection of motion and device orientation. It operates at a supply voltage from 3V to 5V and communicates with the microcontroller using either I2C or SPI. The sensor provides 10-bit resolution and includes three additional analog inputs that can be read through I2C. Its measurement range is configurable and can be set to ±2 g, ±4 g, ±8 g, or ±16 g. It can also operate in Ultra Low Power Mode, where it consumes only 99 µA.
5) Step down voltage regulator -LM2596 step-down converter is a voltage reduction module operating with an input voltage range from 3.2 V to 35 V, with adjustable output voltage set via a potentiometer. Its output current ranges from 2 A to 3 A depending on thermal conditions and cooling, which is sufficient for this project. By placing the converter directly after the battery supply, stable voltage can be maintained throughout the entire operating time of the device.
6) Buzzer - MG15 buzzer is a small analog speaker with a power rating of 0.1 W and an impedance of 8 Ohms. It is used to generate an audible signal during the “dice roll” event. This is achieved by driving it with a PWM signal generated by the microcontroller.
7) Power switch - SS12T44 slide switch is a simple switch used in this project to turn the device power supply on and off.
8) Button switch - 6x6 mm THT tactile switches are standard momentary push-button switches and are fully sufficient for the requirements of this project.
