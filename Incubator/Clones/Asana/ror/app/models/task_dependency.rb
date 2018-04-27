class TaskDependency < ApplicationRecord
  belongs_to :task
  belongs_to :sub_task, foreign_key: 'sub_task_id', class_name: 'Task'
  # ToDo: maybe we can remove the foreign_key. Would it think the key should be 'task_id' if I don't set it
  # ToDo: at least now it's safe
  # By convention, Rails assumes that
  #   the column used to hold the foreign key on this model is
  #     the name of the association with the suffix _id added.
  # The :foreign_key option lets you set the name of the foreign key directly:
end