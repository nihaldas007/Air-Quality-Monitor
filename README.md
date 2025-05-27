# Air-Quality-Monitor
### **Air Quality Monitoring System with TFT Display**

![image alt](https://github.com/nihaldas007/Air-Quality-Monitor/blob/610e2badc0942c17fb78a28bc563f76d45e1cd9c/Main%20Display.jpg)

#### **Project Overview**
This project is an **Air Quality Monitoring System** that utilizes a **TFT touchscreen display** to provide real-time environmental data. The system measures key air quality parameters, including **PM2.5, CO2 levels, gas concentration, temperature, and humidity**, and visually represents the data on a **MCUFRIEND TFT screen** using graphical elements.

#### **Features**
1. **Real-time Air Quality Monitoring:**  
   - Measures **PM2.5 concentration** (particulate matter up to 2.5 microns).  
   - Detects **CO2 levels** in parts per million (ppm).  
   - Monitors **gas concentration** as a percentage.  
   - Displays **temperature and humidity** readings.  

2. **TFT Graphical Interface:**  
   - Uses a **320x480 color TFT display** to present data.  
   - Displays sensor values in colored boxes with automatic range-based color changes (green, yellow, red).  
   - Includes **icons for WiFi, battery status, thermometer, and humidity levels** for a modern UI.  
   - Shows an **AQI (Air Quality Index) indicator**, mapping PM2.5 values to a readable air quality status.  

3. **Serial Communication & Data Processing:**  
   - Receives sensor data through **serial communication**.  
   - Parses incoming data and updates the display dynamically every **2 seconds**.  

4. **User Interface Elements:**  
   - **WiFi & Battery icons** for system status representation.  
   - **AQI bar** for quick assessment of air quality.  
   - **Temperature & humidity icons** for environmental conditions.  
   - **Sensor value boxes** with rounded edges for easy readability.  

#### **How It Works**
1. The system reads data from **external air quality sensors** (not included in the code but expected to be connected via serial communication).  
2. The data is received in a **comma-separated format** (temperature, humidity, CO2, PM2.5, gas).  
3. The program processes this data and updates the **TFT display** with new values.  
4. **Color-coded indicators** help users quickly assess air quality levels.  
5. The system refreshes the display every **2 seconds**, ensuring continuous real-time monitoring.  

#### **Potential Applications**
- **Indoor Air Quality Monitoring** (homes, offices, schools).  
- **Smart Agriculture** (monitoring air pollution in greenhouses).  
- **Industrial Safety** (tracking harmful gas emissions).  
- **IoT Integration** (could be expanded with cloud connectivity).  

This project is ideal for **environmental monitoring, smart homes, and IoT applications**, offering a **compact and visually appealing solution** for air quality assessment. 🚀
