#include "blc_app.h"
#include "debug.h"
#include "version.h"             
#include <Wire.h> 

// --- Data getters for the scene playlist ---
float BubbleLedClockApp_getTimeData() { return 0; }
float BubbleLedClockApp_getTempData() { return BubbleLedClockApp::getInstance().getTempData(); }
float BubbleLedClockApp_getHumidityData() { return BubbleLedClockApp::getInstance().getHumidityData(); }

// --- The application's specific scene playlist ---
static const DisplayScene scenePlaylist[] = {
    { "Time",        "%H.%M.%S", SLOT_MACHINE, false, false, 10000, 200, 50, BubbleLedClockApp_getTimeData },
    { "Date",        "%b %d %Y", SCROLLING,    true,  false,  7000, 300,  0, BubbleLedClockApp_getTimeData },
    { "Time",        "%H.%M.%S", SLOT_MACHINE, false, false, 10000, 200, 50, BubbleLedClockApp_getTimeData },
    { "Temperature", "%3.0f F",  MATRIX,       false, false,  5000, 250, 40, BubbleLedClockApp_getTempData },
    { "Temperature", "%3.0f C",  MATRIX,       false, false,  5000, 250, 40, BubbleLedClockApp_getTempData },
    { "Time",        "%H.%M.%S", SLOT_MACHINE, false, false, 10000, 200, 50, BubbleLedClockApp_getTimeData },
    { "Humidity",    "%3.0f PCT",MATRIX,       false, false,  5000, 250, 40, BubbleLedClockApp_getHumidityData }
};
static const int numScenes = sizeof(scenePlaylist) / sizeof(DisplayScene);

// --- Constructor ---
BubbleLedClockApp::BubbleLedClockApp() :
    _display(HT16K33_I2C_DEF_ADR, HT16K33_DEF_DISP_SIZE), // Default I2C addr and 8 digits
    _appPrefs(),
    _apManager(_appPrefs)
{
    _displayManager = std::make_unique<DisplayManager>(_display);
    _prefs = &_appPrefs; // Assign pointer for base class
    BaseNtpClockApp::_apManager = &_apManager; // Assign pointer for base class
    _rtcActive = false; // Initialize state
}
 
// --- Hardware Setup ---
void BubbleLedClockApp::setupHardware() {
    i2c_bus_clear(); // Use library's version
    Wire.begin();
    _displayManager->begin();
    _rtcActive = _rtc.begin();
    if (_rtcActive && (!_rtc.isrunning() || _rtc.now() < DateTime(F(__DATE__), F(__TIME__)))) {
        LOGINF("RTC time invalid, setting to compile time.");
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    } else if (_rtcActive) {
        LOGINF("RTC time is valid.");
    } else {
        LOGERR("RTC module not found!");
    }
    }
    
// --- Main Setup ---
void BubbleLedClockApp::setup() {
    // 1. Call the base class's setup engine FIRST
    BaseNtpClockApp::setup(); // This calls setupHardware() internally
    
    // 2. Initialize application-specific managers
    _weatherManager = std::make_unique<WeatherDataManager>(*this);
    if (_sceneManager) {
        _sceneManager->setup(scenePlaylist, numScenes);
    }
      
    LOGINF("--- Bubble LED Clock App Setup Complete ---");
    _appPrefs.dumpPreferences(); // Dump loaded prefs
}

// --- Main Loop ---
void BubbleLedClockApp::loop() {
    // 1. Call the base class's loop engine
    BaseNtpClockApp::loop();

    // 2. Call the update loops for application-specific managers
    if (_weatherManager) _weatherManager->update();
}

// --- Interface Implementations ---

void BubbleLedClockApp::activateAccessPoint() {
    _apManager.setup(getAppName());
    String waitingMsgStr = "AP MODE -- WIFI ";
    waitingMsgStr += getAppName();
    String connectedMsgStr = "CONNECT AT ";
    connectedMsgStr += WiFi.softAPIP().toString();
    _apManager.runBlockingLoop(*_displayManager, waitingMsgStr.c_str(), connectedMsgStr.c_str());
}

float BubbleLedClockApp::getTempData() { 
    return _currentWeatherData.isValid ? _currentWeatherData.temperatureF : UNSET_VALUE;
}

float BubbleLedClockApp::getHumidityData() { 
    return _currentWeatherData.isValid ? _currentWeatherData.humidity : UNSET_VALUE;
}

bool BubbleLedClockApp::isOkToRunScenes() const {
    return _fsmManager && _fsmManager->isInState("RUNNING_NORMAL");
}

void BubbleLedClockApp::formatTime(char *txt, unsigned txt_size, const char *format, time_t now) {
    struct tm timeinfo = *localtime(&now);
    strftime(txt, txt_size, format, &timeinfo);
    if (txt[0] == '0' && (strstr(format, "%H") == format || strstr(format, "%I") == format)) { // Suppress leading zero only for hours
        txt[0] = ' ';
    }
}

void BubbleLedClockApp::syncRtcFromNtp() {
    if (!_rtcActive) return;
    time_t now_utc = time(nullptr);
    _rtc.adjust(DateTime(now_utc));
    LOGINF("RTC synchronized with NTP time.");
}

const char* BubbleLedClockApp::getAppName() const {
    return AP_HOST_NAME;
}