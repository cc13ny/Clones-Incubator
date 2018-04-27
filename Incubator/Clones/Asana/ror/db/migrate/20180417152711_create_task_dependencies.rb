class CreateTaskDependencies < ActiveRecord::Migration[5.0]
  def change
    create_table :task_dependencies do |t|
      # ToDo: don't allow task_id, sub_task_id as null
      # ToDo: read http://guides.rubyonrails.org/association_basics.html
      # ToDo: - especially foreign key
      # ToDo: why it's t.references instead of t.belongs_to
      t.references :task, index: true, foreign_key: true
      t.references :sub_task, index: true
      # t.integer :sup_task_id
      # t.integer :sub_task_id

      t.timestamps
    end
    add_foreign_key :task_dependencies, :tasks, column: :sub_task_id
    # add_index :task_dependencies, :sup_task_id
    # add_index :task_dependencies, :sub_task_id
  end
end
