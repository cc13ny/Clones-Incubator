# require 'elasticsearch/model'

class Product < ApplicationRecord
  # include Elasticsearch::Model
  # include Elasticsearch::Model::Callbacks
  include Searchable

  has_many :product_comments #it's not created automatically

  def self.search(search)
    if search
      Product.__elasticsearch__.search(search).records
    else
      Product.all
    end
  end
end