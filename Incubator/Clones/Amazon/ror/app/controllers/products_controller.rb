class ProductsController < ApplicationController
  def index
    # Product.import
    # @products = Product.search('foobar').records
    # @products = Product.all
    # @search = Product.__elasticsearch__.search params[:search]

    @products = Product.search(params[:search])
  end

  def show
    @product = Product.find(params[:id])
  end

  def new
    @product = Product.new
  end

  def edit
    @product = Product.find(params[:id])
  end

  def create
    @product = Product.new(product_params)

    if @product.save
      redirect_to action: 'index'
    else
      render 'new'
    end
  end

  def update
    @product = Product.find(params[:id])

    if @product.update(product_params)
      redirect_to action: 'index'
    else
      render 'edit'
    end
  end

  def destroy
    @product = Product.find(params[:id])
    @product.destroy

    redirect_to products_path
  end

  private
  def product_params
    params.require(:product).permit(:title, :description)
  end
end
