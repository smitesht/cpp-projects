#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

class INotification
{
public:
    virtual void send(const string& message) = 0;
    virtual ~INotification() = default;
};

class EmailNotification : public INotification
{
public:
    void send(const string& message) override {
        cout << "Email notification! " + message << endl;
    }
};

class SMSNotification : public INotification
{
public:
    void send(const string& message) override {
        cout << "SMS notification! " + message << endl;
    }
};

enum NotificationType
{
    EMAIL = 0,
    SMS
};

class NotificationFactory
{
public:
    shared_ptr<INotification> CreateNotification(NotificationType notificationType)
    {
        switch (notificationType)
        {
        case EMAIL:
            return make_shared<EmailNotification>();            
        case SMS:
            return make_shared<SMSNotification>();            
        default:
            return nullptr;            
        }
    }

};

class IEventListener
{
protected:
    vector<shared_ptr<INotification>> observers;
public:
    virtual void subscribe(shared_ptr<INotification> observer) = 0;

    virtual void notify(const string& message) = 0;

    virtual ~IEventListener() = default;
};

class EventManager : public IEventListener
{
    
public:
    EventManager(){}

    void subscribe(shared_ptr<INotification> observer) {
        observers.push_back(observer);
    }

    void notify(const string& message) {
        for (auto observer : observers)
        {
            observer->send(message);
        }
    }
};


class Metrics
{
public:
    int getCPUUsage()
    {
        return rand() % 100;
    }

    int getMemoryUsage()
    {
        return rand() % 100;
    }
};


class MonitoringSystem
{
private:
    Metrics metrics;
    shared_ptr<IEventListener> eventManager;
    int cpuThreshold;
    int memThreshold;

public:
    MonitoringSystem(shared_ptr<IEventListener> eventMgr, int cpuThrsld, int memThrsld)
        :eventManager(eventMgr), cpuThreshold(cpuThrsld), memThreshold(memThrsld)
    {}

    void monitor()
    {
        while (true)
        {
            int cpuUsage = metrics.getCPUUsage();
            int memUsage = metrics.getMemoryUsage();

            if (cpuUsage > cpuThreshold)
            {
                string message = "CPU Usage Alert! CPU Usage: " + to_string(cpuUsage) + "%";
                eventManager->notify(message);
            }

            if (memUsage > memThreshold)
            {
                string message = "Memory Usage Alert! Memory Usage: " + to_string(memUsage) + "%";
                eventManager->notify(message);
            }

            this_thread::sleep_for(chrono::seconds(2));
        }
    }
};

int main()
{
    shared_ptr<IEventListener> eventManager = make_shared<EventManager>();
    NotificationFactory notificationFactory;

    eventManager->subscribe(notificationFactory.CreateNotification(EMAIL));
    eventManager->subscribe(notificationFactory.CreateNotification(SMS));

    MonitoringSystem ms(eventManager, 75, 80);

    thread monitoringThread(&MonitoringSystem::monitor, &ms);
    monitoringThread.join();

    return 0;
}

