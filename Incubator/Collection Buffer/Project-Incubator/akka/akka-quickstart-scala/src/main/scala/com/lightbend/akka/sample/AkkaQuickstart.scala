//-----------------------------------Starter Exampler-----------------------------------//

////#full-example
//package com.lightbend.akka.sample
//
//import akka.actor.{ Actor, ActorLogging, ActorRef, ActorSystem, Props }
//import scala.io.StdIn
//
////#greeter-companion
////#greeter-messages
//object Greeter {
//  //#greeter-messages
//  def props(message: String, printerActor: ActorRef): Props = Props(new Greeter(message, printerActor))
//  //#greeter-messages
//  final case class WhoToGreet(who: String)
//  case object Greet
//}
////#greeter-messages
////#greeter-companion
//
////#greeter-actor
//class Greeter(message: String, printerActor: ActorRef) extends Actor {
//  import Greeter._
//  import Printer._
//
//  var greeting = ""
//
//  def receive = {
//    case WhoToGreet(who) =>
//      greeting = s"$message!     $who"
//    case Greet           =>
//      //#greeter-send-message
//      printerActor ! Greeting(greeting)
//      //#greeter-send-message
//  }
//}
////#greeter-actor
//
////#printer-companion
////#printer-messages
//object Printer {
//  //#printer-messages
//  def props: Props = Props[Printer]
//  //#printer-messages
//  final case class Greeting(greeting: String)
//}
////#printer-messages
////#printer-companion
//
////#printer-actor
//class Printer extends Actor with ActorLogging {
//  import Printer._
//
//  def receive = {
//    case Greeting(greeting) =>
//      log.info(s"Greeting received (from ${sender()}):\n\n $greeting\n")
//  }
//}
////#printer-actor
//
////#main-class
//object AkkaQuickstart extends App {
//  import Greeter._
//
//  // Create the 'helloAkka' actor system
//  val system: ActorSystem = ActorSystem("helloAkka")
//
//  try {
//    //#create-actors
//    // Create the printer actor
//    val printer: ActorRef = system.actorOf(Printer.props, "printerActor")
//
//    // Create the 'greeter' actors
//    val morningGreeter: ActorRef =
//      system.actorOf(Greeter.props("Good Morning", printer), "morningGrtrName") //'Howdy' before
//    val afternoonGreeter: ActorRef =
//      system.actorOf(Greeter.props("Good Afternoon", printer), "afternoonGrtrName") //'Hello' before
//    val eveningGreeter: ActorRef =
//      system.actorOf(Greeter.props("Good Evening", printer), "eveningGrtrName") //'Good day' before
//    val todoGreeter: ActorRef =
//      system.actorOf(Greeter.props("ToDo", printer), "todoGrtrName")
//    //#create-actors
//
//    //#main-send-messages
//    morningGreeter ! WhoToGreet("AkkaDoExist")
//    morningGreeter ! Greet
//
//    morningGreeter ! WhoToGreet("Lightbend")
//    morningGreeter ! Greet
//
//    afternoonGreeter ! WhoToGreet("ScalaNeededBeInstalled")
//    afternoonGreeter ! Greet
//
//    afternoonGreeter ! WhoToGreet("Java8NeededBeInstalled")
//    afternoonGreeter ! Greet
//
//    eveningGreeter ! WhoToGreet("PlayNotHere")
//    eveningGreeter ! Greet
//
//    todoGreeter ! WhoToGreet("Understand SBT")
//    todoGreeter ! Greet
//    //#main-send-messages
//
//    println(">>> Press ENTER to exit <<<")
//    StdIn.readLine()
//  } finally {
//    system.terminate()
//  }
//}
////#main-class
////#full-example



package com.lightbend.akka.sample

import akka.actor.{ Actor, ActorRef, ActorSystem, ActorLogging, Props}
import scala.io.StdIn

object Baby {
  def props(action: String, callerActor: ActorRef): Props =  Props(new Baby(action, callerActor))
  final case class BabyName(who: String)
  case object Call
}

class Baby(action: String, callerActor: ActorRef) extends Actor {
  import Baby._
  import Caller._

  var msg = ""

  def receive = {
    case BabyName(who) =>
      msg = s"${who} ${action}"
    case Call =>
      callerActor ! Calling(msg)
  }
}

object Caller {
  def props: Props = Props[Caller]
  final case class Calling(msg: String)
}

class Caller extends Actor with ActorLogging {
  import Caller._
  def receive = {
    case Calling(msg) =>
      log.info(msg)
  }
}

object BabyAkka extends App {
  import Baby._

  val system: ActorSystem = ActorSystem("BabyAkkaSys")
  try {
    val momCaller: ActorRef = system.actorOf(Caller.props, "momCaller")
    val dadCaller: ActorRef = system.actorOf(Caller.props, "dadCaller")
    val babyFeelHungry: ActorRef = system.actorOf(Baby.props("feels hungry!", momCaller), "bbFeelHungry")
    val babyWakeUp: ActorRef = system.actorOf(Baby.props("wakes up!", dadCaller), "bbWakeUp")

    babyFeelHungry ! BabyName("Jason")
    babyFeelHungry ! Call

    babyFeelHungry ! BabyName("Jack")
    babyFeelHungry ! Call

    babyWakeUp ! BabyName("Anderson")
    babyWakeUp ! Call

    babyWakeUp ! BabyName("Andy")
    babyWakeUp ! Call

    println(">>> Press ENTER to exit <<<")
    StdIn.readLine()
  } finally {
    system.terminate()
  }
}
