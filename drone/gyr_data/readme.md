// ODR 380 hz
// cut off 25
// degree per second
2000  dps

// millidegree per second
70 mdps/digit
=> 1 digit == 70 mdps == 0,07 dps

output is 14.286 for 1.00002 degree

for 1 complete turn (360 degree) i need to turn 6 dps.
output is 6*14.286 = 85.716

85.716 * 60 = 5142.96

5142.96 / 14.286 = 360

380 reading p/s * 60 = 22800

22800 * 85.716 / 380 / 14.286 = 360


// degree per second
digital wero rate level +-75 dps


https://learn.sparkfun.com/tutorials/gyroscope/all
https://www.instructables.com/id/Accelerometer-Gyro-Tutorial/
https://morf.lv/mems-part-1-guide-to-using-accelerometer-adxl345
https://morf.lv/mems-part-2-guide-to-using-gyroscope-l3g4200d

