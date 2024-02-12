## 📕️ __Overview__

The **Home Automation PRO** (Personalized Residential Operations) project aims to create a system that enables the control and monitoring of Internet of Things (**IoT**) devices, including but not limited to, smart home appliances, security systems, and lighting systems. The system uses a WiFi module to connect to the internet and receive commands from a centralized management interface. Additionally, voice recognition technology is implemented to enable users to interact with the system using voice commands. The overall management of various sensors and actuators is handled by the **LPC845-BRK** controller, which allows for efficient and reliable communication between the system components. With this system in place, users can enjoy the convenience of managing their home devices with ease and flexibility, using the latest technologies available.

### 🔌️ __Hardware & Setup__

- [ ] [Identification of the sensors, modules and controllers used.](https://github.com/Danmuse/Home-automation/issues/3) `DDM ~ 2`
  <details>
  <summary><strong><span>&#160;</span>Electronic components</strong></summary>
  <ul>
  <span>&#10038;</span> LPC845-BRK.<br>
  <span>&#10038;</span> ESP8266 WiFi Serial ESP-01 module. (Optional)<br>
  <span>&#10038;</span> Voice recognition module. (Optional)<br>
  <span>&#10038;</span> Display LCD2004A with HD44780U controller.<br>
  <span>&#10038;</span> Control of RGB LEDs with NEC protocol.<br>
  <span>&#10038;</span> LDR (Light-Dependent Resistor) sensor.<br>
  <span>&#10038;</span> Servo MG90S controlled by PWM.<br>
  <span>&#10038;</span> Flame sensor KY-026. (Optional)<br>
  <span>&#10038;</span> Infrared sensor KY-032 or HW-201. (Optional)<br>
  <span>&#10038;</span> EEPROM Memory FM24C16U 16KB.<br>
  <span>&#10038;</span> RTC module DS3231SN.<br>
  <span>&#10038;</span> RFID RC-522 reader.<br>
  <span>&#10038;</span> DFPlayer module.<br>
  </ul>
</details>

- [ ] [Restorations of the house model.](https://github.com/Danmuse/Home-automation/issues/1) `DDM ~ 17`
- [ ] [Hardware installations on the house model.](https://github.com/Danmuse/Home-automation-PRO/issues/28) `DDM ~ 2`
- [ ] Development board (Schematic and PCB).
- [x] 1:18 scale house model.

### 👨‍💻️ __Software & GUI APP__

- [ ] [Desktop and mobile graphical application for controlling automation created in **Qt Creator**.](https://github.com/Danmuse/Home-automation-PRO/issues/12) `LC ~ 17` `CY ~ 4` `DDM ~ 2`
- [ ] [Relational database implementation to manage automation created in Qt Creator.](https://github.com/Danmuse/Home-automation-PRO/issues/13) `LC ~ 29` `AO ~ 20`
- [ ] Implementation of AT commands for WiFi module configuration and voice commands.
- [ ] [Achieving connectivity between sensors and modules with **LPC845-BRK**.](https://github.com/Danmuse/Home-automation/issues/6) <br>
`DDM ~ 250` `AO ~ 116.5` `LC ~ 14` `CY ~ 8`
- [ ] [Communication with **MySQL** Relational Database.](https://github.com/Danmuse/Home-automation/issues/7) `LC ~ 6` `AO ~ 4`
- [ ] [Micro SD card adaptation for data handling and ".mp3" files using DFPlayer module.](https://github.com/Danmuse/Home-automation-PRO/issues/27) `DDM ~ 16`

### 📚️ __Documentation & Doxygen__

- [ ] [Doxygen v1.9.6 Implementation for Code Documentation.](https://github.com/Danmuse/Home-automation/issues/2) `DDM ~ 13` `AO ~ 3.5`
- [x] [Presentation of the project idea.](https://github.com/Danmuse/Home-automation/issues/4) `DDM ~ 9`
- [ ] [Comprehensive and extensive project report.](https://github.com/Danmuse/Home-automation/issues/8) `CY ~ 22`
- [ ] Illustrations of the experience and trials.
- [ ] [Load component datasheet.](https://github.com/Danmuse/Home-automation/issues/5) `DDM ~ 1.5`

---

## 🌟️ __Contributing__

General Software Developers: __@AgustinOrdonez__ ~ __@Danmuse__ ~ __@cyepez4698__ ~ __@DiCerso__  
General Hardware Developers: __@AgustinOrdonez__ ~ __@Danmuse__ ~ __@cyepez4698__ ~ __@DiCerso__  
GUI Application Developers and Setup Managers: __@DiCerso__ ~ __@Danmuse__  
Documentation Managers: __@AgustinOrdonez__ ~ __@cyepez4698__  
Back-End Developer: __@DiCerso__  
Quality Assurance: __@cyepez4698__  
Project Assistant: __@AgustinOrdonez__  
Doxygen Editor: __@Danmuse__  

<br>

<a href="https://github.com/Danmuse/Home-automation/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Danmuse/Home-automation&max=4" />
</a>
