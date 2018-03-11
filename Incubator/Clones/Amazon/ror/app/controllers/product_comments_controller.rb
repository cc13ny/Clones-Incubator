class ProductCommentsController < ApplicationController
  def create
    @product = Product.find(params[:product_id])
    @comment = @product.product_comments.create(comment_params)
    redirect_to product_path(@product)
  end

  def destroy
    @product = Product.find(params[:product_id])
    @comment = @product.product_comments.find(params[:id])
    @comment.destroy
    redirect_to product_path(@product)
  end

  private
  def comment_params
    params.require(:product_comment).permit(:product_id, :user_id, :content)
  end
end
