# Cell-Metrix
🔋 Smart Battery Monitoring &amp; Charging System (Arduino + ESP8266)
This project is a smart battery management system using an Arduino Nano and ESP8266 (NodeMCU) that:

💡 Monitors battery voltage in real-time via a voltage divider and ESP8266 analog input

🌡️ Tracks temperature using a DS18B20 digital temperature sensor

🔁 Controls a cooling fan if the temperature exceeds 40°C

📣 Activates a buzzer warning when battery voltage is low and not charging

🔌 Regulates charging via a P-channel MOSFET and stops charging when the battery exceeds 40V

🖥️ Displays system status (voltage, temperature, charging status) on a 16x2 LCD

🔗 Uses UART communication between ESP8266 and Arduino Nano for seamless data transmission

Ideal for solar-powered systems, portable battery setups, or educational hardware integration projects.

                                                AI integration

We trained a lightweight AI model using real-world BMS data. The model predicts battery anomalies using just temperature and voltage.

It’s designed to enhance the decision-making of our BMS by identifying early failure risks 

and recommending safe actions like activating the fan, cutting charge, or alerting the user
