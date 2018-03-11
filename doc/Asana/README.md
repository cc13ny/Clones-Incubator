1. Think about requirements (__CRUD__)


In a __Project__, we have different __Tasks__. Tasks have different __Task Dependencies__, which means a __Task__ may has a __Super Task__ or __Sub Task__.

# Requirements


As an User

1. I can create a task
2. I can see a task
3. I can update a task
4. I can delete a task
5. I can add due date

6. I can discuss the task with others
7. The discussion will be recoreded
8. I can see task dependencies

9. I can know the status of the task
10. I can set the priority for the task


Less Important
1. I can find the task easily and quickly
2. The task itself can contain various information

# Ideas

1. Automatic Priority Calculation
# Pespective
1. from models
2. from use cases


# Models

1. User, Task, Comment

## Project

1. it has __Tasks__.
2. 

## Task

1. it can be __Completed__ or __Uncompleted__.
2. it has an __Asignee__ or __Not__


## User

1. Create a __Project__
2. See a __Project__
   + See the __timeline__ of a __Project__ based on __Tasks__
3. Update a __Project__
   + Update __description__
3. Delete a __Project__
2. Create a __Task__
3. See a __Task__
   + See the __descriptions__
   + See __due date__
   + See the __assignee__
   + See the __followers__
4. Delete a __Task__
5. Archive a __Task__
3. Assign a __Task__ to an __Asignee__
4. Set a __due date__ for a __Task__
5. Know which __Project__ a __Task__ is belonged to
6. Add __Followers__ to a __Task__
7. Manage a __Task__
   + Create a __Task__
   + Set a __due date__ for a __Task__
   + Mark a __Task__ as __completed__
   
8. Discuss a __Task__
   
   
# Buffer
 
1. Decision Tracking
2. Performance Analysis and Tracking

# Tech

1. Kafka
2. Redis
3. RabbitMQ
4. Backend Job
5. Puma Server
6. Ruby Guide

