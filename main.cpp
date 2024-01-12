#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

enum class SensorType
{
    Altitude,
    SpeedInKmh,
    FuelConsumption
};

class SensorData
{
    float value;
    SensorType sensorType;
    time_t time;

public:
    SensorType GetSensorType() const { return sensorType; }
    float GetValue() const { return value; }
    time_t GetTime() const { return time; }
    void IncreaseFuelConsumption(float increase) { value += increase; }
    SensorData(SensorType sensorType, float value, time_t time)
    {
        this->value = value;
        this->sensorType = sensorType;
        this->time = time;
    }
};

void FillData(vector<SensorData> &v);
time_t CreateTime(int year, int month, int day, int hour, int minute, int second);

int main()
{
    vector<SensorData> sensorData;
    FillData(sensorData);

    // 1. Räkna sensordataregistreringar för Altitude den 2012-01-02.
    int altitudeCount = count_if(sensorData.begin(), sensorData.end(), [](const SensorData &data) {
        time_t time = data.GetTime();
        tm *ltm = localtime(&time);
        return (ltm->tm_year == 112 && ltm->tm_mon == 0 && ltm->tm_mday == 2);
    });
    cout << "Antal Altitude-registreringar den 2012-01-02: " << altitudeCount << endl;

    // 2. Kontrollera om det finns SpeedInKmh-registreringar med hastighet över 99.9.
    bool maxSpeedReached = any_of(sensorData.begin(), sensorData.end(), [](const SensorData &data) {
        return data.GetSensorType() == SensorType::SpeedInKmh && data.GetValue() > 99.9;
    });
    if (maxSpeedReached)
        cout << "Maxhastighet uppnådd" << endl;
    else
        cout << "Ingen maxhastighet uppnådd" << endl;

    // 3. Uppdatera FuelConsumption-poster med en ökning på 75%.
    for_each(sensorData.begin(), sensorData.end(), [](SensorData &data) {
        if (data.GetSensorType() == SensorType::FuelConsumption)
        {
            data.IncreaseFuelConsumption(data.GetValue() * 0.75);
        }
    });

    return 0;
}

void FillData(vector<SensorData> &v)
{
    srand(time(NULL));

    time_t tid = CreateTime(2012, 1, 1, 1, 1, 1);
    for (int i = 0; i < 100000; i++)
    {
        SensorType type = static_cast<SensorType>(rand() % 3);
        float value = 0.0f;
        if (type == SensorType::Altitude)
            value = rand() % 1000;
        else if (type == SensorType::FuelConsumption)
            value = rand() * 3.0f;
        else if (type == SensorType::SpeedInKmh)
            value = rand() % 110;
        else
        {
            value = 99;
        }
        v.push_back(SensorData(type, value, tid));
        tid = tid + rand() % 10 + 1;
    }
}

time_t CreateTime(int year, int month, int day, int hour, int minute, int second)
{
    struct tm tid = {0};
    tid.tm_year = year - 1900;
    tid.tm_mon = month - 1;
    tid.tm_mday = day;
    tid.tm_hour = hour;
    tid.tm_min = minute;
    tid.tm_sec = second;
    return mktime(&tid);
}
// del 1, 2 och 3 är klara.
