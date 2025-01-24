# Task Tracker v1.0.0-alpha
This project is my solution to a project in roadmap.sh.
More can be found in [task-tracker](https://roadmap.sh/projects/task-tracker).

## About
This project is my effort to learning the ropes of C++. For this one, I specifically learn the structuring of files in a project.
It is much appreciated if you can leave any feedbacks on anything with this project. If you ever even read this, thank you so much! ^^'

## How to use
This project is specifically build in Visual Studio. It is advised to compile and manage the project files with Visual Studio.

The list of commands and their usage is given below:
```
# Adding a new task
task-cli add "Buy groceries"
# Output: Task added successfully (ID: 1)

# Updating and deleting tasks
task-cli update 1 "Buy groceries and cook dinner"
task-cli delete 1

# Marking a task as in progress or done
task-cli mark-in-progress 1
task-cli mark-done 1

# Listing all tasks
task-cli list

# Listing tasks by status
task-cli list done
task-cli list todo
task-cli list in-progress
```