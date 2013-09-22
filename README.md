# SEPTA Climate Tracker

This application uses an Arduino to log temperature, humidity and (in the future) other climate sensor data on SEPTA buses to create a world-first moving heat island of thousands of data points all day long.

The size of the logged points is the humidity change against the average and the color is the temperature change compared to the average with (blue for cooler, red for hotter).

![alt tag](https://raw.github.com/davewalk/septa-climate-tracker/master/img/photo1.jpg)

### Future steps

- Print humidity, temperature, latitude and longitude to a log file on the Arduino
- Transfer log file to a server over WiFi
- Parse and store the Arduino data in a database
- Expose the database as an API

Created by Joshua Meyer, Abdus Muwwakkil and Dave Walk at the [2013 Philadelphia Transit Hackathon](http://appsforphilly.org/events/transit-2013).