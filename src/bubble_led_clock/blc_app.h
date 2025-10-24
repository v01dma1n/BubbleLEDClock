#ifndef BLC_APP_H
#define BLC_APP_H

#include <memory>
#include "blc_types.h"
#include "blc_access_point_manager.h"
#include "blc_weather_manager.h"
#include "blc_preferences.h"

#include "RTClib.h"
#include <ESP32NTPClock.h>          // Core library
#include <ESP32NTPClock_HT16K33.h> // The specific driver library
#include <base_ntp_clock_app.h>     // The base application engine
#include <i_weather_clock.h>        // The weather interface

#define AP_HOST_NAME "bubble-clock"

// Inherit from the base app engine and the weather interface
class BubbleLedClockApp : public BaseNtpClockApp, public IWeatherClock {
public:
    // Singleton access method
    static BubbleLedClockApp& getInstance() {
        static BubbleLedClockApp instance;
        return instance;
    }

    // Override the base setup and loop
    void setup() override;
    void loop() override;

    // Provide the hardware setup implementation required by the base class
    void setupHardware() override;

    // Public accessors
    AppPreferences& getPrefs() { return _appPrefs; }
    float getTempData();
    float getHumidityData();
    
    // --- Implementation of IBaseClock & IWeatherClock ---
    const char* getAppName() const override;
    const char* getSsid() const override { return _appPrefs.config.ssid; }
    const char* getPassword() const override { return _appPrefs.config.password; }
    const char* getTimezone() const override { return _appPrefs.config.time_zone; }
    const char* getTempUnit() const override { return _appPrefs.config.tempUnit; }
    const char* getOwmApiKey() const override { return _appPrefs.config.owm_api_key; }
    const char* getOwmCity() const override { return _appPrefs.config.owm_city; }
    const char* getOwmStateCode() const override { return _appPrefs.config.owm_state_code; }
    const char* getOwmCountryCode() const override { return _appPrefs.config.owm_country_code; }

    // State setters/checkers
    void setWeatherData(const OpenWeatherData& data) override { _currentWeatherData = data; }
    bool isOkToRunScenes() const override;

    // Hardware and utility methods
    void syncRtcFromNtp() override;
    void activateAccessPoint() override;
    void formatTime(char *txt, unsigned int txt_size, const char *format, time_t now) override;
    IDisplayDriver& getDisplay() override { return _display; }
    DisplayManager& getClock() override { return *_displayManager; } // Now using unique_ptr
    RTC_DS1307& getRtc() override { return _rtc; }
    bool isRtcActive() const override { return _rtcActive; }

private:
    BubbleLedClockApp(); // Private constructor for singleton

    // Application-specific hardware
    DispDriverHT16K33 _display;
    std::unique_ptr<DisplayManager> _displayManager; // Use unique_ptr
    RTC_DS1307 _rtc;
    bool _rtcActive;

    // Application owns its specific preferences and AP manager
    AppPreferences _appPrefs;
    BlcAccessPointManager _apManager;
    
    // App-level managers and state data
    std::unique_ptr<WeatherDataManager> _weatherManager; 
    OpenWeatherData _currentWeatherData;
};

#endif // BLC_APP_H