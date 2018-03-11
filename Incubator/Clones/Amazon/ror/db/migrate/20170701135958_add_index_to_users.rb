class AddIndexToUsers < ActiveRecord::Migration[5.0]
  def change
    # add_column :users, :username, :string unless column_exists? :users, :username
    add_index :users, :username, unique: true
  end
end
