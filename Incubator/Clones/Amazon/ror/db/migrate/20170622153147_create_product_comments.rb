class CreateProductComments < ActiveRecord::Migration[5.0]
  def change
    create_table :product_comments do |t|
      t.integer :user_id
      t.text :content
      t.references :product, foreign_key: true
      t.references :user, foreign_key: true

      t.timestamps
    end
  end
end
