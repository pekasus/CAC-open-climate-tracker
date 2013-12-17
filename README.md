# SEPTA Climate Tracker

This application uses an Arduino to log temperature, humidity and (in the future) other climate sensor data on SEPTA buses to create a world-first moving heat island of thousands of data points all day long.

The size of the logged points is the humidity change against the average and the color is the temperature change compared to the average with (blue for cooler, red for hotter).

![alt tag](https://raw.github.com/davewalk/septa-climate-tracker/master/img/photo1.jpg)

[Wiki Page](https://github.com/davewalk/septa-climate-tracker/wiki)


### Next steps

While we are waiting for the 1284 boards to be printed, we will run the program on the Beaglebone Black. The initial components are:

- GPS
- Temp/Humidity
- Storage to onboard flash drive
- Upload data to computer

### Future steps

- Connect to web via wifi and transfer files
- Explore data transmission with XBee
- Parse and store the Arduino data in a database
- Graphically display data on map
- Expose the database as an API

Created by Joshua Meyer, Abdus Muwwakkil and Dave Walk at the [2013 Philadelphia Transit Hackathon](http://appsforphilly.org/events/transit-2013).
