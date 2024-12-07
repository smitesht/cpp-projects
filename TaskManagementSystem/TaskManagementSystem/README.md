# Task Management Simulation in C++

## 🛠 Problem Statement

Efficient task management is essential in project tracking, where tasks transition through various states (e.g., Created, InProgress, Completed) and require dynamic notifications to stakeholders. Designing such systems can become challenging due to:

**State transitions**: Tasks must change states seamlessly.

**Real-time notifications**: Observers (e.g., emails, SMS) should be notified of state changes.

**Extensibility**: Adding new states or notification mechanisms should not require significant code overhaul.

## 💡 Project Overview

This project simulates a task management system using Modern C++ features and demonstrates:

- **State Pattern**: Manages task states like Created, InProgress, Completed, and Cancelled. Each state encapsulates its behavior and ensures smooth state transitions.

- **Observer Pattern**: Dynamically registers observers (Email and SMS) and sends notifications whenever a task state changes.

- **Task Management Workflow**: Allows tasks to have priorities, descriptions, start/end dates, and dynamic updates.

- **Modern C++ Features**: Utilizes unique_ptr, shared_ptr, and other advanced C++ capabilities for effective memory management and object lifetime control.

## UML

![image](https://github.com/user-attachments/assets/4ba4a2f2-4b61-4d8a-9faa-1fc308f43574)

## Code Snippet

### Define abstraction

```cpp
enum TaskPriority {
    HIGH = 0,
    MEDIUM,
    LOW
};

class TaskContext;
class TaskManager;

struct TaskState {
    virtual ~TaskState() = default;
    virtual void handle(TaskContext& context) = 0;
    virtual string getCurrentState() = 0;
};

struct TaskObserver {
    virtual ~TaskObserver() = default;
    virtual void Update(const string message) = 0;
};
```

### Create Context for state management and task observers

```cpp
class TaskContext {
private:
    unique_ptr<TaskState> currentTaskState;
    vector<shared_ptr<TaskObserver>> taskObservers;

public:
    explicit TaskContext(unique_ptr<TaskState> newState)
        :currentTaskState(std::move(newState)){}

    void setState(unique_ptr<TaskState> newState) {
        currentTaskState = std::move(newState);
    }

    void addTaskObserver(shared_ptr<TaskObserver> observer) {
        taskObservers.push_back(observer);
    }

    void notify(const string message) {

        for (shared_ptr<TaskObserver> observer : taskObservers) {
            observer->Update(message);
        }
    }

    void executeState() {
        if (currentTaskState) {
            currentTaskState->handle(*this);
        }
    }

    string getCurrentState() {
        return currentTaskState ? currentTaskState->getCurrentState() : "Invalid State";
    }
};
```
### Implement each states

```cpp
class CreateState : public TaskState {
public:
    void handle(TaskContext& context) override {
        string message = "Task has been created...";
        cout << message << endl;
        context.notify(message);
    }

    string getCurrentState() override {
        return "Created";
    }
};

class InProgressState : public TaskState {
public:
    void handle(TaskContext& context) override {
        string message = "Task has been changed to InProgress...";
        cout << message << endl;
        context.notify(message);
    }

    string getCurrentState() override {
        return "InProgress";
    }
};

class CompletedState : public TaskState {
public:
    void handle(TaskContext& context) override {
        string message = "Task has been completed...";
        cout << message << endl;
        context.notify(message);
    }

    string getCurrentState() override {
        return "Completed";
    }
};

class CancelledState : public TaskState {
public:
    void handle(TaskContext& context) override {
        string message = "Task has been Cancelled...";
        cout << message << endl;
        context.notify(message);
    }

    string getCurrentState() override {
        return "Cancelled";
    }
};
```
### Implement task observers

```cpp
class EmailTaskObserver : public TaskObserver {
private:
    string email;
public:

    explicit EmailTaskObserver(const string email)
        :email(email) {}

    void Update(const string message) override {
        cout << "Email: " << email << " , message is " << message << endl;
    }
};

class SMSTaskObserver : public TaskObserver {
private:
    string phone;
public:

    explicit SMSTaskObserver(const string phone)
        :phone(phone) {}

    void Update(const string message) override {
        cout << "SMS : " << phone << " , message is " << message << endl;
    }
};
```

```cpp
class Task {
private:
    int Id;
    string taskTitle;
    string taskDescription;
    time_t startDate;
    time_t endDate;
    TaskPriority taskPriority;
    TaskContext taskContext;
    friend TaskManager;

    Task(int id, string title, TaskPriority priority)
        :Id(id), taskTitle(title), taskPriority(priority),
        taskContext(make_unique<CreateState>()),
        startDate(std::time(nullptr)), endDate(startDate + 2 * 24 * 60 * 60)
    {
        taskContext.executeState();
    }

public:
    void setTitle(string title) {
        taskTitle = title;
    }

    void setDescription(string desc) {
        taskDescription = desc;
    }

    void setStartDate(time_t startDate) {
        this->startDate = startDate;
    }

    void setEndDate(time_t endDate) {
        this->endDate = endDate;
    }

    void setTaskPriority(TaskPriority priority) {
        taskPriority = priority;
    }

    void addTaskObserver(shared_ptr<TaskObserver> observer) {
        taskContext.addTaskObserver(observer);
    }

    void changeState(unique_ptr<TaskState> newState) {
        taskContext.setState(std::move(newState));
        taskContext.executeState();
    }       

    string getCurrentTaskPriorityInString() {
        if (taskPriority == HIGH)
            return "High";
        else if (taskPriority == MEDIUM)
            return "Medium";
        else if (taskPriority == LOW)
            return "Low";
        else
            return "Invalid";
    }

    void print() {

        char startDateStr[20], endDateStr[20];
        tm startDt, endDt;
        localtime_s(&startDt, &startDate); //localtime_r for Linux
        localtime_s(&endDt, &endDate); //localtime_r for Linux

        std::strftime(startDateStr, sizeof(startDateStr), "%Y-%m-%d", &startDt);
        std::strftime(endDateStr, sizeof(endDateStr), "%Y-%m-%d", &endDt);

        cout << "Task Details" << endl;
        cout << "Id: " << Id << ", Priority: " << getCurrentTaskPriorityInString() << endl;
        cout << "Title: " << taskTitle << endl;
        cout << "Description: " << taskDescription << endl;
        cout << "Start Date: " << startDateStr << ", End Date: " << endDateStr << endl;
        cout << "Task Status: " << taskContext.getCurrentState() << endl;
    }
};
```
### Create task builder

```cpp
class TaskManager
{
private:
    Task task;
public:
    TaskManager(int id, string title, TaskPriority priority)
        :task(id,title,priority)
    {}

    TaskManager& setDescription(string description) {
        task.taskDescription = description;
        return *this;
    }

    TaskManager& setPriority(TaskPriority taskPriority) {
        task.taskPriority = taskPriority;
        return *this;
    }

    TaskManager& setStartDate(time_t startDate) {
        task.startDate = startDate;
        return *this;
    }

    TaskManager& setEndDate(time_t endDate) {
        task.endDate = endDate;
        return *this;
    }

    TaskManager& addTaskObserver(shared_ptr<TaskObserver> observer) {
        task.addTaskObserver(observer);
        return *this;
    }

    Task build() {
        return std::move(task);
    }
};
```

```cpp
int main()
{

    Task t1 = TaskManager(1001, "C++ Dev Task", LOW)
        .setDescription("This is C++ Dev Task for the LinkedIn")
        .addTaskObserver(make_shared<EmailTaskObserver>("jhon.d@gmail.com"))
        .addTaskObserver(make_shared<EmailTaskObserver>("martha.p@gmail.com"))
        .addTaskObserver(make_shared<SMSTaskObserver>("+1-(234)-(435)-1122"))
        .addTaskObserver(make_shared<SMSTaskObserver>("+91-(982)-(432)-3878"))
        .build();
    t1.print();
    cout << endl;
    t1.setTaskPriority(HIGH);
    t1.print();
    cout << endl;    
    t1.changeState(make_unique<InProgressState>());
    cout << endl;
    t1.changeState(make_unique<CompletedState>());

    return 0;
}
```

```cpp

OUTPUT
==============

Task has been created...
Task Details
Id: 1001, Priority: Low
Title: C++ Dev Task
Description: This is C++ Dev Task for the LinkedIn
Start Date: 2024-12-07, End Date: 2024-12-09
Task Status: Created

Task Details
Id: 1001, Priority: High
Title: C++ Dev Task
Description: This is C++ Dev Task for the LinkedIn
Start Date: 2024-12-07, End Date: 2024-12-09
Task Status: Created

Task has been changed to InProgress...
Email: jhon.d@gmail.com , message is Task has been changed to InProgress...
Email: martha.p@gmail.com , message is Task has been changed to InProgress...
SMS : +1-(234)-(435)-1122 , message is Task has been changed to InProgress...
SMS : +91-(982)-(432)-3878 , message is Task has been changed to InProgress...

Task has been completed...
Email: jhon.d@gmail.com , message is Task has been completed...
Email: martha.p@gmail.com , message is Task has been completed...
SMS : +1-(234)-(435)-1122 , message is Task has been completed...
SMS : +91-(982)-(432)-3878 , message is Task has been completed...

```
