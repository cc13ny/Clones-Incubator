class Task < ApplicationRecord
  # has_many :task_dependencies
  # - I think it's not necessary (above) because we already have
  #   - sup_task_dependencies
  #   - sub_task_dependencies
  #   which are more important

  has_many :sup_task_dependencies, foreign_key: :sub_task_id, class_name: 'TaskDependency', dependent: :delete_all
  has_many :sup_tasks, through: :sup_task_dependencies, source: :task

  has_many :sub_task_dependencies, foreign_key: :task_id, class_name: 'TaskDependency', dependent: :delete_all
  has_many :sub_tasks, through: :sub_task_dependencies
  # , source: :sub_task

  # def sup_tasks
  #   sup_task_dependencies.find_by(sup_task_id: id)
  # end
  #
  # def sub_tasks
  #   sub_task_dependencies.find_by(sub_task_id: id)
  # end

  # def has_suptask task
  #   sup_tasks.find_by(id: task.id).present?
  # end
  #
  # def has_subtask task
  #   sub_tasks.find_by(id: task.id).present?
  # end
end
