object SqrtNewton{

  def abs(x: Double) = if (x < 0) -x else x
  
  def sqrt(x: Double) = {
    def sqrtIter(guess: Double): Double =
        if (stopCond(guess)) x
        else sqrtIter(approx(guess))
  
    def stopCond(guess: Double) =
        abs(guess * guess - x)/x < 0.001
  
    def approx(guess: Double) = 
        (guess + x / guess) / 2
      
    sqrtIter(1.0)
  }
      
  def main(args: Array[String]){
      println(sqrt(4))
  }
  
}
