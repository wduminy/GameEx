# Static View
@startuml
scale 1.2 
hide methods


enum LifeStatus {
 Dormant
 Active
 Destroyed
} 

class Animate { 
 + location
}


Animate -> LifeStatus
game.CollidablePolygon <|-- Animate

class Drome {
+ is static? 
+ image-index
+ update and draw
}

game.GameObject <|-- Drome
Animate <|-- Drome

class Shooter {
+ moves
}
Animate <|-- Shooter


class ShooterState {
- encapsulates game state
+ updates game state
+ handles input
+ readsXML
}
note right of ShooterState
 <color:red>TODO
  move shooter 
  and warzone specific code 
  outa here?
end note
ShooterState *--> Shooter
ShooterState *--> WarZone

game.GameObjectWithParts <|-- ShooterState
ShooterState *-- Drome: "parts"


class WarZone {
- encapsulate map data (BMP)
}

class ShooterController {
+ handles macro state changes
}

ShooterController *--> ShooterState
game.MainObject <|-- ShooterController
@enduml

# Load and draw sequence
@startuml
== init ==
main -> ShooterController : construct
ShooterController ->  ShooterState : construct
ShooterState -> Shooter : construct
main -> Game : construct and run
Game -> ShooterController: init
ShooterController -> ShooterState: init
ShooterState -> WarZone: construct
ShooterState -> Drone: build drones as parts
== update and draw ==
Game -> ShooterController : update
ShooterController -> ShooterState: update 
ShooterState -> Drone : update parts
ShooterController -> ShooterState : draw
ShooterState -> Drone : draw parts
@enduml
