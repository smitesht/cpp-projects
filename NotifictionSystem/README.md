# Event-Driven Monitoring System Simulation in C++

This project uses modern C++ to create a monitoring system that not only detects CPU and memory thresholds but also sends notifications through different channels (Email, SMS) using the Observer Pattern.

## 📌 Project Highlights:

- **Observer Pattern**: Centralized AlertManager notifies subscribed observers (EmailNotification, SMSNotification) whenever a threshold is breached.
- **Event-Driven Design**: Real-time monitoring via Metrics and MonitoringSystem detects and notifies on-the-fly.
- **Scalability**: Additional notifications or metrics can be seamlessly integrated.

## ⚙️ Core Code Structure:

- **Observer Interface**: Flexible INotification base class allows multiple notification channels.
- **EventManager**: Uses Observer Pattern to alert subscribed observers of critical metrics.
- **MonitoringSystem**: Monitors system performance and triggers alerts via AlertManager on threshold breaches.
- 
## 🔧 Future Enhancements: 
With real notifications and possibly logging, this setup could handle more complex, mission-critical monitoring requirements.

## UML Diagram
![image](https://github.com/user-attachments/assets/247bfd87-4f08-43f8-b29d-fdfd00724cfa)

## Code

### Notification 
```cpp
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
```

### Notification Factory
```cpp
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
```
### Event Listener and Event Manager

```cpp
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
```
### Matrics Monitoring System
```cpp
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

```

### main
```cpp
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
```

### Output
```
Email notification! CPU Usage Alert! CPU Usage: 78%
SMS notification! CPU Usage Alert! CPU Usage: 78%

Email notification! CPU Usage Alert! CPU Usage: 81%
SMS notification! CPU Usage Alert! CPU Usage: 81%

Email notification! Memory Usage Alert! Memory Usage: 91%
SMS notification! Memory Usage Alert! Memory Usage: 91%
```

