/*
 * DS3231-RTC.h
 *
 * The great C++ Library for the DS3231 Real-Time Clock chip
 *
 */

#ifndef __DS3231_RTC_H__
#define __DS3231_RTC_H__

#include <Arduino.h>
#include <time.h>
#include <Wire.h>

#define CLOCK_ADDRESS 0x68

#if !defined (UNIX_OFFSET)
// SECONDS_FROM_1970_TO_2000
// Difference between the Y2K and the UNIX epochs, in seconds.
// To convert a Y2K timestamp to UNIX... 
#define UNIX_OFFSET 946684800UL
#endif

#if !defined (NTP_OFFSET)
// SECONDS_FROM_1900_TO_2000
// Difference between the Y2K and the NTP epochs, in seconds.
// To convert a Y2K timestamp to NTP... 
#define NTP_OFFSET 3155673600UL
#endif

// DateTime class restructured by using standardized time functions
class DateTime {
    public:
        DateTime (time_t unix_timestamp = 0);

	    DateTime ( 	int16_t year, int8_t month, int8_t mday,
                    int8_t hour = 0, int8_t min = 0, int8_t sec = 0,
                    int8_t wday = 0, int16_t yday = 0, int16_t dst = -1);
    
        DateTime (const char *date, const char *time);
        
        int16_t getYear()   const	{ return _tm.tm_year + 1900; }
        int8_t getMonth()   const   { return _tm.tm_mon + 1; }
        int8_t getDay()     const   { return _tm.tm_mday; }
        int8_t getHour()    const   { return _tm.tm_hour; }
        int8_t getMinute()  const   { return _tm.tm_min; }
        int8_t getSecond()  const   { return _tm.tm_sec; }
        int8_t getWeekDay() const	{ return _tm.tm_wday; }
        int16_t getYearDay() const	{ return _tm.tm_yday; }
        int16_t getDST()    const   { return _tm.tm_isdst; }
        size_t show_DateTime(char *buffer, size_t buffersize, const char *formatSpec = "%a %h %d %T %Y");

        // time_t value as seconds since 1/1/2000
        time_t getY2kTime() const { return _y2k_timestamp; }
        
        // time_t value as seconds since 1/1/1970
        // THE ABOVE COMMENT IS CORRECT FOR LOCAL TIME; TO USE THIS COMMAND TO
        // OBTAIN TRUE UNIX TIME SINCE EPOCH, YOU MUST CALL THIS COMMAND AFTER
        // SETTING YOUR CLOCK TO UTC
        time_t getUnixTime() const  { return _unix_timestamp; }
    
    private:
        void set_timstamps();

    protected:
        time_t _unix_timestamp;
        time_t _y2k_timestamp;
        struct tm _tm;
};

class RTClib {
    public:
        // Get date and time snapshot
        static DateTime now(TwoWire &_Wire = Wire);
};

// Eric's original code is everything below this line
class DS3231 {
    public:
        // Constructor
        DS3231();
        DS3231(TwoWire &twowire);
        
        TwoWire &_Wire;

        // ************************************
        //      Time-retrieval functions
        // ************************************
        // Get the second of the DS3231 module
        byte getSecond();
        // Get the minute of the DS3231 module
        byte getMinute();

        // Get the hour of the DS3231 module, in addition, this function
        // returns the values of the 12/24-hour flag and the AM/PM flag.
        byte getHour(bool& h12, bool& PM_time);

        // Get the DayOfWeek of the DS3231 module   
        byte getDoW();

        // Get the date of the DS3231 module
        byte getDate();

        // Get the month and the century roll over of the DS3231 module
        byte getMonth(bool &century);
            
        // Get the year of the DS3231 module
        byte getYear();
            

        // ************************************
        //        Time-setting functions
        // ************************************
        // Note that none of these check for sensibility: You can set the
        // date to July 42nd and strange things will probably result.

        // set epoch function gives the epoch as parameter and feeds the RTC
        // epoch = UnixTime and starts at 01.01.1970 00:00:00
        void setEpoch(time_t epoch = 0, bool flag_localtime = false);

        // Set the Second of the DS3231 module
        void setSecond(byte second);
        // Set the minute of the DS3231 module
        void setMinute(byte minute);
        // Set the hour of the DS3231 module
        void setHour(byte hour);
        // Sets the Day of the Week (1...7) of the DS3231 module
        void setDoW(byte dayOfWeek);
        // Sets the Date of the DS3231 module
        void setDate(byte date);
         // Sets the Month of the DS3231 module
        void setMonth(byte month);
         // Sets the Year of the DS3231 module
        void setYear(byte year);
         // Sets the Hour format (12h/24h) of the DS3231 module
        void setClockMode(bool h12);
            

        // ************************************
        //        Temperature function
        // ************************************
        // get temperature of the DS3231 module
        float getTemperature();

        // Alarm functions
        void getA1Time(byte& A1Day, byte& A1Hour, byte& A1Minute, byte& A1Second, byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM);

