class TaskDependenciesController < ApplicationController
  def index
    @task = Task.find(params[:task_id])
    # @subtasks = @task.sub_tasks
    # @suptasks = @task.sup_tasks
  end

  def destroy
  end
end
