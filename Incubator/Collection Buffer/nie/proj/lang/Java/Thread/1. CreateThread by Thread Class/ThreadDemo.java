class ThreadExtended extends Thread {
   private Thread t;
   private String threadName;
   
   ThreadExtended(String name){
       threadName = name;
       System.out.println("---Creating " +  threadName);
   }
   
   public void run() {
      System.out.println("-------------Running " +  threadName );
      try {
         for(int i = 4; i > 0; i--) {
            System.out.println("--------------------Thread: " + threadName + ", " + i);
            // Let the thread sleep for a while.
            Thread.sleep(50);
         }
      } catch (InterruptedException e) {
         System.out.println("Thread " +  threadName + " interrupted.");
      }
      
      System.out.println("-----------------------------Thread " +  threadName + " exiting.");
   }
   
   public void start ()
   {
      System.out.println("---------Starting " +  threadName );
      if (t == null)
      {
         t = new Thread (this, threadName);
         t.start ();
      }
   }

}

public class ThreadDemo {
   public static void main(String args[]) {
   
      ThreadExtended T1 = new ThreadExtended( "Thread-A");
      T1.start();
      
      ThreadExtended T2 = new ThreadExtended( "Thread-B");
      T2.start();
   }   
}