        /* Retrieves everything you could want to know about alarm
        * one.
        * A1Dy true makes the alarm go on A1Day = Day of Week,
        * A1Dy false makes the alarm go on A1Day = Date of month.
        *
        * byte AlarmBits sets the behavior of the alarms:
        *	Dy	A1M4	A1M3	A1M2	A1M1	Rate
        *	X	1		1		1		1		Once per second
        *	X	1		1		1		0		Alarm when seconds match
        *	X	1		1		0		0		Alarm when min, sec match
        *	X	1		0		0		0		Alarm when hour, min, sec match
        *	0	0		0		0		0		Alarm when date, h, m, s match
        *	1	0		0		0		0		Alarm when DoW, h, m, s match
        *
        *	Dy	A2M4	A2M3	A2M2	Rate
        *	X	1		1		1		Once per minute (at seconds = 00)
        *	X	1		1		0		Alarm when minutes match
        *	X	1		0		0		Alarm when hours and minutes match
        *	0	0		0		0		Alarm when date, hour, min match
        *	1	0		0		0		Alarm when DoW, hour, min match
        *
        *	Note: byte AlarmBits is not explicitly cleared for the getAXTime methods to
        *	support sequential retreival of both alarms with the same byte AlarmBits.
        *	Use the flag bool clearAlarmBits=True to explicitly clear byte AlarmBits on
        *  call to getAXTime.
        */

        // Same as getA1Time();, but A2 only goes on seconds == 00.
        void getA2Time(byte& A2Day, byte& A2Hour, byte& A2Minute, byte& AlarmBits, bool& A2Dy, bool& A2h12, bool& A2PM);
        
        // Same as getA1Time();, but clears byte AlarmBits.
        void getA1Time(byte& A1Day, byte& A1Hour, byte& A1Minute, byte& A1Second, byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM, bool clearAlarmBits);

        // Same as getA1Time();, but clears byte AlarmBits.
        void getA2Time(byte& A1Day, byte& A1Hour, byte& A1Minute,byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM, bool clearAlarmBits);
        
        // Set the details for Alarm 1
        void setA1Time(byte A1Day, byte A1Hour, byte A1Minute, byte A1Second, byte AlarmBits, bool A1Dy, bool A1h12, bool A1PM);
        
        // Set the details for Alarm 2
        void setA2Time(byte A2Day, byte A2Hour, byte A2Minute, byte AlarmBits, bool A2Dy, bool A2h12, bool A2PM);
        
        // Enables alarm 1 or 2 and the external interrupt pin.
        // If Alarm != 1, it assumes Alarm == 2.
        void turnOnAlarm(byte Alarm);

        // Disables alarm 1 or 2 (default is 2 if Alarm != 1);
        // and leaves the interrupt pin alone.
        void turnOffAlarm(byte Alarm);
        
        // Returns T/F to indicate whether the requested alarm is
        // enabled. Defaults to 2 if Alarm != 1.
        bool checkAlarmEnabled(byte Alarm);
        
        // Checks whether the indicated alarm (1 or 2, 2 default);
        // has been activated. Always clears flag.
        bool checkIfAlarm(byte Alarm);
        
        // Checks whether the indicated alarm (1 or 2, 2 default);
        // has been activated. IF clearflag is set, clears alarm flag.
        bool checkIfAlarm(byte Alarm, bool clearflag);
            

        // ************************************
        //        Oscillator functions
        // ************************************

        // turns oscillator on or off. True is on, false is off.
        // if battery is true, turns on even for battery-only operation,
        // otherwise turns off if Vcc is off.
        // frequency must be 0, 1, 2, or 3.
        // 0 = 1 Hz
        // 1 = 1.024 kHz
        // 2 = 4.096 kHz
        // 3 = 8.192 kHz (Default if frequency byte is out of range);
        void enableOscillator(bool TF, bool battery, byte frequency);
        
        // Turns the 32kHz output pin on (true); or off (false).
        void enable32kHz(bool TF);
        
        // Checks the status of the OSF (Oscillator Stop Flag);.
        // If this returns false, then the clock is probably not
        // giving you the correct time.
        // The OSF is cleared by function setSecond();.
        bool oscillatorCheck();


    private:
        // the getter functions retrieve current values of the registers.
        byte getRegisterValue() {
            _Wire.requestFrom(CLOCK_ADDRESS, 1);
            return bcdToDec(_Wire.read());
        }

        // Convert normal decimal numbers to binary coded decimal
        byte decToBcd(byte val);
        // Convert binary coded decimal to normal decimal numbers
        byte bcdToDec(byte val);


    protected:
        // Read selected control byte: (0); reads 0x0e, (1) reads 0x0f
        byte readControlByte(bool which);
        
        // Write the selected control byte.
        // which == false -> 0x0e, true->0x0f.
        void writeControlByte(byte control, bool which);		
};
#endif
